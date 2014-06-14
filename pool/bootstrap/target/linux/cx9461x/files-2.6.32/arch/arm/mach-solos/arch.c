/*
 * linux/arch/arm/mach-solos/arch.c
 *
 * Author: Guido Barzini
 * Copyright (C) 2004 Conexant inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#include <linux/config.h>
#include <linux/init.h>
#include <linux/timex.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/mtd/partitions.h>
#include <mtd/mtd-abi.h>
#include <asm/io.h>
#include <asm/hardware.h>
#include <asm/irq.h>
#include <asm/mach/map.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/mach/time.h>
#include <asm/mach/irq.h>
#include "board.h"


#ifdef CONFIG_SOLOS_SPI

#include <linux/device.h>
#include <linux/platform.h>
#include <linux/spi/spi.h>
#include <linux/spi/flash.h>

static struct platform_device solos_spi_device0 = {
	.name           = "spi", /* the same as the master controller driver name */
	.id             = 0,
};

static struct platform_device solos_spi_device1 = {
	.name           = "spi", /* the same as the master controller driver name */
	.id             = 1,
};


static struct platform_device *spi_devices[] __initdata = {
	&solos_spi_device0,
	&solos_spi_device1,
};

static struct mtd_partition spi_flash_partitions[] = {
	{
		.name = "bootloader",
		.size = 0x00020000,
		.offset =  0,
		.mask_flags =  MTD_CAP_ROM
	},{
		.name = "kernel",
		.size = 0xA0000,
		.offset = 0x20000
	},{
		.name = "jffs2",
		//.size = 0x140000,
		.size = 0x340000,
		.offset = 0x000C0000,
	}
};

static struct flash_platform_data spi_flash_info = {
	//.name           = "m25p16",
	//.type           = "m25p16", /* for the serial flash driver */
	.name           = "m25p32",
	.type           = "m25p32", /* for the serial flash driver */
	.parts			= spi_flash_partitions,
	.nr_parts       = ARRAY_SIZE(spi_flash_partitions), /* number of static partitons */
};

static struct spi_board_info spi_board_info[] __initdata = {
  {
	  .modalias       = "m25p80", /* name of the slave/flash driver to be used for SPI<bus_num> */
	  .platform_data  = &spi_flash_info,
	  .bus_num        = 1,
	  .chip_select    = 0,
  },
};

#endif /* CONFIG_SOLOS_SPI */


static const char *solos4510 = "Solos CX4610";
static const char *solos4515 = "Solos CX4615";
static char solos_chip_name[32] = "Solos CX46xx, unknown variant";


//#define NASTY_PROC_HACK
#ifdef NASTY_PROC_HACK
#include <linux/ctype.h>
#include <linux/proc_fs.h>
static struct proc_dir_entry *dbg_file;

static const char *parsearg(const char *buffer, u32 *val)
{
	char *tmp = (char *)buffer;
	while( isspace(*buffer)) buffer++;
	*val = simple_strtoul(buffer, &tmp, 0);
	return tmp;
}
 
static  int dbg_proc_write(struct file *file, const char *buffer,
                          unsigned long count, void *data)
{
        const char *end = buffer + count;
	u32 addr, n;
	int i;
        while( buffer < end )
        { 
            switch( tolower(*buffer++) ) 
            {
            case 'r': /* Read */
                buffer = parsearg(buffer, &addr);
		printk("%x : %x\n", addr, *((u32*)addr));
                break;
            case 'w': /* Write */
                buffer = parsearg(buffer, &addr);
                buffer = parsearg(buffer, &n);
		printk("Write %x to %x\n", n, addr );
		*((u32*)addr) = n;
                break;
            case 'd': /* Dump */
                buffer = parsearg(buffer, &addr);
                buffer = parsearg(buffer, &n);
		printk("Dump %x words at %x\n", n, addr );
		for (i = 0; i < n; i++ ) {
			printk("%x : %x\n", addr, *((u32*)addr) );
			addr += 4;
		}
                break;
            default:  /* Give up */
                buffer = end;
                break;
            }
        }
        return count;
}


