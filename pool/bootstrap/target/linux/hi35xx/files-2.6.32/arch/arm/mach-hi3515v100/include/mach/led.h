/* linux/include/asm-arm/arch-hi3515v100/led.h
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
#include <mach/gpio.h>

#ifndef __ASM_ARCH_LED_H
#define __ASM_ARCH_LED_H

#ifdef CONFIG_LEDS_CPU

#define CPULED_GPIO_GRP ((CONFIG_HISILICON_GPIO_CPU_LED%100)/10)
#define CPULED_GPIO_BIT (CONFIG_HISILICON_GPIO_CPU_LED%10)
#define CPULED_INVEST	(CONFIG_HISILICON_GPIO_CPU_LED/100)
#define CPULED_VAL(x)	(CPULED_INVEST==0 ? !(x):(x))

#define cpu_led_init() HISILICON_GPIO_SET_BITDIR(1,CPULED_GPIO_GRP,CPULED_GPIO_BIT)
#define cpu_led_wr(x) HISILICON_GPIO_WRBIT(CPULED_VAL(x),CPULED_GPIO_GRP,CPULED_GPIO_BIT)
#define cpu_led_rd() CPULED_VAL(HISILICON_GPIO_RDBIT(CPULED_GPIO_GRP,CPULED_GPIO_BIT))

#else

#define cpu_led_init() 
#define cpu_led_wr(x) 
#define cpu_led_rd() 

#endif /* CONFIG_LEDS_CPU */

#endif /* __ASM_ARCH_LED_H */

