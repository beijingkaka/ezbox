/* linux/drivers/net/solos_ethernet.c
 *
 *
 *      Copyright (c) 2004-2005 Conexant 
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Solos driver for on-chip ethernet MACs. Portions of the MII code based on the 
 * Helium 500 ethernet driver
 */

#include <linux/module.h>

#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/interrupt.h>
#include <linux/ptrace.h>
#include <linux/ioport.h>
#include <linux/in.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/timer.h>
#include <linux/dma-mapping.h>
#include <asm/system.h>
#include <asm/bitops.h>
#include <linux/spinlock.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/dma.h>
#include <asm/memory.h>
#include <asm/delay.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/vmalloc.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>
#include <linux/mii.h>
#include <linux/ethtool.h>

#include "asm/arch/hardware/ethernet.h"

/* Bureaucracy */
MODULE_AUTHOR("Guido Barzini");
MODULE_DESCRIPTION("Solos SoC ethernet driver");
MODULE_LICENSE("GPL");

/* Defines */
#define READ_REG(_x) readl(_x)
#define WRITE_REG(_addr, _val) writel(_val, _addr)

#define DEFAULT_NUM_TXDESCS (8)
#define DEFAULT_NUM_RXDESCS (8)

#define SOLOS_POLL_RATE (10)	/* Times per second */

#define RX_SIZE (1536)

/* #define SOLOS_ETH_DEBUG */
/* #define SOLOS_ETH_DEBUG_VERBOSE */

#ifdef SOLOS_ETH_DEBUG_VERBOSE
#define DBG(_x...) printk(KERN_EMERG _x)
#else
#define DBG(_x...)
#endif

#define SRAM_PHYS( _x ) (((u32)_x & ~SOLOS_SRAM_VIRT) | SOLOS_SRAM_PHYS)

/* Note: 8 byte alignment required. */
struct solos_eth_desc {
	u32 ctrl;		/* Control/status field. */
	dma_addr_t ptr;		/* Data pointer. */
};

enum mii_req {
	MII_NONE,
	MII_READ,
	MII_WRITE
};

struct solos_eth_ctxt {
	int n_txdescs;
	int n_rxdescs;
	/* The descriptor rings */
	struct solos_eth_desc *tx_desc_ring;
	struct solos_eth_desc *rx_desc_ring;
	/* The pointers into the rings */
	struct solos_eth_desc *tx_desc_ptr;
	struct solos_eth_desc *rx_desc_ptr;
	struct solos_eth_desc *tx_desc_done;

	/* Pointers used for tracking the sk_buffs */
	struct sk_buff **tx_buffs;
	struct sk_buff **rx_buffs;
	/* Buffers for Tx */
	struct sk_buff_head tx_queue;
	struct net_device_stats stats;

	/* PHY handling */
	struct mii_if_info mii;
	u16 mii_id_low;
	u16 mii_id_hi;
	struct semaphore mutex;
	spinlock_t lock;
	enum mii_req mii_pending;
	u16 mii_state;
};

/* Globals */
static struct net_device solos_devs[SOLOS_ETHER_MAX_PORTS];

static struct solos_eth_ctxt solos_privs[SOLOS_ETHER_MAX_PORTS];
/* Both MACs are on the upper AHB */
static u32 solos_eth_addrs[SOLOS_ETHER_MAX_PORTS] =
	{
	SOLOS_UAHB_VIRT + SOLOS_MAC0_BASE
#if (SOLOS_ETHER_MAX_PORTS == 2)
	, SOLOS_UAHB_VIRT + SOLOS_MAC1_BASE
#endif 
	};
static u32 solos_eth_irqs[SOLOS_ETHER_MAX_PORTS] = 
	{ 
	IRQ_ETH0
#if (SOLOS_ETHER_MAX_PORTS == 2)
	, IRQ_ETH1 
#endif
	};

static const char *devname = "solos_eth";

/* There is only one thread to manage, so no need to duplicate these */
static struct completion thread_exit_complete;
static volatile int thread_exit_request;

