/*
 *  EZBOX on TL-WR740N v4 board support
 *
 *  Copyright (C) 2009-2010 Gabor Juhos <juhosg@openwrt.org>
 *  Copyright (C) 2010      Zeta Labs <zetalabs@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 */

#include <linux/gpio.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>

#include <asm/mach-ar71xx/ar71xx.h>

#include "machtype.h"
#include "devices.h"
#include "dev-m25p80.h"
#include "dev-ap91-pci.h"
#include "dev-gpio-buttons.h"
#include "dev-leds-gpio.h"
#include "dev-usb.h"
#include "nvram.h"
#include "uboot-env.h"

#define EZBOX_TL_WR740N_V4_GPIO_LED_QSS		0
#define EZBOX_TL_WR740N_V4_GPIO_LED_SYSTEM	1

#define EZBOX_TL_WR740N_V4_GPIO_BTN_RESET	11
#define EZBOX_TL_WR740N_V4_GPIO_BTN_QSS		12

#define EZBOX_TL_WR740N_V4_GPIO_USB_POWER	6

#define EZBOX_TL_WR740N_V4_KEYS_POLL_INTERVAL	20	/* msecs */
#define EZBOX_TL_WR740N_V4_KEYS_DEBOUNCE_INTERVAL (3 * EZBOX_TL_WR740N_V4_KEYS_POLL_INTERVAL)


#define EZBOX_TL_WR740N_V4_UBOOT_ENV_ADDR	0x1f040000
#define EZBOX_TL_WR740N_V4_UBOOT_ENV_SIZE	0x10000

#define EZBOX_TL_WR740N_V4_NVRAM_ADDR		0x1f3e0000
#define EZBOX_TL_WR740N_V4_NVRAM_SIZE		0x10000

#ifdef CONFIG_MTD_PARTITIONS
static struct mtd_partition ezbox_tl_wr740n_v4_partitions[] = {
	{
		.name		= "u-boot",
		.offset		= 0,
		.size		= 0x040000,
		.mask_flags	= MTD_WRITEABLE,
	} , {
		.name		= "u-boot-env",
		.offset		= 0x040000,
		.size		= 0x010000,
	} , {
		.name		= "kernel",
		.offset		= 0x050000,
		.size		= 0x200000,
	} , {
		.name		= "rootfs",
		.offset		= 0x250000,
		.size		= 0x190000,
	} , {
		.name		= "nvram",
		.offset		= 0x3e0000,
		.size		= 0x010000,
		.mask_flags	= MTD_WRITEABLE,
	} , {
		.name		= "art",
		.offset		= 0x3f0000,
		.size		= 0x010000,
		.mask_flags	= MTD_WRITEABLE,
	} , {
		.name		= "firmware",
		.offset		= 0x050000,
		.size		= 0x390000,
		.mask_flags	= MTD_WRITEABLE,
	}
};
#endif /* CONFIG_MTD_PARTITIONS */

static struct flash_platform_data ezbox_tl_wr740n_v4_flash_data = {
#ifdef CONFIG_MTD_PARTITIONS
        .parts          = ezbox_tl_wr740n_v4_partitions,
        .nr_parts       = ARRAY_SIZE(ezbox_tl_wr740n_v4_partitions),
#endif
};

static struct gpio_led ezbox_tl_wr740n_v4_leds_gpio[] __initdata = {
	{
		.name		= "ezbox-tl-wr740n-v4:green:system",
		.gpio		= EZBOX_TL_WR740N_V4_GPIO_LED_SYSTEM,
		.active_low	= 1,
	}, {
		.name		= "ezbox-tl-wr740n-v4:green:qss",
		.gpio		= EZBOX_TL_WR740N_V4_GPIO_LED_QSS,
		.active_low	= 1,
	}
};

