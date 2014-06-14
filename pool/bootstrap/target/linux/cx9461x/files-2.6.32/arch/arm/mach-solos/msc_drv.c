/*
 * linux/arch/arm/mach-solos/msc_drv.c
 *
 * Author: Guido Barzini
 * Copyright (C) 2005 Conexant inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

/* Support code for the MSC16 controllers */

#include <linux/device.h>
#include <linux/firmware.h>
#include <linux/a.out.h>
#include <asm/io.h>
#include <asm/arch/hardware/msc16.h>
#include <asm/arch/msc_drv.h>

#define NUM_MSCS (3)

struct msc_priv {
	const char *name;;
	struct device *dev;
	const struct firmware *fw;
	u32 base_addr;
};

struct device msc_devs[NUM_MSCS];

struct msc_priv mscs[NUM_MSCS] = {
	{
	 .name = "dsl-hw.msc",
	 .dev = &msc_devs[0],
	 .base_addr = SOLOS_DSL_MSC16},
	{
	 .name = "nxm-hw.msc",
	 .dev = &msc_devs[1],
	 .base_addr = SOLOS_NXM_MSC16},
	{
	 .name = "crypto-hw.msc",
	 .dev = &msc_devs[2],
	 .base_addr = SOLOS_CRYPT_MSC16}
};

void msc16_start(enum msc16_id id, u16 pc)
{
	u32 base_addr = mscs[id].base_addr;
	writel(pc | MSC16_ADDR_MODE_PC, base_addr + MSC16_ADDR_REG);
	writel(MSC16_CTRL_PCLD, base_addr + MSC16_CTRL_REG);
	writel(MSC16_CTRL_START, base_addr + MSC16_CTRL_REG);
}

void msc16_stop(enum msc16_id id)
{
	writel(MSC16_CTRL_STOP, mscs[id].base_addr + MSC16_CTRL_REG);
}

static void msc16_load_ram(u32 base, u32 len, u8 * data, u32 mode)
{
	u16 *ptr = (__u16 *) data;
	u32 i;
	len = len / 2;
	/* Start loading from 0 */
	writel(mode | MSC16_ADDR_WRITE | MSC16_ADDR_AUTOINC,
	       base + MSC16_ADDR_REG);
	for (i = 0; i < len; i++) {
		writel((__u32) * ptr, base + MSC16_WRITE_REG);
		ptr++;
	}
}

static void msc16_init_bss(u32 base, u32 len, u32 offset)
{
	u32 i;
	len = len / 2;
	/* Start loading from offset */
	writel(offset | MSC16_ADDR_MODE_DMEM | MSC16_ADDR_WRITE |
	       MSC16_ADDR_AUTOINC, base + MSC16_ADDR_REG);
	for (i = 0; i < len; i++)
		writel(0, base + MSC16_WRITE_REG);
}

static u16 msc16_read_imem(u32 msc16, u16 addr)
{
	writel(addr | MSC16_ADDR_MODE_IMEM | MSC16_ADDR_READ,
	       msc16 + MSC16_ADDR_REG);
	return (readl(msc16 + MSC16_READ_REG) & 0xffff);
}

static u16 msc16_read_dmem(u32 msc16, u16 addr)
{
	writel(addr | MSC16_ADDR_MODE_DMEM | MSC16_ADDR_READ,
	       msc16 + MSC16_ADDR_REG);
	return (readl(msc16 + MSC16_READ_REG) & 0xffff);
}

static u16 msc16_read_reg(u32 msc16, u16 reg)
{
	writel(reg | MSC16_ADDR_MODE_REG | MSC16_ADDR_READ,
	       msc16 + MSC16_ADDR_REG);
	return (readl(msc16 + MSC16_READ_REG) & 0xffff);
}

static void msc16_write_imem(u32 msc16, u16 addr, u16 val)
{
	writel(addr | MSC16_ADDR_MODE_IMEM | MSC16_ADDR_WRITE,
	       msc16 + MSC16_ADDR_REG);
	writel(val, msc16 + MSC16_WRITE_REG);
}

void msc16_write_periph(enum msc16_id id, u16 addr, u16 val)
{
	u32 msc16 = mscs[id].base_addr;
	writel(addr | MSC16_ADDR_MODE_PERIPH | MSC16_ADDR_WRITE,
	       msc16 + MSC16_ADDR_REG);
	writel(val, msc16 + MSC16_WRITE_REG);
}

void msc16_write_data(enum msc16_id id, u16 addr, u16 val)
{
	u32 msc16 = mscs[id].base_addr;
	writel(addr | MSC16_ADDR_MODE_DMEM | MSC16_ADDR_WRITE,
	       msc16 + MSC16_ADDR_REG);
	writel(val, msc16 + MSC16_WRITE_REG);
}

