/* linux/include/asm-arm/arch-hi3515v100/platform.h
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
#ifndef	__HI_CHIP_REGS_H__
#define	__HI_CHIP_REGS_H__


#define REG_BASE_NANDC	0x10000000
#define REG_NANDC_IOSIZE	PAGE_ALIGN(0x10000)

#define REG_BASE_SMI	0x10010000
#define REG_SMI_IOSIZE	PAGE_ALIGN(0x10000)

#define REG_BASE_SDIO	0x10030000
#define REG_SDIO_IOSIZE	PAGE_ALIGN(0x10000)

#define REG_BASE_SIO0	0x10040000
#define REG_SIO0_IOSIZE	PAGE_ALIGN(0x10000)

#define REG_BASE_SIO1	0x10050000
#define REG_SIO1_IOSIZE	PAGE_ALIGN(0x10000)

#define REG_BASE_INTC 	0x10070000	
#define REG_INTC_IOSIZE 	PAGE_ALIGN(0x10000)

#define REG_BASE_ETH 	0x10090000	
#define REG_ETH_IOSIZE 	PAGE_ALIGN(0x10000)

#define REG_BASE_USBOHCI 	0x100a0000	
#define REG_USBOHCI_IOSIZE 	PAGE_ALIGN(0x10000)

#define REG_BASE_USBEHCI 	0x100b0000	
#define REG_USBEHCI_IOSIZE 	PAGE_ALIGN(0x10000)

#define REG_BASE_CIPHER 	0x100c0000	
#define REG_CIPHER_IOSIZE 	PAGE_ALIGN(0x10000)

#define REG_BASE_DMAC 	0x100d0000	
#define REG_DMAC_IOSIZE 	PAGE_ALIGN(0x10000)

#define REG_BASE_VEDU0	0x100e0000	
#define REG_VEDU0_IOSIZE 	PAGE_ALIGN(0x10000)

#define REG_BASE_VIU 	0x10100000	
#define REG_VIU_IOSIZE 	PAGE_ALIGN(0x10000)

#define REG_BASE_TIMER12	0x20000000
#define REG_TIMER12_IOSIZE	PAGE_ALIGN(0x10000)

#define REG_BASE_TIMER34	0x20010000
#define REG_TIMER34_IOSIZE	PAGE_ALIGN(0x10000)

#define REG_BASE_WDG	0x20040000
#define REG_WDG_IOSIZE	PAGE_ALIGN(0x10000)

#define REG_BASE_SCTL	0x20050000
#define REG_SCTL_IOSIZE	PAGE_ALIGN(0x10000)

#define REG_BASE_RTC	0x20060000
#define REG_RTC_IOSIZE	PAGE_ALIGN(0x10000)

#define REG_BASE_IR	0x20070000
#define REG_IR_IOSIZE	PAGE_ALIGN(0x10000)

#define REG_BASE_UART0	0x20090000
#define REG_UART0_IOSIZE	PAGE_ALIGN(0x10000)

#define REG_BASE_UART1	0x200a0000
#define REG_UART1_IOSIZE	PAGE_ALIGN(0x10000)

#define REG_BASE_UART2	0x200b0000
#define REG_UART2_IOSIZE	PAGE_ALIGN(0x10000)

#define REG_BASE_UART3	0x200c0000
#define REG_UART3_IOSIZE	PAGE_ALIGN(0x10000)

#define REG_BASE_I2C	0x200d0000
#define REG_I2C_IOSIZE	PAGE_ALIGN(0x10000)

#define REG_BASE_SPI	0x200e0000
#define REG_SPI_IOSIZE	PAGE_ALIGN(0x10000)

#define REG_BASE_IO	0x200f0000
#define REG_IO_IOSIZE	PAGE_ALIGN(0x10000)

#define REG_BASE_DDRC	0x20110000
#define REG_DDRC_IOSIZE	PAGE_ALIGN(0x10000)

#define REG_BASE_VOU	0x20130000
#define REG_VOU_IOSIZE	PAGE_ALIGN(0x10000)

#define REG_BASE_TDE	0x20140000
#define REG_TDE_IOSIZE	PAGE_ALIGN(0x10000)

#define REG_BASE_GPIO0	0x20150000
#define REG_GPIO0_IOSIZE	PAGE_ALIGN(0x10000)

#define REG_BASE_GPIO1	0x20160000
#define REG_GPIO1_IOSIZE	PAGE_ALIGN(0x10000)

#define REG_BASE_GPIO2	0x20170000
#define REG_GPIO2_IOSIZE	PAGE_ALIGN(0x10000)

#define REG_BASE_GPIO3	0x20180000
#define REG_GPIO3_IOSIZE	PAGE_ALIGN(0x10000)

#define REG_BASE_GPIO4	0x20190000
#define REG_GPIO4_IOSIZE	PAGE_ALIGN(0x10000)

#define REG_BASE_GPIO5	0x201a0000
#define REG_GPIO5_IOSIZE	PAGE_ALIGN(0x10000)

#define REG_BASE_GPIO6	0x201b0000
#define REG_GPIO6_IOSIZE	PAGE_ALIGN(0x10000)

#define REG_BASE_GPIO7	0x201c0000
#define REG_GPIO7_IOSIZE	PAGE_ALIGN(0x10000)


/* SYSTEM CONTROL REG */
#define REG_SC_CTRL	0x000
#define REG_SC_SYSSTAT	0x004
#define REG_SC_ITMCTRL	0x008
#define REG_SC_IMSTAT	0x00C
#define REG_SC_XTALCTRL	0x010
#define REG_SC_PLLCTRL	0x014
#define REG_SC_PLLFCTRL	0x018
#define REG_SC_PERCTRL0	0x01C
#define REG_SC_PERCTRL1	0x020
#define REG_SC_PEREN	0x024
#define REG_SC_PERDIS	0x028
#define REG_SC_PERCLKEN	0x02C
#define REG_SC_RESERVED	0x030
#define REG_SC_PERCTRL2	0x034
#define REG_SC_PERCTRL3	0x038
#define REG_SC_PERCTRL4	0x03C
#define REG_SC_PERLOCK	0x044
#define REG_SC_SYSID	0xEE0
#define REG_SC_SLAVE	0x050