static struct net_device_stats *solos_get_stats(struct net_device *dev)
{
	struct solos_eth_ctxt *ctxt = (struct solos_eth_ctxt *)dev->priv;
	return &(ctxt->stats);
}

/*****************************************************************************
 *
 *MII Support 
 *
 */

/* If previously posted request was reading, and it is completed,
 * store contents of MII data register to result and return 1. If previous
 * operation do not match to requested, post new MII read request and return 0
 */
static inline int solos_mii_read_posted(struct net_device *dev, int phy,
					int reg, short *result)
{
	struct solos_eth_ctxt *ctxt = (struct net_local *)dev->priv;
	unsigned int status;

	status = READ_REG(dev->base_addr + SOLOS_ETHER_STAT);

	if (ctxt->mii_pending && !(status & SOLOS_ETHER_STAT_MDIO)) {	/* Is the last MII op complete? */
		/* No. caller will just have to try again later... */
		return 0;
	}

	/* Clear the ready bit... */
	WRITE_REG(dev->base_addr + SOLOS_ETHER_STAT, SOLOS_ETHER_STAT_MDIO);

	if (ctxt->mii_pending == MII_READ) {
		*result =
		    READ_REG(dev->base_addr +
			     SOLOS_ETHER_MDIO) & SOLOS_ETHER_MDIO_DATA_MASK;
		ctxt->mii_pending = MII_NONE;
		return 1;
	} else {
		u32 request;
		request = SOLOS_ETHER_MDIO_OP_READ | SOLOS_ETHER_MDIO_SFD | SOLOS_ETHER_MDIO_TA	/* Set up a read */
		    | (phy << SOLOS_ETHER_MDIO_PHY_SHIFT) | (reg <<
							     SOLOS_ETHER_MDIO_REG_SHIFT);
		/* Post read request */
		WRITE_REG(dev->base_addr + SOLOS_ETHER_MDIO, request);
		ctxt->mii_pending = MII_READ;
		return 0;
	}
}

/* Posted Write to specified Ethernet PHY through MII. Return 0 if MII is
 * busy, 1 if write request posted */
static inline int solos_mii_write_posted(struct net_device *dev, int phy,
					 int reg, unsigned short value)
{
	struct solos_eth_ctxt *ctxt = (struct net_local *)dev->priv;
	unsigned int status;
	u32 request;

	status = READ_REG(dev->base_addr + SOLOS_ETHER_STAT);

	if (ctxt->mii_pending && !(status & SOLOS_ETHER_STAT_MDIO)) {	/* Is the last MII op complete? */
		/* No. caller will just have to try again later... */
		return 0;
	}

	/* Clear the ready bit... */
	WRITE_REG(dev->base_addr + SOLOS_ETHER_STAT, SOLOS_ETHER_STAT_MDIO);

	request = SOLOS_ETHER_MDIO_OP_READ | SOLOS_ETHER_MDIO_SFD | SOLOS_ETHER_MDIO_TA	/* Set up a write */
	    | (phy << SOLOS_ETHER_MDIO_PHY_SHIFT) | (reg <<
						     SOLOS_ETHER_MDIO_REG_SHIFT)
	    | value;

	WRITE_REG(dev->base_addr + SOLOS_ETHER_MDIO, request);
	ctxt->mii_pending = MII_WRITE;

	return 1;
}

/* Read MII register from specified ethernet PHY */
static int solos_mii_read(struct net_device *dev, int phy, int reg)
{
	struct solos_eth_ctxt *ctxt = (struct net_local *)dev->priv;
	u16 value;
	down(&ctxt->mutex);;
	while (!solos_mii_read_posted(dev, phy, reg, &value)) ;
	up(&ctxt->mutex);
	return value;
}

/* Write MII register to specified Ethernet PHY */
static void solos_mii_write(struct net_device *dev, int phy, int reg, int value)
{
	struct solos_eth_ctxt *ctxt = (struct net_local *)dev->priv;
	down(&ctxt->mutex);
	while (!solos_mii_write_posted(dev, phy, reg, value)) ;
	up(&ctxt->mutex);
	return;
}

