/* linux/include/asm/arch-hi3515v100/entry-macro.S
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
#include <asm/arch/hardware.h>
#define NR_IRQS 32
/* VO > VI > DMAC > VEDU > MDU > DSU > ETH > PCI > MMC */
#define HI_INTTERUPT_PRIO_HIGH ( (1<<INTNR_VOU)	| ( 1 << INTNR_VIU) )	

#define HI_INTTERUPT_PRIO_MEDIUM ( (1<<INTNR_DMAC)| (1<<INTNR_VEDU) | (1<<INTNR_TDE) )	

		.macro	disable_fiq
		.endm
	        
		.macro  get_irqnr_preamble, base, tmp
        	.endm

	        .macro  arch_ret_to_user, tmp1, tmp2
        	.endm


		.macro	get_irqnr_and_base, irqnr, irqstat, base, tmp
		ldr	\base, =IO_ADDRESS(REG_BASE_INTC)
		ldr	\tmp, [\base, #REG_INTC_IRQSTATUS]	@ get masked status

		ldr	\irqstat,	=HI_INTTERUPT_PRIO_HIGH	
		ands	\irqstat, \tmp, \irqstat

		ldreq	\irqstat,	=HI_INTTERUPT_PRIO_MEDIUM 
		andeqs	\irqstat, \tmp, \irqstat

		/* normal priority */
		moveqs	\irqstat, \tmp

		clzne	\irqnr, \irqstat
		rsbne	\irqnr, #(NR_IRQS-1)

		.endm