/* SMI REG */
#define REG_SMI_BIDCYR1			0x000
#define REG_SMI_BWSTRDR1		0x004
#define REG_SMI_BWSTWRR1		0x008
#define REG_SMI_BWSTOENR1		0x00C
#define REG_SMI_BWSTWENR1		0x010
#define REG_SMI_BCR1			0x014
#define REG_SMI_BSR1			0x018
#define REG_SMI_BWSTBRDR1		0x01C
#define REG_SMI_BIDCYR0			0x0E0
#define REG_SMI_BWSTRDR0		0x0E4
#define REG_SMI_BWSTWRR0		0x0E8
#define REG_SMI_BWSTOENR0		0x0EC
#define REG_SMI_BWSTWENR0		0x0F0
#define REG_SMI_BCR0			0X0F4
#define REG_SMI_BSR0			0x0F8
#define REG_SMI_BWSTBRDR0		0x0FC
#define REG_SMI_SR				0x200
#define REG_SMI_CR				0x204

#define REG_INTC_IRQSTATUS	0x000
#define REG_INTC_FIQSTATUS	0x004
#define REG_INTC_RAWSTATUS	0x008
#define REG_INTC_INTSELECT	0x00C
#define REG_INTC_INTENABLE	0x010
#define REG_INTC_INTENCLEAR	0x014
#define REG_INTC_SOFTINT	0x018
#define REG_INTC_SOFTINTCLEAR	0x01C
#define REG_INTC_PROTECTION	0x020

#define INTNR_IRQ_START	0
#define INTNR_IRQ_END	31