static void solos_scan_mii(struct net_device *dev)
{
	struct solos_eth_ctxt *ctxt = (struct net_local *)dev->priv;
	int addr;
	for (addr = 0; addr < 32; addr++) {
		u16 idlo = solos_mii_read(dev, addr, MII_PHYSID1);
		u16 idhi = solos_mii_read(dev, addr, MII_PHYSID2);

		/* If there's nothing there we expect to get back all 1s or all 0s */
		if ((idlo == 0) || (idhi == 0xffff))
			continue;
		printk(KERN_INFO
		       "MAC %d: found PHY at MII addr %d with id %.4x%.4x\n",
		       dev->dma, addr, idhi, idlo);
		ctxt->mii.phy_id = addr;
		ctxt->mii.phy_id_mask = 0x1f;
		ctxt->mii.reg_num_mask = 0x1f;
		ctxt->mii.dev = dev;
		ctxt->mii.mdio_read = solos_mii_read;
		ctxt->mii.mdio_write = solos_mii_write;
		ctxt->mii_id_low = idlo;
		ctxt->mii_id_hi = idhi;
		/* Avoid a spurious link state change when we start polling */
		ctxt->mii_state = solos_mii_read(dev, addr, MII_BMSR);
	}
}

/* This thread polls the PHY at intervals to adjust the settings of the MAC. The
 * only thing we really need to do is update the duplex status, but in debug builds
 * we are more informative. */
static int solos_eth_thread(void *dummy)
{
	daemonize("ethmaint");
	while (1) {
		int i;
		for (i = 0; i < SOLOS_ETHER_MAX_PORTS; i++) {
			if (solos_privs[i].mii.phy_id != -1) {
				struct ethtool_cmd cmd;
				u16 tmp = solos_privs[i].mii.full_duplex;
				mii_ethtool_gset(&solos_privs[i].mii, &cmd);
				if (cmd.duplex != tmp) {
					u32 ctrl;
					printk(KERN_INFO
					       "Changing duplex setting for MAC %d to %s\n",
					       i,
					       cmd.duplex ? "true" : "false");
					spin_lock_irq(&solos_privs[i].lock);
					ctrl =
					    READ_REG(solos_eth_addrs[i] +
						     SOLOS_ETHER_CONTROL);
					if (cmd.duplex)
						ctrl |=
						    SOLOS_ETHER_CONTROL_ENBFULL;
					else
						ctrl &=
						    ~SOLOS_ETHER_CONTROL_ENBFULL;
					WRITE_REG(solos_eth_addrs[i] +
						  SOLOS_ETHER_CONTROL, ctrl);
					spin_unlock_irq(&solos_privs[i].lock);
				}
#ifdef SOLOS_ETH_DEBUG
				tmp =
				    solos_mii_read(&solos_devs[i],
						   solos_privs[i].mii.phy_id,
						   MII_BMSR);
				if (tmp != solos_privs[i].mii_state) {
					printk(KERN_INFO
					       "Status change for %s: Link speed %d Link status %s\n",
					       solos_devs[i].name, cmd.speed,
					       (tmp & BMSR_LSTATUS) ? "ON" :
					       "OFF");
					solos_privs[i].mii_state = tmp;
				}
#endif
			}
		}
		set_current_state(TASK_INTERRUPTIBLE);
		schedule_timeout(HZ / SOLOS_POLL_RATE);
		if (thread_exit_request)
			break;
	}
	complete_and_exit(&thread_exit_complete, 0);
	return 0;
}

/*****************************************************************************
 *
 * Descriptor and Tx/Rx code 
 *
 */

