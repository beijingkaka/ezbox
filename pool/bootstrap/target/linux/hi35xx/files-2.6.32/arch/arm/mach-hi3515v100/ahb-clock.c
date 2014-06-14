/* linux/arch/arm/mach-hi3515_v100/ahb-clock.c
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

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/string.h>

#include <linux/semaphore.h>
#include <linux/clk.h>

#include <mach/system.h>
#include <mach/clock.h>
#include <mach/early-debug.h>

static unsigned long hil_def_ahbclk = CONFIG_DEFAULT_BUSCLK;
static unsigned long set_busclk = 0;

static int __init hil_setup_bus_clock(char *s)
{
	hil_def_ahbclk = simple_strtoul(s, NULL, 0);
	set_busclk = 1;
	
	return 0;
}
__setup("busclk=", hil_setup_bus_clock);

unsigned long get_ahbclk_hw(void)
{
#if 0
	unsigned long pllc = RD_PLLC();

	return PLLC_TO_AHBCLK(pllc);
#endif
	if (set_busclk)
	{
		return hil_def_ahbclk;
	}

	/* read clk from PLL */
	return CONFIG_DEFAULT_BUSCLK;
}

static int ahb_clock_notifier(struct clk *clk, 
		unsigned int cmd, unsigned long wparam, unsigned long lparam)
{
	int ret = 0;
	ulong pllc;
	struct clk *p;

	switch(cmd) {
	case HISILICON_CLK_REGISTER:
		edb_trace(1);

		clk->rate = get_ahbclk_hw() ;
		clk_notify_childs(clk, HISILICON_CLK_PARENT_SETRATE, clk->rate, lparam);

		hisilicon_trace(2,"AHB clock is %lu HZ", clk->rate);
		break;

	case HISILICON_CLK_SETRATE:
	case HISILICON_CLK_PARENT_SETRATE:
		printk(KERN_INFO "currently do not support busclk change!\n");
		break;

	default:
		ret =  hisilicon_clk_default_notifier(clk, cmd, wparam, lparam);
		break;
	}

	return ret;
}

static int armcore_clock_notifier(struct clk *clk, 
		unsigned int cmd, unsigned long wparam, unsigned long lparam)
{
	int ret = 0;
	struct clk *p;
	
	switch(cmd) {
	case HISILICON_CLK_REGISTER:
		p = clk_get_parent(clk);
		if(p)
			clk->rate = BUSCLK_TO_CPUCLK(clk_get_rate(p));
		break;

	case HISILICON_CLK_SETRATE:
		p = clk_get_parent(clk);
		if(p)
			clk_set_rate(p, BUSCLK_TO_CPUCLK(wparam));
		clk_notify_childs(clk, HISILICON_CLK_PARENT_SETRATE, clk->rate, lparam);
		break;

	case HISILICON_CLK_PARENT_SETRATE:
		clk->rate = BUSCLK_TO_CPUCLK(wparam);
		clk_notify_childs(clk, HISILICON_CLK_PARENT_SETRATE, clk->rate, lparam);
		break;

	default:
		ret =  hisilicon_clk_default_notifier(clk, cmd, wparam, lparam);
		break;
	}

	return ret;
}

static struct clk xtal_clk = {
	.name		= "PLLIN",
	.rate		= CONFIG_DEFAULT_MPLLIN,
};

static struct clk ahb_clk = {
	.name		= "BUSCLK",
	.rate		= CONFIG_DEFAULT_BUSCLK,
	//.parent_name	= "PLLIN",
	//.notifier	= ahb_clock_notifier,
};

static struct clk armcore_clk = {
	.name		= "CPUCLK",
	.rate		= BUSCLK_TO_CPUCLK(CONFIG_DEFAULT_BUSCLK),
	//.parent_name	= "BUSCLK",
	//.notifier	= armcore_clock_notifier,
};

static struct clk timer2_clk = {
	.name		= "TIMER2CLK",
	.parent_name = "BUSCLK",
};


static int __init ahbclk_init(void)
{
	ahb_clk.rate = get_ahbclk_hw();
	armcore_clk.rate = BUSCLK_TO_CPUCLK(get_ahbclk_hw());
	clk_register(&xtal_clk);
	clk_register(&ahb_clk);
	clk_register(&armcore_clk);
	clk_register(&timer2_clk);

	return 0;
}

subsys_initcall(ahbclk_init);

