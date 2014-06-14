/*
 * linux/arch/arm/mach-solos/solos_spi.c
 *
 * Author: Guido Barzini
 * Copyright (C) 2005 Conexant inc.
 *
 * Reimplementation of the SPI API used by a91_dataflash.c to
 * allow it to be used with Solos
 */

#include <linux/config.h>
#include <linux/completion.h>
#include <linux/dma-mapping.h>
#include <linux/interrupt.h>
#include <asm/delay.h>
#include <asm/io.h>
#include <asm/hardware.h>
#include <asm/arch/hardware/spi.h>
#include <linux/device.h>
#include <linux/spi/spi.h>
#include <linux/list.h>

#define CHARM_CFG_SPICTRL   (0xFF000034)

#ifndef CONFIG_SOLOS_SPI
#include <linux/at91_spi.h>
#define SPI_DEFAULT_DIVIDER (4)

#ifdef SOLOS_SPI_DBG
#define SPIDBG( _x... ) prink(_x)
#else
#define SPIDBG( _x... ) do {} while(0)
#endif

static int spi_cs;
static int spi_bus;

#else

#define SPI_DEFAULT_DIVIDER (2)
static int spi_devices; /* No. of SPI masters probed */
#ifdef CONFIG_ARCH_SOLOSW_GALLUS
static const char *dev_name[1] = {"spi0"};
#else
static const char *dev_name[2] = {"spi0", "spi1"};
#endif
#endif /* !CONFIG_SOLOS_SPI */


struct  spi_dev {
	u32 ctrl;
	u32 div;
	u32 baseaddr;
	struct completion spi_comp;
};

#ifdef CONFIG_ARCH_SOLOSW_GALLUS
static struct spi_dev spi_devs[1];
#else
static struct spi_dev spi_devs[2];
#endif


#ifndef CONFIG_SOLOS_SPI
DECLARE_MUTEX(spi_mutex); /* Unlocked */

void spi_access_bus(short device)
{
	down(&spi_mutex);
	spi_cs = (device & 3 );
	spi_bus = (device >> 2 );
}

void spi_release_bus(short device)
{
	up(&spi_mutex);
}
#endif /* !CONFIG_SOLOS_SPI */



static void spi_setup_hw(struct spi_dev *spi, int bus, int cs, int clkhi, int altphase)
{
	u32 ctrl;
	u32 aux = spi->div | SOLO_SPI_AUXCTRL_ENAB;
	aux |= (clkhi) ? SOLO_SPI_AUXCTRL_CLKHI : SOLO_SPI_AUXCTRL_CLKLO; /* Does the clock idle high or low? */
	aux |= (clkhi == altphase) ? (SOLOS_SPI_AUXCTRL_OUTEDGE_LO | SOLOS_SPI_AUXCTRL_INEDGE_HI) : (SOLOS_SPI_AUXCTRL_OUTEDGE_HI | SOLOS_SPI_AUXCTRL_INEDGE_LO);
	
	//	printk(KERN_CRIT "%s: bus(%d), cs(%d), clkhi(%d), altphase(%d).\n", __FUNCTION__, bus, cs, clkhi, altphase);

	writel(aux | SOLO_SPI_AUXCTRL_CLEAR, spi->baseaddr + SOLOS_SPI_AUXCTRL);
	writel(aux, spi->baseaddr + SOLOS_SPI_AUXCTRL);
	//	printk("Wrote %x to AUX on bus %d\n", aux, bus);

	ctrl = SOLOS_SPI_CS_RXON | SOLOS_SPI_CS_CHIPSEL(cs);

#ifndef CONFIG_SOLOS_SPI
	ctrl |= SOLOS_SPI_CS_STARTBIT_31 | SOLOS_SPI_CS_TXWIDTH_32 | SOLOS_SPI_CS_FRAMEDELAY_1;
#else
	ctrl |= SOLOS_SPI_CS_STARTBIT_31 | SOLOS_SPI_CS_TXWIDTH_32;
#endif /* !CONFIG_SOLOS_SPI */

	writel(ctrl, spi->baseaddr + SOLOS_SPI_CTRLSTAT);
	//	printk("Wrote %x to CS on bus %d\n", ctrl, bus);
	if (bus == 0)
	  writel(0, CHM_BASE_VIRT + CHM_SPICTRL_OFFSET);
}