static int solos_alloc_descriptors(struct net_device *dev,
				   struct solos_eth_ctxt *ctxt)
{
	int err = 0;
	int i;
	if (ctxt->tx_desc_ring == NULL) {
		/* Put the desciptor ring in uncached SRAM. This isn't single-cycle,
		 * but given how small the descriptors are it's probably quicker to
		 * fetch them from (quite fast) on-chip RAM than mess about with cache
		 * management. We will need to align this to 8 bytes. */
		struct solos_eth_desc *mem =
		    solos_sram_alloc(sizeof(struct solos_eth_desc) *
				     (ctxt->n_txdescs + ctxt->n_rxdescs) + 7);
		u32 tmp = (u32) mem;
		if (mem == NULL) {
			DBG("failed to alloc descriptor ring\n");
			return -ENOMEM;
		}
		/* Align the pointer */
		tmp = (tmp + 7) & (~7);
		mem = (struct solos_eth_desc *)tmp;
		ctxt->tx_desc_ring = mem;
		ctxt->rx_desc_ring = mem + ctxt->n_txdescs;
	}
	/* Set up the pointers into the rings */
	ctxt->tx_desc_ptr = ctxt->tx_desc_ring;
	ctxt->rx_desc_ptr = ctxt->rx_desc_ring;
	ctxt->tx_desc_done = ctxt->tx_desc_ring;

	/* Any old memory will do for the buffer array: onlu code touches it */
	ctxt->tx_buffs = vmalloc(ctxt->n_txdescs * sizeof(struct sk_buff *));
	ctxt->rx_buffs = vmalloc(ctxt->n_rxdescs * sizeof(struct sk_buff *));

	if ((ctxt->tx_buffs) == NULL || (ctxt->rx_buffs == NULL)) {
		DBG("failed to alloc %ld + %ld bytes for buffer pointers\n",
		    ctxt->n_txdescs * sizeof(struct sk_buff *),
		    ctxt->n_rxdescs * sizeof(struct sk_buff *));
		err = -ENOMEM;
		goto vmalloc_failed;
	}
	/* OK, we have a full set of descriptors. Now set them up. */

	memset(ctxt->tx_desc_ring, 0,
	       sizeof(struct solos_eth_desc) * ctxt->n_txdescs);

	memset(ctxt->tx_buffs, 0, ctxt->n_txdescs * sizeof(struct sk_buff *));
	/* Zap the Rx descriptors too */
	for (i = 0; i < ctxt->n_rxdescs; i++) {
		struct sk_buff *skb = dev_alloc_skb(RX_SIZE + NET_IP_ALIGN);
		if (skb == NULL) {
			DBG("failed to alloc skb\n");
			err = -ENOMEM;
			goto no_skbs;
		}
		skb_reserve(skb, NET_IP_ALIGN);
		ctxt->rx_desc_ring[i].ctrl =
		    SOLOS_ETHER_RXINFO_CPU_OWN | RX_SIZE;
		ctxt->rx_desc_ring[i].ptr =
		    dma_map_single(NULL, skb->tail, RX_SIZE, DMA_FROM_DEVICE);
		ctxt->rx_buffs[i] = skb;
	}
	return 0;

      no_skbs:
	for (i = 0; i < ctxt->n_rxdescs; i++) {
		if (ctxt->rx_buffs[i] != NULL)
			kfree_skb(ctxt->rx_buffs[i]);
	}

      vmalloc_failed:
	if (ctxt->tx_buffs != NULL) {
		vfree(ctxt->tx_buffs);
		ctxt->tx_buffs = NULL;
	}

	if (ctxt->rx_buffs != NULL) {
		vfree(ctxt->rx_buffs);
		ctxt->rx_buffs = NULL;
	}
	return err;
}

static void solos_do_rx(struct net_device *dev, struct solos_eth_ctxt *ctxt)
{
	struct solos_eth_desc *desc = ctxt->rx_desc_ptr;
	int idx = (desc - ctxt->rx_desc_ring);
	while (!(desc->ctrl & SOLOS_ETHER_RXINFO_OWN)) {
		struct sk_buff *rx_skb = ctxt->rx_buffs[idx];
		struct sk_buff *refill = dev_alloc_skb(RX_SIZE + NET_IP_ALIGN);

		if (refill != NULL) {
			int len;
			skb_reserve(refill, NET_IP_ALIGN);
			ctxt->rx_buffs[idx] = refill;
			desc->ptr =
			    dma_map_single(NULL, refill->tail, RX_SIZE,
					   DMA_FROM_DEVICE);
			len = desc->ctrl & SOLOS_ETHER_RXINFO_RX_LEN;
			if (len >= 64) {
				len -= 4;
			}
			skb_put(rx_skb, len);
			rx_skb->dev = dev;
			rx_skb->protocol = eth_type_trans(rx_skb, dev);
			DBG("Rx: buffer %p, len %d, starts %.2x %.2x %.2x %.2x\n", rx_skb, rx_skb->len, rx_skb->data[0], rx_skb->data[1], rx_skb->data[2], rx_skb->data[3]);
			ctxt->stats.rx_packets++;
			ctxt->stats.rx_bytes += len;
			netif_rx(rx_skb);
		} else {
			/*  just carry on with existing skb. and record the drop */
			ctxt->stats.rx_dropped++;
		}
		desc->ctrl = SOLOS_ETHER_RXINFO_CPU_OWN | RX_SIZE;
		idx++;
		if (idx == ctxt->n_rxdescs) {
			idx = 0;
			desc = ctxt->rx_desc_ring;
		} else {
			desc++;
		}
	}
	ctxt->rx_desc_ptr = desc;
}

