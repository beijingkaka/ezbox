/* linux/include/asm-arm/arch-hi3515v100/timer.h
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
#ifndef __ASM_ARCH_TIMER_H__
#define __ASM_ARCH_TIMER_H__

#define CFG_TIMER_VABASE	IO_ADDRESS(REG_BASE_TIMER12)
#define CFG_TIMER_CONTROL	( (1<<7) | (1<<6) | (1<<5) | (1<<1) )
#define CFG_TIMER_ONE_CONTROL	( (1<<7) | (1<<5) | (1<<1)| 1 )
#define CONFIG_DEFAULT_TIMERCLK  (100000000)
#define CFG_TIMER_PRESCALE      2	
#define BUSCLK_TO_TIMER_RELOAD(busclk)	(((busclk)/CFG_TIMER_PRESCALE)/HZ)
#define BUSCLK_TO_TIMER0_CLK_HZ(busclk)  ((busclk)/CFG_TIMER_PRESCALE)
#define BUSCLK_TO_TIMER1_CLK_HZ(busclk)  ((busclk)/CFG_TIMER_PRESCALE)
#define BUSCLK_TO_TIMER0_CLK_KHZ(busclk) (((busclk)/CFG_TIMER_PRESCALE)/1000)
#define BUSCLK_TO_TIMER1_CLK_KHZ(busclk) (((busclk)/CFG_TIMER_PRESCALE)/1000)
#define CFG_TIMER_INTNR		INTNR_TIMER_0_1
struct hisilicon_timer_onchip{
        unsigned long reload;
        unsigned long value;
        unsigned long ctrl;
        unsigned long clear;

        unsigned long ris_ro;
        unsigned long mis_ro;
        unsigned long bigload;
};

#define hisilicon_onchip_timer1(n) ((struct hisilicon_timer_onchip *)IO_ADDRESS(REG_BASE_TIMER12))
#define hisilicon_onchip_timer2(n) ((struct hisilicon_timer_onchip *)IO_ADDRESS(REG_BASE_TIMER23))

#define __TIMER_CTRL(tmr_ena, tmr_mode, int_ena, clk_pre, tmr_size, one_shot) \
		( (((tmr_ena)&0x01)<<7) | (((tmr_mode)&0x01)<<6) | (((int_ena)&0x01)<<5) | \
		  (((clk_pre)&0x03)<<2) | (((tmr_size)&0x01)<<1) | ((one_shot)&0x01) )

#define HISILICON_SYSTIMER_CTRL       __TIMER_CTRL(1,1,1,1,1,0)

#define __CALC_TIMER_RELOAD_DIVER	(16*HZ)
#define __CALC_TIMER_RELOAD(ahb_clk)	(((ahb_clk)+__CALC_TIMER_RELOAD_DIVER/2)/__CALC_TIMER_RELOAD_DIVER)
#define HISILICON_SYSTIMER_RELOAD     __CALC_TIMER_RELOAD(HISILICON_AHBCLK_DEFAULT)

#define HIS_TIMER_INT_CLEAN(timer) do{ ((volatile struct hisilicon_timer_onchip *)(timer))->clear = 0x00; }while(0)


extern unsigned long hisilicon_timer_reload;

#define hisilicon_ticks2us(ticks) (((ticks)*((1000000/HZ) >> 2))/(hisilicon_timer_reload >> 2))
void hisilicon_timer_init(void);
unsigned long hisilicon_gettimeoffset(void);
unsigned long long sched_clock(void);

#endif /* __ASM_ARCH_TIMER_H__ */



