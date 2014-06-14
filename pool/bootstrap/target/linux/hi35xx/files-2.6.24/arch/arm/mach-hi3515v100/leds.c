/* linux/arch/arm/mach-hi3511_v100_f01/leds.c
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

#include <linux/config.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/sysdev.h>
#include <linux/interrupt.h>

#include <asm/irq.h>
#include <asm/leds.h>
#include <asm/system.h>
#include <asm/hardware.h>
#include <asm/io.h>

#include <asm/hardware/amba.h>
#include <asm/hardware/amba_clcd.h>
#include <asm/hardware/arm_timer.h>

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
#include <linux/timer.h>

#include <asm/arch/led.h>
#include <asm/arch/early-debug.h>

volatile unsigned int led_state = 0;

static void _leds_timeout(unsigned long data);
static DEFINE_TIMER(leds_timer, _leds_timeout, 0, 0);

static void _leds_timeout(unsigned long data)
{
	if(led_state==0)
		cpu_led_wr(0);

	leds_timer.expires = jiffies + 1;
	add_timer(&leds_timer);
}

static void hisilicon_leds_event(led_event_t ledevt)
{
	switch (ledevt) {
	case led_idle_start:
		led_state = 0;
		break;

	case led_idle_end:
		led_state = 1;
		cpu_led_wr(1);
		break;

	case led_release:
		cpu_led_wr(0);
		break;

	case led_halted:
		cpu_led_wr(0);
		break;

	default:
		break;
	}
}

static int __init hisilicon_leds_init(void)
{
	leds_event = hisilicon_leds_event;

	cpu_led_init();
	cpu_led_wr(0);

	leds_timer.expires = jiffies + 1;
	add_timer(&leds_timer);

	return 0;
}

module_init(hisilicon_leds_init);