static void solos_kick_tx(struct solos_eth_ctxt *ctxt, u32 addr)
{
	DBG("solos_kick_tx, ctxt %p, tx_desc_ptr %p\n", ctxt,
	    ctxt->tx_desc_ptr);
	/* Is there any space in the descriptor ring? */
	if (!(ctxt->tx_desc_ptr->ctrl & SOLOS_ETHER_TXINFO_OWN)
	    && (ctxt->tx_desc_ptr->ptr == 0)) {
		/* Queue the buffer */
		struct sk_buff *buf = skb_dequeue(&ctxt->tx_queue);
		DBG("slot is empty\n");
		if (buf) {
			int idx = ctxt->tx_desc_ptr - ctxt->tx_desc_ring;
			DBG("got a buffer to queue.\n");
			ctxt->stats.tx_packets++;
			ctxt->stats.tx_bytes += buf->len;

			/* Remember where we put the skb */
			ctxt->tx_buffs[idx++] = buf;

			/* Pass the skb off to the hardware */
			ctxt->tx_desc_ptr->ptr =
			    dma_map_single(NULL, buf->data, buf->len,
					   DMA_TO_DEVICE);
			ctxt->tx_desc_ptr->ctrl =
			    buf->len | SOLOS_ETHER_TXINFO_CPU_OWN |
			    SOLOS_ETHER_TXINFO_CPU_FIRST |
			    SOLOS_ETHER_TXINFO_CPU_LAST;
			DBG("recording buffer location: idx = %d, tx_buffs = %p\n", idx, ctxt->tx_buffs);

			/* Handle wrap */
			ctxt->tx_desc_ptr++;
			if (idx == ctxt->n_txdescs)
				ctxt->tx_desc_ptr = ctxt->tx_desc_ring;

			/* Prod the MAC */
			WRITE_REG(addr + SOLOS_ETHER_STAT,
				  SOLOS_ETHER_STAT_TXPOLL);
		}
	}
}

