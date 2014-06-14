/* linux/include/asm-arm/arch-hi3515v100/uncompress.h
*
* Copyright (c) 2009 Hisilicon Co., Ltd. 
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

#include <mach/system.h>


#define AMBA_UART_DR	(*(volatile unsigned char *)(BOOTUP_UART_BASE + 0x000))
#define AMBA_UART_LCRH	(*(volatile unsigned char *)(BOOTUP_UART_BASE + 0x02C))
#define AMBA_UART_CR	(*(volatile unsigned char *)(BOOTUP_UART_BASE + 0x030))
#define AMBA_UART_FR	(*(volatile unsigned char *)(BOOTUP_UART_BASE + 0x018))
/*
 * This does not append a newline
 */
#if 0
static void putc(const char *s)
{
	if (*s) {
		while (AMBA_UART_FR & (1 << 5))
			barrier();
		AMBA_UART_DR = *s;

		if (*s == '\n') {
			while (AMBA_UART_FR & (1 << 5))
				barrier();
			AMBA_UART_DR = '\r';
		}
	}
	while (AMBA_UART_FR & (1 << 3))
		barrier();
}
#endif
static void putstr(const char *s)
{
        while (*s) {
                while (AMBA_UART_FR & (1 << 5))
                        barrier();

                AMBA_UART_DR = *s;

                if (*s == '\n') {
                        while (AMBA_UART_FR & (1 << 5))
                                barrier();

                        AMBA_UART_DR = '\r';
                }
                s++;
        }
        while (AMBA_UART_FR & (1 << 3))
                barrier();
}

/*
 * nothing to do
 */
#define arch_decomp_setup()
#define arch_decomp_wdog()

