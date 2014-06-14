/* linux/include/asm-arm/arch-hi3515v100/system.h
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
#include <mach/hardware.h>
#include <asm/io.h>
#include <mach/platform.h>

#ifndef __ASM_ARCH_SYSTEM_H
#define __ASM_ARCH_SYSTEM_H

void arch_idle(void);
void arch_reset(char mode, const char *cmd);


#define hisilicon_intc_reg(x)	IO_ADDRESS(REG_BASE_INTC + (x))
#define hisilicon_sctl_reg(x)	IO_ADDRESS(REG_BASE_SCTL +(x))

#endif /* __ASM_ARCH_SYSTEM_H */

