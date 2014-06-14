/* linux/include/asm/arch-hi3515v100/entry-macro.S
*
* Copyright (c) 2008 Hisilicon Co., Ltd. 
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
#include <mach/hardware.h>
#ifdef CONFIG_HI_PRIO_IRQ
#define NR_IRQS 32
irq_priority_base:
.word irq_priority
                .macro  disable_fiq
                .endm

                .macro  get_irqnr_preamble, base, tmp
                .endm

                .macro  arch_ret_to_user, tmp1, tmp2
                .endm


		.macro	get_irqnr_and_base, irqnr, prio, base, tmp
		ldr	\base, =IO_ADDRESS(REG_BASE_INTC)
		ldr	\tmp, [\base, #REG_INTC_IRQSTATUS]	@ get masked status

		ldr     \base, irq_priority_base
		mov	\irqnr, \base
		ldr     \prio, [\base]			@ priority 0
		ands	\prio, \tmp, \prio

		ldreq   \prio, [\base, #4]! 		@ priority 1
		andeqs	\prio, \tmp, \prio

		ldreq   \prio, [\base, #4]!		@ priority 2
		andeqs	\prio, \tmp, \prio

		ldreq   \prio, [\base, #4]!		@ priority 3
		andeqs	\prio, \tmp, \prio

		ldreq   \prio, [\base, #4]!		@ priority 4
		andeqs	\prio, \tmp, \prio

		ldreq   \prio, [\base, #4]!		@ priority 5
		andeqs	\prio, \tmp, \prio

		ldreq   \prio, [\base, #4]!		@ priority 6
		andeqs	\prio, \tmp, \prio

		ldreq   \prio, [\base, #4]!		@ priority 7
		andeqs	\prio, \tmp, \prio

		ldreq   \prio, [\base, #4]!		@ priority 8
		andeqs	\prio, \tmp, \prio

		ldreq   \prio, [\base, #4]!		@ priority 9
		andeqs	\prio, \tmp, \prio

		ldreq   \prio, [\base, #4]!		@ priority 10
		andeqs	\prio, \tmp, \prio

		ldreq   \prio, [\base, #4]!		@ priority 11
		andeqs	\prio, \tmp, \prio

		ldreq   \prio, [\base, #4]!		@ priority 12
		andeqs	\prio, \tmp, \prio

		ldreq   \prio, [\base, #4]!		@ priority 13
		andeqs	\prio, \tmp, \prio

		ldreq   \prio, [\base, #4]!		@ priority 14
		andeqs	\prio, \tmp, \prio

		ldreq   \prio, [\base, #4]!		@ priority 15
		andeqs	\prio, \tmp, \prio

		ldreq   \prio, [\base, #4]!		@ priority 16
		andeqs	\prio, \tmp, \prio

		ldreq   \prio, [\base, #4]!		@ priority 17
		andeqs	\prio, \tmp, \prio

		ldreq   \prio, [\base, #4]!		@ priority 18
		andeqs	\prio, \tmp, \prio


		ldreq   \prio, [\base, #4]!		@ priority 19
		andeqs	\prio, \tmp, \prio


		ldreq   \prio, [\base, #4]!		@ priority 20 
		andeqs	\prio, \tmp, \prio


		ldreq   \prio, [\base, #4]!		@ priority 21
		andeqs	\prio, \tmp, \prio


		ldreq   \prio, [\base, #4]!		@ priority 22
		andeqs	\prio, \tmp, \prio


		ldreq   \prio, [\base, #4]!		@ priority 23
		andeqs	\prio, \tmp, \prio


		ldreq   \prio, [\base, #4]!		@ priority 24
		andeqs	\prio, \tmp, \prio


		ldreq   \prio, [\base, #4]!		@ priority 25
		andeqs	\prio, \tmp, \prio


		ldreq   \prio, [\base, #4]!		@ priority 26
		andeqs	\prio, \tmp, \prio


		ldreq   \prio, [\base, #4]!		@ priority 27
		andeqs	\prio, \tmp, \prio

		ldreq   \prio, [\base, #4]!		@ priority 28
		andeqs	\prio, \tmp, \prio

		ldreq   \prio, [\base, #4]!		@ priority 29
		andeqs	\prio, \tmp, \prio

		ldreq   \prio, [\base, #4]!		@ priority 30 
		andeqs	\prio, \tmp, \prio

		ldreq   \prio, [\base, #4]!		@ priority 31
		andeqs	\prio, \tmp, \prio



		sub	\base,	\base,	\irqnr
		mov	\base,	\base,	lsr#2
		clzne	\irqnr, \prio
		rsbne	\irqnr, #(NR_IRQS-1)
		.endm
#endif