static irqreturn_t solos_eth_irq(int irq, void *dev_id, struct pt_regs *regs)
{
	struct net_device *dev = (struct net_device *)dev_id;
	struct solos_eth_ctxt *ctxt = (struct solos_eth_ctxt *)dev->priv;
	u32 status = READ_REG(dev->base_addr + SOLOS_ETHER_STAT);

	/* Do the receives first as the most urgent thing */
	if (status & SOLOS_ETHER_STAT_RXINT) {
		DBG("Got Rx IRQ\n");
		solos_do_rx(dev, ctxt);
	}

	if (status & SOLOS_ETHER_STAT_TXINT) {
		DBG("Got Tx IRQ\n");
		/* Clean out any done frames. */
		while (!(ctxt->tx_desc_done->ctrl & SOLOS_ETHER_TXINFO_OWN)
		       && (ctxt->tx_desc_done->ptr != 0)) {
			u32 idx = ctxt->tx_desc_done - ctxt->tx_desc_ring;
			struct sk_buff *done_buf = ctxt->tx_buffs[idx];
			BUG_ON(done_buf == NULL);	/* We must be confused */
			DBG("Free buffer %p in slot %p. Desc status was %x\n",
			    done_buf, ctxt->tx_desc_done,
			    ctxt->tx_desc_done->ctrl);
			dev_kfree_skb_irq(done_buf);	/* Review: if we implement NAPI this will need changing */
			ctxt->tx_desc_done->ptr = 0;
			ctxt->tx_buffs[idx] = 0;
			idx++;
			ctxt->tx_desc_done++;
			if (idx == ctxt->n_txdescs) {
				ctxt->tx_desc_done = ctxt->tx_desc_ring;
			}
		}
		/* Queue any pneding frames */
		solos_kick_tx(ctxt, dev->base_addr);
	}

	if (status & (SOLOS_ETHER_STAT_TXCH | SOLOS_ETHER_STAT_MISSERR |
		      SOLOS_ETHER_STAT_RXCRC | SOLOS_ETHER_STAT_RXFRAME |
		      SOLOS_ETHER_STAT_RXOFLOW)) {
		if ( status & SOLOS_ETHER_STAT_MISSERR) ctxt->stats.rx_missed_errors++;
		if ( status & SOLOS_ETHER_STAT_RXCRC) ctxt->stats.rx_crc_errors++;
		if ( status & SOLOS_ETHER_STAT_RXFRAME) ctxt->stats.rx_frame_errors++;
		if ( status & SOLOS_ETHER_STAT_RXOFLOW) ctxt->stats.rx_over_errors++;
		if ( status & SOLOS_ETHER_STAT_TXCH ) ctxt->stats.tx_errors++;
		else ctxt->stats.rx_errors++;
	}
	/* We do NOT want to clear the MDIO completion bit as that is being polled from process context */
	status &= ~SOLOS_ETHER_STAT_MDIO;
	WRITE_REG(dev->base_addr + SOLOS_ETHER_STAT, status);
	return IRQ_HANDLED;
}

static int solos_eth_tx(struct sk_buff *skb, struct net_device *dev)
{
	struct solos_eth_ctxt *ctxt = (struct solos_eth_ctxt *)dev->priv;
	/* REVIEW: if the network stack is already serialising this we can use
	 * __skb_queue_tail and skip the expense of locking. */
	skb_queue_tail(&ctxt->tx_queue, skb);
	solos_kick_tx(ctxt, dev->base_addr);
	return 0;
}

static void solos_stop_hw(struct net_device *dev)
{
	struct solos_eth_ctxt *ctxt = (struct solos_eth_ctxt *)dev->priv;
	int i;
	int not_stuck = 1000000;

	/* Stop the hardware */
	WRITE_REG(dev->base_addr + SOLOS_ETHER_CONTROL, 0);
	/* Turn off all interrupts */
	WRITE_REG(dev->base_addr + SOLOS_ETHER_ENABLE, 0);

	/* Zap the descriptor ring pointers */
	WRITE_REG(dev->base_addr + SOLOS_ETHER_TXRINGPTR, 0);
	WRITE_REG(dev->base_addr + SOLOS_ETHER_RXRINGPTR, 0);

	/* Do this while we wait for the DMA to wind down... */
	__skb_queue_purge(&ctxt->tx_queue);

	/* Now spin until the DMA really does stop */
	while (not_stuck
	       && (READ_REG(dev->base_addr + SOLOS_ETHER_TXPTRREAD) |
		   READ_REG(dev->base_addr + SOLOS_ETHER_RXPTRREAD)))
		not_stuck--;

	if (!not_stuck)
		printk(KERN_WARNING
		       "Seem not to have quiesced DMA. Just giving up and hoping.\n");

	/* And clear out the descriptor rings */
	for (i = 0; i < ctxt->n_txdescs; i++) {
		ctxt->tx_desc_ring[i].ctrl = 0;
		ctxt->tx_desc_ring[i].ptr = 0;

		if (ctxt->tx_buffs[i] != NULL)
			dev_kfree_skb(ctxt->tx_buffs[i]);
	}
	/* And clear out the descriptor rings */
	for (i = 0; i < ctxt->n_rxdescs; i++) {
		ctxt->rx_desc_ring[i].ctrl = 0;
		ctxt->rx_desc_ring[i].ptr = 0;

		if (ctxt->rx_buffs[i] != NULL)
			dev_kfree_skb(ctxt->rx_buffs[i]);
	};
}