int spi_transact(int bus, int cs, const u8 *outdata, int txlen, u8 *indata, int rxlen, u32 flags)
{
	struct spi_dev *spi = &spi_devs[bus];
	int txpos = 0;
	int rxpos = 0;
	u32 ctrl;
	int inbytes = 0;

	spi_setup_hw(spi, bus, cs, flags & SPI_CLCKHI, flags & SPI_ALTPHASE);

	ctrl = (readl(spi->baseaddr + SOLOS_SPI_CTRLSTAT) & ~(SOLOS_SPI_CS_TXLEN_MASK | SOLOS_SPI_CS_TXAUTO )) | SOLOS_SPI_CS_TXAUTO  | 8 * (txlen + rxlen);
	/* Fill the Tx FIFO */
	while( txpos < txlen ) {
	  u32 status = readl(spi->baseaddr + SOLOS_SPI_CTRLSTAT);
	  if ( status & SOLOS_SPI_CS_TXSPACE ) {

		  u32 val = 0;
		  int j;
		  for (j = 3; j >= 0; j-- ) {
		    if (txpos < txlen )
		      val |= (outdata[txpos++] << (8 * j) );
		  } 
		  //printk(KERN_CRIT "%s: TX -> 0x%.8x (txlen = %d)\n", __FUNCTION__, val, txlen);
		  writel(val,spi->baseaddr + SOLOS_SPI_TXFIFO);
		}
	}
	
	/* Set the block read bit so we will continue to clock once the TX fifo is empty */
	if ( rxlen > 0 )
	  ctrl |= SOLOS_SPI_CS_READMINUS;
	
	/* Kick the transfer off */
	writel(ctrl, spi->baseaddr + SOLOS_SPI_CTRLSTAT);

	while( inbytes < (rxlen + txlen) ) {
		u32 status = readl(spi->baseaddr + SOLOS_SPI_CTRLSTAT);
		if ( status & SOLOS_SPI_CS_RXDATA) {
			int j, n;

			u32 data = readl(spi->baseaddr + SOLOS_SPI_RXFIFO);
			n = ( rxlen + txlen - inbytes > 3 ) ? 3 : ( rxlen + txlen - inbytes ) - 1;
			for( j = n; j >= 0; j-- ) {
				if ((rxpos < rxlen) && (inbytes >= txlen) ){
				  //printk(KERN_CRIT "%s: byte -> 0x%.2x (rxlen = %d)\n", __FUNCTION__, (data >> (8 * j)) & 0xff, rxlen);
					indata[rxpos++] = (data >> (8 * j)) & 0xff;
				}
				inbytes++;
			}
		}
	}
	while(!(readl(spi->baseaddr + SOLOS_SPI_CTRLSTAT) & SOLOS_SPI_CS_TXIDLE) );
	
	return 0;
}


#ifndef CONFIG_SOLOS_SPI
int spi_transfer(struct spi_transfer_list* list)
{
	int i, rc = 0;
	for (i = 0; i < list->nr_transfers; i++) {
		rc = spi_transact(spi_bus, spi_cs, list->tx[i], list->txlen[i], list->rx[i], list->rxlen[i], 0);
		if ( rc != 0 )
			break;
	}
	return rc;
}
#endif /* !CONFIG_SOLOS_SPI */


/* IRQ handler: just clear the interrupt and wake whoever was waiting. */
static irqreturn_t spi_isr(int irq, void *dev_id, struct pt_regs *regs)
{
	struct  spi_dev *spi = dev_id;
	u32 tmp = readl(spi->baseaddr + SOLOS_SPI_AUXCTRL);
	tmp &= ~(SOLOS_SPI_AUXCTRL_TX_IRQEN | SOLOS_SPI_AUXCTRL_RX_IRQEN | SOLOS_SPI_AUXCTRL_IDLE_IRQEN);
	writel(tmp, spi->baseaddr + SOLOS_SPI_AUXCTRL);
	complete(&spi->spi_comp);
	return IRQ_HANDLED;
}


#ifndef CONFIG_SOLOS_SPI
static int __init solos_spi_init(void)
{
	int rc = request_irq(IRQ_SPI0, spi_isr, 0, "spi0", &spi_devs[0]);
	if (rc == 0) {
		rc == request_irq(IRQ_SPI1, spi_isr, 0, "spi1", &spi_devs[1]);
	}
	spi_devs[0].baseaddr = SOLOS_LAHB_VIRT + SOLOS_SPI0_BASE;
	spi_devs[1].baseaddr = SOLOS_LAHB_VIRT + SOLOS_SPI1_BASE;
	spi_devs[0].div = SPI_DEFAULT_DIVIDER;
	spi_devs[1].div = SPI_DEFAULT_DIVIDER;
	return rc;
}

static void solos_spi_exit(void)
{
	free_irq(IRQ_SPI0, &spi_devs[0]);
	free_irq(IRQ_SPI1, &spi_devs[1]);
}

EXPORT_SYMBOL(spi_access_bus);
EXPORT_SYMBOL(spi_release_bus);
EXPORT_SYMBOL(spi_transfer);

module_init(solos_spi_init);
module_exit(solos_spi_exit);

#else