static int __init create_dbg_procfile(void)
{
    dbg_file = create_proc_entry("debug",  S_IFREG | S_IRUSR, &proc_root );
    if ( dbg_file != NULL )
    {
        dbg_file->read_proc =  NULL;
        dbg_file->write_proc =  dbg_proc_write;        
    }
    else
    {
        printk("Failed to create /proc/debug\n"); 
    }
    return 0;
}

late_initcall(create_dbg_procfile);

#endif

static struct map_desc solos_io_desc[] __initdata = {
        /* Boot ROM */
  {SOLOS_ROM_VIR,  SOLOS_ROM_PHYS, SOLOS_ROM_SIZE, MT_DEVICE },
  /*
   * Added mappings for uncached SDRAM memory & EPB mapped memory
   */
  /* Uncached Mapping */
  {SOLOS_SDRAMU_VIRT,  SOLOS_SDRAMU_PHYS, SOLOS_SDRAMU_MAP_SIZE, MT_DEVICE},
  /* EPBMEM Mapping */
  {SOLOS_EPBMEM_VIRT, SOLOS_EPBMEM_PHYS, SOLOS_EPBMEM_MAP_SIZE, MT_DEVICE},
  /* The core CHARM peripherals */
  {CHM_BASE_VIRT, CHM_BASE_PHYS, CHM_MAP_SIZE, MT_DEVICE},
  /* The upper AHB */
  {SOLOS_UAHB_VIRT, SOLOS_UAHB_PHYS, SOLOS_UAHB_MAP_SIZE, MT_DEVICE},
  /* The lower AHB */
  {SOLOS_LAHB_VIRT, SOLOS_LAHB_PHYS, SOLOS_LAHB_MAP_SIZE, MT_DEVICE},        
  /* The On-chip SRAM */
  {SOLOS_SRAM_VIRT, SOLOS_SRAM_PHYS, SOLOS_SRAM_MAP_SIZE, MT_DEVICE}
};

/* Create the static I/O mappings */
static void __init solos_map_io(void)
{
	iotable_init(solos_io_desc, ARRAY_SIZE(solos_io_desc));
        /* Right, the I/O world is mapped in, now do any board-specific start-of-day */
    solos_board_init();
	switch(readl(CHM_BASE_VIRT) >> CHM_VERSION_IDPINSSHIFT ) /* Read the version register */
	{
		case 0:
			strcpy(solos_chip_name, solos4515);
			break;
		case 1:
			strcpy(solos_chip_name, solos4510);
			break;
	}
}

/* LATCH is the system-provided conversion from h/w ticks to jiffies */
static unsigned long solos_gettimeoffset(void)
{
        unsigned long ticks = LATCH - CHMIO_VIRT(TIMER2_VALUE); /* Elapsed time in H/W ticks */
	/* Convert to usecs */
        return (ticks * (1000000/LATCH))/HZ; 
}


static irqreturn_t solos_timer_interrupt(int irq, void *dev_id,
					 struct pt_regs *regs)
{
	CHMIO_VIRT(TIMER2_CLEAR) = irq;
	timer_tick(regs);
	return IRQ_HANDLED;
}

static struct irqaction solos_timer2_irq = {
	.name = "System Timer",
	.flags = SA_INTERRUPT,
	.handler = solos_timer_interrupt
};

/* Set up timer 2 to generate system timer interrupt */
static void __init solos_init_time(void)
{
	setup_irq(IRQ_TIMER2, &solos_timer2_irq);
	CHMIO_VIRT(TIMER2_RELOAD) = LATCH;
	/* Enable interrupts, set reload mode, clear overrun */
	CHMIO_VIRT(TIMER2_CTRL) =
	    CHM_TIMER_CTRL_INTENB | CHM_TIMER_CTRL_RELOAD |
	    CHM_TIMER_CTRL_OVERRUN;
	/* Start countdown. */
	CHMIO_VIRT(TIMER2_VALUE) = LATCH;
}

struct sys_timer solos_timer = {
	.init           = solos_init_time,
	.offset         = solos_gettimeoffset,
};

static void mask_vic_irq(unsigned int irq)
{
	CHMIO_VIRT(VIC_IRQ_DIS) = (1 << irq);
}

static void unmask_vic_irq(unsigned int irq)
{
	CHMIO_VIRT(VIC_IRQ_ENA) = (1 << irq);
}