static int solos_eth_open(struct net_device *dev)
{
	struct solos_eth_ctxt *ctxt = (struct solos_eth_ctxt *)dev->priv;
	u32 tmp;
	int status = 0;

	DBG("solos_eth_open\n");
	/* Set up the DMA descriptors */
	status = solos_alloc_descriptors(dev, ctxt);
	DBG("descriptors allocated returned %d\n", status);
	/* Set up the buffer Q */
	skb_queue_head_init(&ctxt->tx_queue);
	/* Set up the IRQ */
	if (status == 0)
		status = request_irq(dev->irq, solos_eth_irq, 0, devname, dev);
	if (status != 0)
		return status;
	DBG("got IRQ %d\n", dev->irq);
	/* Program up the hardware. */
	WRITE_REG(dev->base_addr + SOLOS_ETHER_TXRINGPTR,
		  SRAM_PHYS(ctxt->tx_desc_ring));
	WRITE_REG(dev->base_addr + SOLOS_ETHER_RXRINGPTR,
		  SRAM_PHYS(ctxt->rx_desc_ring));
	tmp =
	    dev->dev_addr[0] | (dev->dev_addr[1] << 8) | (dev->
							  dev_addr[2] << 16) |
	    (dev->dev_addr[3] << 24);
	WRITE_REG(dev->base_addr + SOLOS_ETHER_ADDRL, tmp);
	tmp = dev->dev_addr[4] | (dev->dev_addr[5] << 8);
	WRITE_REG(dev->base_addr + SOLOS_ETHER_ADDRH, tmp);

	WRITE_REG(dev->base_addr + SOLOS_ETHER_ENABLE,
		  SOLOS_ETHER_STAT_TXINT | SOLOS_ETHER_STAT_RXINT |
		  SOLOS_ETHER_STAT_ERR);

	/* Turn on misc extra features */
	tmp = READ_REG(dev->base_addr + SOLOS_ETHER_XTRACTRL);
	tmp |= SOLOS_ETHER_XTRACTRL_BVCIFIX |
	    SOLOS_ETHER_XTRACTRL_TXAUTOPAD |
	    SOLOS_ETHER_XTRACTRL_RXBURST8 |
	    SOLOS_ETHER_XTRACTRL_TXBURST8 |
	    SOLOS_ETHER_XTRACTRL_XTRAENB | SOLOS_ETHER_XTRACTRL_LOCKFIX;
	WRITE_REG(dev->base_addr + SOLOS_ETHER_XTRACTRL, tmp);

	/* Kick thinsg off */
	tmp =
	    SOLOS_ETHER_CONTROL_ENABLE | SOLOS_ETHER_CONTROL_TXRUN |
	    SOLOS_ETHER_CONTROL_RXRUN | SOLOS_ETHER_CONTROL_ENBFULL |
	    (ctxt->n_txdescs << SOLOS_ETHER_CONTROL_TXBDTLEN_SHIFT)
	    | (ctxt->n_rxdescs << SOLOS_ETHER_CONTROL_RXBDTLEN_SHIFT);

	WRITE_REG(dev->base_addr + SOLOS_ETHER_CONTROL, tmp);

	/* We've set the MAC to FD, so set the current MII state the same so if it's wrong 
	 * the maintenance thread will spot a transition and fix it. */ 
	ctxt->mii.full_duplex = 1;

	netif_start_queue(dev);
	return status;
}

static int solos_eth_stop(struct net_device *dev)
{
	netif_stop_queue(dev);
	solos_stop_hw(dev);
	free_irq(dev->irq, dev);
	return 0;
}

/* solos_macaddr : the base MAC address has been passed to the system 
 * as the serial number. We just add on the instance to get a MAC address */