/* bulk read(507 byte chunks), assuming that the first word is m25 serial flash read command */
static int spi_big_read(int bus, int cs, const u8 *tx, int txlen, u8 *rx, int rxlen, u32 flags)
{
  int opcode, len, bits = 8 * rxlen;
  unsigned int offset;
  u32 command;
  u8 *in = rx;
  int max_rx_bits = 4056; /* 8 X (511 - 4) */

  if(txlen == 4) { /* 1 byte command + 3 byte address for serial flash */
    command = *(u32 *)tx;
    opcode = command & 0x000000ff;
    command &= 0xffffff00;
    command = swab32(command);
    offset = command;
  }
  else {
    printk(KERN_ERR "%s: Expected read command not found in tx buffer.\n", __FUNCTION__);
    return -EIO;
  }
  
  while(bits > 0) {
    if(bits >= max_rx_bits) {
      bits -= max_rx_bits;
      len = 507;
    }
    else {
      len = bits / 8;
      bits = 0;
    }
    spi_transact(bus, cs, tx, 4, in, len, flags);
    in += len;

    /* increment the offset in write command */
    offset += len;
    command = opcode;
    command = command << 24;
    command |= offset;
    command = swab32(command);
    *(u32 *)tx = command;
  }

  return 0;
}


/* bulk data transfer(32 byte chunks) */
static int spi_write_only(int bus, int cs, const u8 *outdata, int txlen, u32 flags)
{
	struct spi_dev *spi = &spi_devs[bus];
	int txpos = 0;
	u32 ctrl;

	spi_setup_hw(spi, bus, cs, flags & SPI_CLCKHI, flags & SPI_ALTPHASE);

      	ctrl = (readl(spi->baseaddr + SOLOS_SPI_CTRLSTAT) & ~(SOLOS_SPI_CS_TXLEN_MASK | SOLOS_SPI_CS_TXAUTO | SOLOS_SPI_CS_RXON)) | SOLOS_SPI_CS_TXAUTO | (8 * txlen);
	writel(ctrl, spi->baseaddr + SOLOS_SPI_CTRLSTAT);

	/* Fill the Tx FIFO */
	while( txpos < txlen ) {
	  u32 status = readl(spi->baseaddr + SOLOS_SPI_CTRLSTAT);

	  if ( status & SOLOS_SPI_CS_TXSPACE) {
	    u32 val = 0;
	    int j;

	    for (j = 3; j >= 0; j-- ) {
	      if (txpos < txlen ) {
		val |= (outdata[txpos++] << (8 * j) );
	      }
	    } 
	    writel(val,spi->baseaddr + SOLOS_SPI_TXFIFO);
	  }
	  else {
	    while(!(readl(spi->baseaddr + SOLOS_SPI_CTRLSTAT) & SOLOS_SPI_CS_TXFIFO_EMPTY));
	  }
	}
	
	while(!(readl(spi->baseaddr + SOLOS_SPI_CTRLSTAT) & SOLOS_SPI_CS_TXFIFO_EMPTY));
	return 0;
}



static int solos_spi_setup(struct spi_device *spi)
{
	int bus, cs;

	bus = spi->master->bus_num - 1;
	cs = spi->chip_select;
	spi_setup_hw(&spi_devs[bus], bus, cs, 0 & SPI_CLCKHI, 0 & SPI_ALTPHASE);
	return 0;
}


static int solos_spi_transfer(struct spi_device *spi, struct spi_message *message)
{
	int tx_len = 0, rx_len = 0, bus, cs, count = 0;
	int rc = 0, ch = 0;
	const u8 *tx = NULL;
	u8 *rx = NULL;
	u8 write[300];
	struct list_head *p, *list = &(message->transfers);
	struct spi_transfer *t;

	bus = spi->master->bus_num - 1;
	cs = spi->chip_select;

	/* we expect spi_message to contain no more than two spi_transfer requests. Moreover, each spi_transfer has one of tx_buf or rx_buf as NULL. A write sequence comes before a read sequence. The write sequence contains two transfer requests. This quick-fix is to allow m25p16 flash support only. */
	list_for_each(p, list) {
		t = list_entry(p, struct spi_transfer, transfer_list);

		if(count > 1) {
		  printk("%s: spi_message contains more than two messages.\
		  	Error! Not prepared for this!!!!\n", __FUNCTION__);
		}

		if(count) { /* second spi_transfer contains read request (when called in the context of a read) */
		  if(!t->rx_buf) { /* this is a write, copY data from current and previous transfer buffers to a common tx buffer */
		    memcpy(write, tx, tx_len);
		    memcpy(write + tx_len, t->tx_buf, t->len);
		    tx_len += t->len;
		    tx = write;
		    ch = 1;
		  } 
		  else { /* this is a read request */
		    rx_len = t->len;
		    rx = (u8 *)t->rx_buf;
		    if((rx_len + tx_len) >= 512)
		      ch = 2;
		  }
		} 
		else { /* first spi_transfer always contains a transfer request */
		  tx_len = t->len;
		  tx = (const u8 *)t->tx_buf;
		  if(!tx)
		    printk("%s: Error...spi_message contains no transfer request\
				at the head of the list!\n", __FUNCTION__);
		}
		++count;		
	}
	
	if(ch == 1)
	  rc = spi_write_only(bus, cs, tx, tx_len, 0);
	else if(ch == 2)
	  rc = spi_big_read(bus, cs, tx, tx_len, rx, rx_len, 0);
	else
	  rc = spi_transact(bus, cs, tx, tx_len, rx, rx_len, 0);
	
	if (rc) {
	  printk(KERN_ERR "%s: spi write error.\n", __FUNCTION__);
	  rc = -EIO;
	} 
	else {
	  message->actual_length = (tx_len + rx_len);
	}
	message->status = rc;
	message->complete(message->context); /* notify the SPI subsystem that the transfer is complete */
	return rc;
}