static struct gpio_keys_button ezbox_tl_wr740n_v4_gpio_keys[] __initdata = {
	{
		.desc		= "reset",
		.type		= EV_KEY,
		.code		= KEY_RESTART,
		.debounce_interval = EZBOX_TL_WR740N_V4_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= EZBOX_TL_WR740N_V4_GPIO_BTN_RESET,
		.active_low	= 1,
	}, {
		.desc		= "qss",
		.type		= EV_KEY,
		.code		= KEY_WPS_BUTTON,
		.debounce_interval = EZBOX_TL_WR740N_V4_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= EZBOX_TL_WR740N_V4_GPIO_BTN_QSS,
		.active_low	= 1,
	}
};

static void __init ezbox_tl_wr740n_v4_setup(void)
{
	const char *nvram = (char *) KSEG1ADDR(EZBOX_TL_WR740N_V4_NVRAM_ADDR);
	const char *ubootenv = (char *) KSEG1ADDR(EZBOX_TL_WR740N_V4_UBOOT_ENV_ADDR);
	u8 mac[6];
	u8 *ee = (u8 *) KSEG1ADDR(0x1fff1000);

	/* enable power for the USB port */
	gpio_request(EZBOX_TL_WR740N_V4_GPIO_USB_POWER, "USB power");
	gpio_direction_output(EZBOX_TL_WR740N_V4_GPIO_USB_POWER, 1);

	ar71xx_add_device_m25p80(&ezbox_tl_wr740n_v4_flash_data);

	ar71xx_add_device_leds_gpio(-1, ARRAY_SIZE(ezbox_tl_wr740n_v4_leds_gpio),
					ezbox_tl_wr740n_v4_leds_gpio);

	ar71xx_register_gpio_keys_polled(-1, EZBOX_TL_WR740N_V4_KEYS_POLL_INTERVAL,
					ARRAY_SIZE(ezbox_tl_wr740n_v4_gpio_keys),
					ezbox_tl_wr740n_v4_gpio_keys);

	ar71xx_eth1_data.has_ar7240_switch = 1;
	if (nvram_parse_mac_addr(nvram, EZBOX_TL_WR740N_V4_NVRAM_SIZE,
	                         "lan_hwaddr=", mac) == 0) {
		ar71xx_init_mac(ar71xx_eth0_data.mac_addr, mac, 0);
		ar71xx_init_mac(ar71xx_eth1_data.mac_addr, mac, 1);
	}
	else if (ubootenv_parse_mac_addr(ubootenv, EZBOX_TL_WR740N_V4_UBOOT_ENV_SIZE,
	                         "ethaddr=", mac) == 0) {
		ar71xx_init_mac(ar71xx_eth0_data.mac_addr, mac, 0);
		ar71xx_init_mac(ar71xx_eth1_data.mac_addr, mac, 1);
	}
	else {
		memcpy(mac, (u8 *) KSEG1ADDR(0x1f01fc00), sizeof(mac));
		ar71xx_init_mac(ar71xx_eth0_data.mac_addr, mac, 0);
		ar71xx_init_mac(ar71xx_eth1_data.mac_addr, mac, 1);
	}

	/* WAN port */
	ar71xx_eth0_data.phy_if_mode = PHY_INTERFACE_MODE_RMII;
	ar71xx_eth0_data.speed = SPEED_100;
	ar71xx_eth0_data.duplex = DUPLEX_FULL;

	/* LAN ports */
	ar71xx_eth1_data.phy_if_mode = PHY_INTERFACE_MODE_RMII;
	ar71xx_eth1_data.speed = SPEED_1000;
	ar71xx_eth1_data.duplex = DUPLEX_FULL;

	ar71xx_add_device_mdio(0x0);
	ar71xx_add_device_eth(1);
	ar71xx_add_device_eth(0);

	ar71xx_add_device_usb();

	ap91_pci_setup_wmac_led_pin(1);

	ap91_pci_init(ee, mac);
}

MIPS_MACHINE(AR71XX_MACH_EZBOX_TL_WR740N_V4, "EZBOX-TL-WR740N-v4", "EZBOX on TL-WR740N v4",
	     ezbox_tl_wr740n_v4_setup);