#define INTNR_WATCHDOG					0	/* Watchdog timer */
#define INTNR_SOFTINT					1	/* Software interrupt */
#define INTNR_COMMRx					2	/* Debug Comm Rx interrupt */
#define INTNR_COMMTx					3	/* Debug Comm Tx interrupt */
#define INTNR_TIMER_0_1					4	/* Timer 0 and 1 */
#define INTNR_TIMER_2_3                 5	/* Timer 2 and 3 */
#define INTNR_GPIO_0                    6	/* GPIO 0 */
#define INTNR_GPIO_1                    7	/* GPIO 1 */
#define INTNR_GPIO_2_7                  8	/* GPIO 2 */
#define INTNR_IR						9	/* GPIO 3 */
#define INTNR_RTC						10	/* Real Time Clock */
#define INTNR_SPI						11	/* Synchronous Serial Port */
#define INTNR_UART0						12
#define INTNR_UART1						12
#define INTNR_UART2						13
#define INTNR_UART3						13
#define INTNR_ETH						14
#define INTNR_DMAC						15
#define INTNR_I2C						16
#define INTNR_VIU						17
#define INTNR_TDE						18
#define INTNR_VOU						19
#define INTNR_VEDU						20
#define INTNR_OHCI						22
#define INTNR_EHCI						23
#define INTNR_SDIO						24
#define INTNR_SIO_0						25
#define INTNR_SIO_1						26
#define INTNR_CIPHER					28
#define INTNR_SATA						29
#define INTNR_NANDC						30

#define REG_TIMER_RELOAD	0x000
#define REG_TIMER_VALUE		0x004
#define REG_TIMER_CONTROL	0x008
#define REG_TIMER_INTCLR	0x00C
#define REG_TIMER_RIS		0x010
#define REG_TIMER_MIS		0x004
#define REG_TIMER_BGLOAD	0x008

#define REG_TIMER1_RELOAD	0x020
#define REG_TIMER1_VALUE	0x024
#define REG_TIMER1_CONTROL	0x028
#define REG_TIMER1_INTCLR	0x02C
#define REG_TIMER1_RIS		0x030
#define REG_TIMER1_MIS		0x034
#define REG_TIMER1_BGLOAD	0x038
	
#define REG_DDRC_STATUS		0x000
#define REG_DDRC_CTRL		0x004
#define REG_DDRC_EMRS01		0x008
#define REG_DDRC_EMRS23		0x00C
#define REG_DDRC_CONFIG		0x010
#define REG_DDRC_TIMING0	0x020
#define REG_DDRC_TIMING1	0x024
#define REG_DDRC_TIMING2	0x028
#define REG_DDRC_TIMING3	0x02C
#define REG_DDRC_ODT_CONFIG	0x040
#define REG_DDRC_PHY_CONFIG	0x060
#define REG_DDRC_DLL_STATUS	0x078
#define REG_DDRC_DLL_CONFIG	0x07C
#define REG_DDRC_QOS_CONFIG	0x094
#define REG_DDRC_CH0_QOS	0x098
#define REG_DDRC_CH1_QOS	0x09C
#define REG_DDRC_CH2_QOS	0x0A0
#define REG_DDRC_CH3_QOS	0x0A4
#define REG_DDRC_CH4_QOS	0x0A8
#define REG_DDRC_CH5_QOS	0x0AC
#define REG_DDRC_CH6_QOS	0x0B0
#define REG_DDRC_CH7_QOS	0x0B4

#define DDRC_BUSWITH_32BITS     1
#define DDRC_BUSWITH_16BITS     0

#define DDRC_CHIPCAP_64Mb       0
#define DDRC_CHIPCAP_128Mb      1
#define DDRC_CHIPCAP_256Mb      2
#define DDRC_CHIPCAP_512Mb      3
#define DDRC_CHIPCAP_1Gb      	4
#define DDRC_CHIPCAP_2Gb      	5

#define DDRC_CHIP_8BITS         0
#define DDRC_CHIP_16BITS        1
#define DDRC_CHIP_32BITS        2

#define DDRC_CHIP_4BANK         0
#define DDRC_CHIP_8BANK         1

#define DDRC_READDELAY_2        0
#define DDRC_READDELAY_2_5      1
#define DDRC_READDELAY_3        2
#define DDRC_READDELAY_4        3
#define DDRC_READDELAY_5        4
#define DDRC_READDELAY_6        5


#define MEM_BASE_ITCM	0x00000000
#define MEM_SIZE_ITCM	0x00000800
#define MEM_CONF_ITCM_SIZE	3

#endif /*End of __HI_CHIP_REGS_H__ */

