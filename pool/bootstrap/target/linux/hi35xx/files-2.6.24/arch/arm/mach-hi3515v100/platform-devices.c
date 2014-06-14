/* linux/arch/arm/mach-hi3515_v100/platform-devices.c
*
* Copyright (c) 2006 Hisilicon Co., Ltd. 
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
*
*/
#include <linux/autoconf.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/sysdev.h>
#include <linux/interrupt.h>

#include <asm/irq.h>
#include <asm/leds.h>
#include <asm/system.h>
#include <asm/hardware.h>
#include <asm/io.h>
#include <linux/amba/bus.h>
#include <linux/amba/clcd.h>
#include <asm/hardware/arm_timer.h>
#include <linux/platform_device.h>
#include <asm/mach/irq.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/mach/time.h>
#include <asm/mach/flash.h>
#include <asm/mach/map.h>

#include <asm/arch/platform.h>
#include <asm/arch/irqs.h>
#include <asm/arch/io.h>
#include <asm/arch/timer.h>

#include <linux/delay.h>

#include <asm/arch/led.h>
#include <asm/arch/early-debug.h>
#include <linux/mtd/plat-ram.h>
#include <linux/mtd/partitions.h>

#if 0
static struct resource hisilicon_usbhost_resources[] = {
	[0] = {
		.start = REG_BASE_USB11,
		.end   = (REG_BASE_USB11+ 0xffff),
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = INTNR_USB11,
		.end   = INTNR_USB11,
		.flags = IORESOURCE_IRQ,
	},
};

static u64 hisilicon_usbhost_dma_mask = 0xffffffffUL;

static struct platform_device hisilicon_usbhost_device = {
	        .name = "hisilicon-ohci",
		.id   = 0,
		.dev  = {
			.dma_mask = &hisilicon_usbhost_dma_mask,
			.coherent_dma_mask = 0xffffffffUL,
		},
		.num_resources = ARRAY_SIZE(hisilicon_usbhost_resources),
		.resource = hisilicon_usbhost_resources,
};


	
static struct resource hisilicon_eth_resources[] = {
	[0] = {
		.start = REG_BASE_GMAC,
		.end   = REG_BASE_GMAC + REG_GMAC_IOSIZE,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = INTNR_GMAC,
		.end   = INTNR_GMAC,
		.flags = IORESOURCE_IRQ,
	},
};

static struct resource hisilicon_ethv100_resources[] = {
	[0] = {
		.start = REG_BASE_GMAC,
		.end   = REG_BASE_GMAC + REG_GMAC_IOSIZE,
		.flags = IORESOURCE_MEM,
		.parent = &hisilicon_eth_resources[0],
	},

	[1] = {
		.start = INTNR_GMAC,
		.end   = INTNR_GMAC,
		.flags = IORESOURCE_IRQ,
	},
};

static struct resource hisilicon_ethv100_mdio_resources[] = {
	[0] = {
		.start = REG_BASE_GMAC,
		.end   = REG_BASE_GMAC + REG_GMAC_IOSIZE,
		.flags = IORESOURCE_MEM,
		.parent = &hisilicon_ethv100_resources[0],
	},
};

static struct platform_device hisilicon_ethv100_device = {
	        .name = "hiethv100",
		.id   = 0,
		.num_resources = ARRAY_SIZE(hisilicon_ethv100_resources),
		.resource = hisilicon_ethv100_resources,
};

static struct platform_device hisilicon_mdiobus_device = {
	        .name = "hiethv100_mdiobus",
		.id   = 0,
		.num_resources = ARRAY_SIZE(hisilicon_ethv100_mdio_resources),
		.resource = hisilicon_ethv100_mdio_resources,
};

static struct platform_device hisilicon_eth_device = {
	        .name = "Hi3511-ETH",
		.id   = 0,
		.num_resources = ARRAY_SIZE(hisilicon_eth_resources),
		.resource = hisilicon_eth_resources,
};
#endif
static struct resource hisilicon_sfv300_resources[] = {
	[0] = {
		.start = 0x10090000,/*hi3515v100 sfv300*/
		.end   = 0x10090000 + 0xffff,
		.flags = IORESOURCE_MEM,
	},

	[1] = {
		.start = 14,/*hi3515v100 sfv300*/
		.end   = 14,
		.flags = IORESOURCE_IRQ,
	},
};

static struct platform_device hisilicon_sfv300_device = {
	.name = "hisfv300",
	.id   = 0,
	.num_resources = ARRAY_SIZE(hisilicon_sfv300_resources),
	.resource = hisilicon_sfv300_resources,
};

static struct resource hisilicon_sfv300_mdio_resources[] = {
	[0] = {
		.start = 0x10090000,/*hi3515v100 sfv300*/
		.end   = 0x10090000 + 0xffff,
		.flags = IORESOURCE_MEM,
		.parent = &hisilicon_sfv300_resources[0],
	},
};

static struct platform_device hisilicon_mdiobus_device = {
	.name = "hisfv300_mdiobus",
	.id   = 0,
	.num_resources = ARRAY_SIZE(hisilicon_sfv300_mdio_resources),
	.resource = hisilicon_sfv300_mdio_resources,
};

static struct platform_device *hisilicon_plat_devs[] __initdata = {
	&hisilicon_sfv300_device,
	&hisilicon_mdiobus_device,
};


int __init hisilicon_register_platform_devices(void)
{
	platform_add_devices(hisilicon_plat_devs, ARRAY_SIZE (hisilicon_plat_devs));
	
	return 0;
}

