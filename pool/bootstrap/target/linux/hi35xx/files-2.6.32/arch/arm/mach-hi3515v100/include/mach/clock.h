/* linux/include/asm-arm/arch-hi3515v100/clock.h
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

#ifndef __ASM_ARCH_CLOCK_H
#define __ASM_ARCH_CLOCK_H

struct module;

#define MAX_CLK_NAME	16
struct clk {
	struct list_head	list;
	struct list_head	child_list;

	unsigned long		rate;

	char			name[MAX_CLK_NAME];
	char 			parent_name[MAX_CLK_NAME];

	struct module		*owner;
	void			*data;

	int (*notifier)(struct clk *clk, unsigned int cmd, unsigned long wparam, unsigned long lparam);
};


#define HISILICON_CLK_REGISTER		0x001
#define HISILICON_CLK_UNREGISTER	0x002
#define HISILICON_CLK_PARENT_DEL	0x003
#define HISILICON_CLK_SETRATE		0x004
#define HISILICON_CLK_PARENT_SETRATE	0x005

int hisilicon_clk_default_notifier(struct clk *clk, unsigned int cmd, unsigned long wparam, unsigned long lparam);

int clk_register(struct clk *clk);
void clk_unregister(struct clk *clk);
int clk_info(char *buf, int len);
int clk_notify_childs(struct clk *clk, unsigned int cmd, unsigned long wparam, unsigned long lparam);

unsigned long get_ahbclk_hw(void);

static inline long __pow(long a, unsigned int x)
{
	if(x==0)
		return 1;
	for(;x!=1;x--)
		a = a*a;
	return a;
}

#define RD_PLLC() readl(hisilicon_sctl_reg(0x18))
#define WR_PLLC(x) do{\
			unsigned long irqs; int i=PLLC_TO_ARMCORE(x)/1000/4; \
			local_irq_save(irqs);\
			writel(x, hisilicon_sctl_reg(0x18));\
			while(i--) __asm__ __volatile__("nop");\
			local_irq_restore(irqs);\
		}while(0)

#define PLLC_TO_ARMCORE(x) ({unsigned long __pllc=(x), __freq; \
		__freq=(HISILICON_OSC_CLOCK*((__pllc>>4)&0x0FF))/( max_t(unsigned long,__pllc&0x0F,1) * __pow(2, ((__pllc>>12)&0x03)) ); \
		__freq; })
#define PLLC_TO_AHBCLK(x) (PLLC_TO_ARMCORE(x)/HISILICON_ARMCORECLK_SCALE)
#define AHBCLK_TO_ARMCORE(x) ((x)*HISILICON_ARMCORECLK_SCALE)
#define ARMCORE_TO_AHBCLK(x) ((x)/HISILICON_ARMCORECLK_SCALE)

#define MIN_AHBCLK	(10*MHZ)
#define MAX_AHBCLK	(150*MHZ)

#define CHECK_AHBCLK(x) ((x)>MIN_AHBCLK && (x)<MAX_AHBCLK)

#endif /* __ASM_ARCH_CLOCK_H */