static int spi_master_init(struct device *dev)
{
	struct spi_master *master;
	int ret = 0;
	
	master = spi_alloc_master(dev, 0);
		
	if (master == NULL) {
		ret = -ENOMEM;
		printk("%s: spi_alloc_master failed for spi device: %s.\
			This can result in possible memory leak.\n",
			__FUNCTION__, dev_name[spi_devices]); /* TODO: Free memory in case of failure */
	} else {
		master->bus_num = spi_devices + 1;
		master->num_chipselect = 0; /* only one device per SPI master */
		master->setup = solos_spi_setup;
		master->transfer = solos_spi_transfer;
		ret = spi_register_master(master);
		if(ret) {
		  printk("%s: spi_register_master failed for device: %s.\n",
		  	__FUNCTION__, dev_name[spi_devices]);
		}
	}
	return ret;
}


static int solos_spi_probe(struct device *dev)
{
	int rc = 0;

#ifdef CONFIG_ARCH_SOLOSW_GALLUS
	if(spi_devices) { /* Not more than one spi block  */
#else
	if(spi_devices > 1) { /* more than 2 spi masters found, not possible...just being paranoid */
#endif
		rc = -ENODEV;
	} else {
		rc = request_irq(spi_devices ? IRQ_SPI1 : IRQ_SPI0, spi_isr,
			0, dev_name[spi_devices], &spi_devs[spi_devices]);
		if (rc) {
			printk("%s: SPI platform device IRQ request failed.\n", __FUNCTION__);
		} else {
			spi_devs[spi_devices].baseaddr = spi_devices ? SOLOS_SPI1_BASE : SOLOS_SPI0_BASE;
			spi_devs[spi_devices].baseaddr += SOLOS_LAHB_VIRT;
			spi_devs[spi_devices].div = SPI_DEFAULT_DIVIDER;
			rc = spi_master_init(dev);
			if(rc) {
			  printk("%s: %s probe failed.\n", __FUNCTION__, dev_name[spi_devices]);
			}
			++spi_devices;
		}
	}
	return rc;
}



static int __devexit solos_spi_remove(struct device *dev)
{
	struct spi_master *master;

	master = dev_get_drvdata(dev);
	free_irq(IRQ_SPI0, &spi_devs[0]);
#ifndef CONFIG_ARCH_SOLOSW_GALLUS
	free_irq(IRQ_SPI1, &spi_devs[1]);
#endif
	spi_master_put(master);
	kfree(master);
	return 0;
}


static struct device_driver solos_spi_driver = {
	.name = "spi",
	.bus = &platform_bus_type,
	.probe = solos_spi_probe,
	.remove = __devexit_p(solos_spi_remove),
};


static int __init spi_driver_init(void)
{
	int ret = 0;

	ret = driver_register(&solos_spi_driver);
	if(ret) {
		printk("%s: Registration of SPI device driver failed!\n", __FUNCTION__);
	}
	return ret;
}

static void __exit spi_driver_exit(void)
{
	free_irq(IRQ_SPI0, &spi_devs[0]);
#ifndef CONFIG_ARCH_SOLOSW_GALLUS
	free_irq(IRQ_SPI1, &spi_devs[1]);
#endif
	driver_unregister(&solos_spi_driver);
}

module_init(spi_driver_init);
module_exit(spi_driver_exit);

#ifdef CONFIG_ARCH_SOLOSW_GALLUS
MODULE_DESCRIPTION("SolosW SPI Driver");
#else
MODULE_DESCRIPTION("Solos-228 Platform SPI Driver");
#endif

#endif /* !CONFIG_SOLOS_SPI */
