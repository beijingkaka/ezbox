/* linux/arch/arm/mach-hi3515_v100/systimer.c
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
#include <mach/hardware.h>
#include <asm/io.h>

#include <linux/amba/bus.h>
#include <linux/amba/clcd.h>
#include <asm/hardware/arm_timer.h>
#include <linux/clk.h>

#include <asm/mach/irq.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/mach/time.h>
#include <asm/mach/flash.h>
#include <asm/mach/map.h>

#include <mach/platform.h>
#include <mach/irqs.h>
#include <mach/io.h>
#include <mach/timer.h>
#include <mach/clock.h>

#include <linux/delay.h>

#include <mach/led.h>
#include <mach/early-debug.h>

extern unsigned long hisilicon_timer_reload;

static int sys_timer_notifier(struct clk *clk, 
		unsigned int cmd, unsigned long wparam, unsigned long lparam)
{
	int ret = 0;
	struct clk *p;

	switch(cmd)
	{
	case HISILICON_CLK_REGISTER:
	case HISILICON_CLK_PARENT_SETRATE:
		p = clk_get_parent(clk);
		if(p==NULL)
			break;

		hisilicon_timer_reload = BUSCLK_TO_TIMER_RELOAD(clk_get_rate(p));
		writel(hisilicon_timer_reload, CFG_TIMER_VABASE + REG_TIMER_RELOAD);
		clk->rate = clk_get_rate(p)/CFG_TIMER_PRESCALE;
		break;

	default:
		ret =  hisilicon_clk_default_notifier(clk, cmd, wparam, lparam);
		break;
	}

	return ret;
}


static struct clk timer1_clk = {
	.name		= "SYSTIMER",
	.notifier	= sys_timer_notifier,
	.parent_name = "BUSCLK",
};

static int __init __system_timer_init(void)
{
	clk_register(&timer1_clk);

	edb_trace(1);

	return 0;
}

module_init(__system_timer_init);