static struct irqchip solos_vic_irq = {
	.ack = mask_vic_irq,
	.mask = mask_vic_irq,
	.unmask = unmask_vic_irq,
	.retrigger = NULL,
	.type = NULL
};

/* Set up all the other IRQs */
static void __init solos_init_irq(void)
{
	int i;
	for (i = 0; i < NR_IRQS; i++) {
		set_irq_chip(i, &solos_vic_irq);
		set_irq_handler(i, do_level_IRQ);
		set_irq_flags(i, IRQF_VALID | IRQF_PROBE);
	}
	CHMIO_VIRT(VIC_IRQ_DIS) = ~(-1);
	/* Now program up the IRQ controller. */
	CHMIO_VIRT(VIC_IRQ_RR) |= CHM_VIC_RR_AUTOADV;
}

/* Utility code: simple allocator for the on-chip SRAM. This is 
 * bogglingly crude.  */
static u8 *free_sram = (u8 *) SOLOS_SRAM_VIRT;


void *solos_sram_alloc(int size)
{
	void *tmp;
	size = (size + 3) & (~3);	/* Word align the end. */
	if ((free_sram + size) - (u8 *) SOLOS_SRAM_VIRT > SOLOS_SRAM_SIZE)
		return NULL;
	tmp = free_sram;
	free_sram += size;
	return tmp;
}

EXPORT_SYMBOL(solos_sram_alloc);

int quantum_sched_setscheduler(struct task_struct *p, int policy, struct
sched_param *param)
{
    return sched_setscheduler( p, policy, param);
}
EXPORT_SYMBOL(quantum_sched_setscheduler);
#if 1
int as_printk(const char *fmt, ...)
{
    va_list args;
    int r;
 
    va_start(args, fmt);
    r = vprintk(fmt, args);
    va_end(args);
 
    return r;
}
EXPORT_SYMBOL(as_printk);

asmlinkage  int as_vsnprintf(char *s,int size, const char *format, va_list *arg);
asmlinkage  int as_vsnprintf(char *s,int size, const char *format, va_list *arg)
{
	int r;
	r = vsnprintf(s,size,format,arg);
	return r;	
}
EXPORT_SYMBOL(as_vsnprintf);

asmlinkage int as_vprintk(const char *format, va_list *args);
asmlinkage int as_vprintk(const char *format, va_list *args)
{
	int r;
	r = vprintk(format,args);
	return r;	
}
EXPORT_SYMBOL(as_vprintk);

int as_snprintf(char * buf, int size, const char *fmt, ...)
{
    va_list args;
    int i;
 
    va_start(args, fmt);
    i=vsnprintf(buf,size,fmt,args);
    va_end(args);
    return i;
}
 
EXPORT_SYMBOL(as_snprintf);

int as_sprintf(char * buf, const char *fmt, ...)
{
    va_list args;
    int i;
 
    va_start(args, fmt);
    i=vsprintf(buf,fmt,args);
    va_end(args);
    return i;
}
 
EXPORT_SYMBOL(as_sprintf);

int as_sscanf(const char *buf, const char *fmt, ...)
{
	va_list args;
    int i;
 
    va_start(args, fmt);
    i=vsscanf(buf,fmt,args);
    va_end(args);
    return i;
}

EXPORT_SYMBOL(as_sscanf);

#endif

#ifdef CONFIG_SOLOS_SPI
static int __init solos_init_machine(void)
{
	platform_add_devices(spi_devices, ARRAY_SIZE(spi_devices));
	return spi_register_board_info(spi_board_info, ARRAY_SIZE(spi_board_info));
}
#endif /* CONFIG_SOLOS_SPI */

void __init init_dma(void)
{
        printk(KERN_ERR"%s not supported \n",__FUNCTION__);
}

MACHINE_START(SOLOS, solos_chip_name)
    MAINTAINER("Conexant inc.")
    MAPIO(solos_map_io)
    INITIRQ(solos_init_irq)
    .timer = &solos_timer,
    BOOT_MEM(0, CHM_BASE_PHYS, CHM_BASE_VIRT)
    BOOT_PARAMS(0x80)
#ifdef CONFIG_SOLOS_SPI
    INIT_MACHINE(solos_init_machine)
#endif /* CONFIG_SOLOS_SPI */
MACHINE_END

