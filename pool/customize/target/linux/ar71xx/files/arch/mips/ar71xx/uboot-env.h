/*
 *  Atheros AR71xx minimal uboot-env support
 *
 *  Copyright (C) 2009 Gabor Juhos <juhosg@openwrt.org>
 *  Copyright (C) 2010 ezbox project
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 */

#ifndef _AR71XX_UBOOT_ENV_H
#define _AR71XX_UBOOT_ENV_H

char *ubootenv_find_var(const char *name, const char *buf,
		        unsigned buf_len) __init;
int ubootenv_parse_mac_addr(const char *ubootenv, unsigned ubootenv_len,
			    const char *name, char *mac) __init;

#endif /* _AR71XX_UBOOT_ENV_H */