void msc16_write_instr(enum msc16_id id, u16 addr, u16 val)
{
	msc16_write_imem(mscs[id].base_addr, addr, val);
}

u16 msc16_read_data(enum msc16_id id, u16 addr)
{
	return msc16_read_dmem(mscs[id].base_addr, addr);
}

u16 msc16_read_instr(enum msc16_id id, u16 addr)
{
	return msc16_read_instr(mscs[id].base_addr, addr);
}

/* Request a firmware image in OMAGIC format from userland and load it onto
 * the MSC16 */
int msc16_load_fw(enum msc16_id id)
{
	int rc = 0;
	if (rc < 0 || rc >= NUM_MSCS)
		return -EINVAL;

	strcpy(msc_devs[id].bus_id, mscs[id].name);
	kobject_set_name(&msc_devs[id].kobj, "msc16");
	kobject_init(&msc_devs[id].kobj);
	rc = request_firmware(&mscs[id].fw, mscs[id].name, mscs[id].dev);

	if (rc == 0) {
		struct exec *image = (struct exec *)mscs[id].fw->data;
		if (N_MAGIC(*image) != OMAGIC) {
			printk("firmware image invalid.\n");
			goto bad_image;
		}
		msc16_load_ram(mscs[id].base_addr, image->a_text,
			       (__u8 *) image + N_TXTOFF(*image),
			       MSC16_ADDR_MODE_IMEM);
		msc16_load_ram(mscs[id].base_addr, image->a_data,
			       (__u8 *) image + N_DATOFF(*image),
			       MSC16_ADDR_MODE_DMEM);
		msc16_init_bss(mscs[id].base_addr, image->a_bss,
			       N_TXTOFF(*image) + image->a_text +
			       image->a_data);
	} else {
		printk("request_firmware returned %d\n", rc);
	}

      bad_image:
	release_firmware(mscs[id].fw);
	return rc;
}

/* Sequence for setting r0 and r1 then halting the MSC16 */
static u16 regset_seq[] = { 0xff00, 0xffff, 0xff01, 0xffff, 0xf1ff };	/* ldi r0, #M; ldi r1, #N; halt */
static int save_len = sizeof(regset_seq) / sizeof(u16);

/* Run the msc16, with r0 and r1 as specified. Because the register file
 * is read-only on the current MSC16 we write in a short stub to load the
 * registers, run that, then replace the original code. Rather than
 * trying to be clever and work out where the end of the image is and
 * where the ISRAM ends on that particular MSC16 we just use address 0
 * and save/restore the code. */
void msc16_run_fw(enum msc16_id id, u16 arg0, u16 arg1)
{
	u32 msc16 = mscs[id].base_addr;
	u16 save_buf[save_len];
	int i;
	msc16_stop(id);
	regset_seq[1] = arg0;
	regset_seq[3] = arg1;

	for (i = 0; i < save_len; i++) {
		save_buf[i] = msc16_read_imem(msc16, i);
		msc16_write_imem(msc16, i, regset_seq[i]);
	}
	/* Run the msc16 to load r0 and r1 */
	msc16_start(id, 0);
	/* Wait for MSC16 to halt. We only need it to execute 5 instructions. */
	while ((readl(msc16 + MSC16_CTRL_REG) & MSC16_CTRL_HALT) == 0)	/* do nothing */
		;

	msc16_stop(id);
	/* Replace the original code */
	for (i = 0; i < save_len; i++) {
		msc16_write_imem(msc16, i, save_buf[i]);
	}
	if ((msc16_read_reg(msc16, 0) != arg0)
	    || (msc16_read_reg(msc16, 1) != arg1))
		printk(KERN_ERR "Read back incorrect values in r0 and r1!\n");
	/* And run the original code with r0 and r1 set to arg0 and arg1 */
	msc16_start(id, 0);
	writel(MSC16_CTRL_STAT0, msc16 + MSC16_CTRL_REG);
}

/* Handshake with the msc16: clear STAT0, set ATT0, wait for STAT0 to 
 * come back on. */

void msc16_handshake(enum msc16_id id)
{
	u32 msc16 = mscs[id].base_addr;
	int i = 0;
	writel(MSC16_CTRL_ATT0, msc16 + MSC16_CTRL_REG);
	while ((readl(msc16 + MSC16_CTRL_REG) & MSC16_CTRL_STAT0) == 0) {
		/* Just spin for now. Consider (a) and interrupt, and (b) timing out */
		i++;
	}
	/* Clear STAT0 for next time */
	writel(MSC16_CTRL_STAT0, msc16 + MSC16_CTRL_REG);
}

EXPORT_SYMBOL(msc16_load_fw);
EXPORT_SYMBOL(msc16_run_fw);
EXPORT_SYMBOL(msc16_start);
EXPORT_SYMBOL(msc16_stop);
EXPORT_SYMBOL(msc16_handshake);