static void solos_macaddr(int instance, unsigned char *addr)
{
	addr[0] = system_serial_low & 0xff;
	addr[1] = (system_serial_low >> 8) & 0xff;
	addr[2] = (system_serial_low >> 16) & 0xff;
	addr[3] = (system_serial_low >> 24) & 0xff;
	addr[4] = system_serial_high & 0xff;
	addr[5] = ((system_serial_high >> 8) & 0xff) + instance;
}

/* Ethernet driver IOCTL handler */
static int solos_eth_ioctl(struct net_device *dev, struct ifreq *rq, int cmd)
{
	struct solos_eth_ctxt *ctxt = dev->priv;
	int rc;

	if (!netif_running(dev))
		return -EINVAL;

	spin_lock_irq(&ctxt->lock);
	rc = generic_mii_ioctl(&ctxt->mii, if_mii(rq), cmd, NULL);
	spin_unlock_irq(&ctxt->lock);

	return rc;
}

/* solos_ether_init: init the device. We know it corresponds to valid device since 
 * it would not have been registered otherwise. */
static int solos_eth_init(struct net_device *dev)
{
	int status = 0;
	ether_setup(dev);
	dev->open = solos_eth_open;
	dev->stop = solos_eth_stop;
	dev->hard_start_xmit = solos_eth_tx;
	dev->get_stats = solos_get_stats;
	dev->do_ioctl = solos_eth_ioctl;
	solos_macaddr(dev->dma /* Actually the instance number. */ ,
		      dev->dev_addr);
	return status;
}

int __init solos_ether_load(void)
{
	int status = 0;
	int i, n;
	n = 0;
	for (i = 0; i < SOLOS_ETHER_MAX_PORTS; i++) {
		u32 base_addr = solos_eth_addrs[i];
		u32 tmp;
		solos_privs[i].mii.phy_id = -1;
		if (READ_REG(base_addr + SOLOS_ETHER_ID) ==
		    SOLOS_ETHER_ID_VALUE) {
			struct net_device *dev = &solos_devs[n];
			n++;
			dev->base_addr = base_addr;
			dev->irq = solos_eth_irqs[i];
			dev->init = solos_eth_init;
			dev->dma = i;	/* Slight abuse, but it's a handy place to store this. */
			/* Set up the context pointer. */
			dev->priv = &solos_privs[i];
			spin_lock_init(&solos_privs[i].lock);
			init_MUTEX(&solos_privs[i].mutex);
			solos_privs[i].n_txdescs = DEFAULT_NUM_TXDESCS;
			solos_privs[i].n_rxdescs = DEFAULT_NUM_RXDESCS;
			/* Init. some register bits that are undefined on reset. */
			tmp = READ_REG(base_addr + SOLOS_ETHER_XTRACTRL);
			tmp &= ~SOLOS_ETHER_XTRACTRL_RESETMASK;
			WRITE_REG(base_addr + SOLOS_ETHER_XTRACTRL, tmp);
			if ((status = register_netdev(dev)) == 0) {
				printk(KERN_INFO
				       "Registered 10/100 MAC at 0x%x\n",
				       base_addr);
				solos_scan_mii(dev);
			} else
				printk(KERN_WARNING
				       "Failed to register 10/100 MAC at 0x%x: %d\n",
				       base_addr, status);
		}
	}
	if (n == 0) {
		printk(KERN_WARNING
		       "Failed to discover any MAC blocks. Suspicious as they are built-in!\n");
		status = -ENODEV;
	} else {
		int rc;
		thread_exit_request = 0;
		init_completion(&thread_exit_complete);
		rc = kernel_thread(solos_eth_thread, NULL, CLONE_FS);
		if (rc < 0)
			printk(KERN_WARNING
			       "Failed to start ethernet maintenance thread: %d\n",
			       rc);

	}

	return status;
}

/* Module unload: just unregister all devices. */
void __exit solos_ether_unload(void)
{
	int i;
	thread_exit_request = 1;
	for (i = 0; i < SOLOS_ETHER_MAX_PORTS; i++) {
		struct net_device *dev = &solos_devs[i];
		if (dev->priv != NULL) {
			unregister_netdev(dev);
		}
	}
	wait_for_completion(&thread_exit_complete);
}

module_init(solos_ether_load)
module_exit(solos_ether_unload)
