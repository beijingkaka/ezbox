/* linux/include/asm-arm/arch-hi3515v100/hardware.h
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
#include <asm/sizes.h>
#include <asm/arch/cpu.h>
#include <asm/arch/platform.h>

#ifndef __ASM_ARCH_HARDWARE_H 
#define __ASM_ARCH_HARDWARE_H 

#ifndef CONFIG_HISILICON_OSC_CLOCK
	#define HISILICON_OSC_CLOCK	24000000
#else
	#define HISILICON_OSC_CLOCK	CONFIG_HISILICON_OSC_CLOCK
#endif

#ifdef CONFIG_HISILICON_ARMCORECLK_SCALE
	#define HISILICON_ARMCORECLK_SCALE CONFIG_HISILICON_ARMCORECLK_SCALE
#else
	#define HISILICON_ARMCORECLK_SCALE 3
#endif

#ifdef CONFIG_HISILICON_AHBCLK_DEF
#define HISILICON_AHBCLK_DEFAULT (CONFIG_HISILICON_AHBCLK_DEF)
#else
#define HISILICON_AHBCLK_DEFAULT (200*MHZ)
#endif

#define hisilicon_readl(addr) readl(addr)

#define hisilicon_writel(value,mask,addr)  do{	unsigned long t, flags;\
	                                        local_irq_save(flags);\
                                                t = readl(addr);\
	                                        t &= ~mask;\
    	                                        t |= value & mask;\
    	                                        writel(value, addr);\
                                        	local_irq_restore(flags);\
                                            }while(0)
                                            

#define __IO_ADDRESS_INRANGE(x, base, size) ( (((x)>=(base)) && ((x)<((base)+(size))))&0x01 )
#define __IO_ADDRESS_T(x, base, size, new) ( __IO_ADDRESS_INRANGE(x,base,size)*( (new) + ((x)-(base))) )

#define __IO_ADDRESS(x) ( \
		__IO_ADDRESS_T(x, 0x10000000, 0x00100000, 0x00100000) + \
		__IO_ADDRESS_T(x, 0x10100000, 0x00100000, 0x00200000) + \
		__IO_ADDRESS_T(x, 0x20000000, 0x00100000, 0x00300000) + \
		__IO_ADDRESS_T(x, 0x20100000, 0x00100000, 0x00400000) + \
		__IO_ADDRESS_T(x, 0xb0000000, 0x00100000, 0x00500000) + \
		0 )

#define IO_ADDRESS(x) ( ((__IO_ADDRESS(x)>0)&0x01)*(__IO_ADDRESS(x)+0xFE000000) )

#define BUSCLK_TO_CPUCLK(x) ((x)*CONFIG_CPUCLK_MULTI)
#define CPUCLK_TO_BUSCLK(x) ((x)/CONFIG_CPUCLK_MULTI)
/* CIK guesswork */
#define PCIBIOS_MIN_IO			0xB0010000
#define PCIBIOS_MIN_MEM			0xB0020000
#define pcibios_assign_all_busses()     1
#define SC_LOCK()	writel(0, IO_ADDRESS(REG_BASE_SCTL + REG_SC_PERLOCK))
#define SC_UNLOCK()	writel(REG_VALUE_SC_NOLOCK, IO_ADDRESS(REG_BASE_SCTL + REG_SC_PERLOCK))
#define IS_SC_LOCKED()	(readl(IO_ADDRESS(REG_BASE_SCTL + REG_SC_PERLOCK)) == REG_VALUE_SC_LOCKED)

#if (CONFIG_HISILICON_KCONSOLE==1)
	#define BOOTUP_UART_BASE REG_BASE_UART1	
#else
	#define BOOTUP_UART_BASE REG_BASE_UART0	
#endif

#endif /* __ASM_ARCH_HARDWARE_H */

