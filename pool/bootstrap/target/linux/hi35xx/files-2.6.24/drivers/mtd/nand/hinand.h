/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hinand.c
  Version       : 1.0
  Author        : Hisilicon multimedia software group
  Created       : 2009/02/15
  Last Modified :
  Description   : nand flash controller driver
  Function List :
  History       :
  1.Date        : 2009/02/15
    Author      : Zhan Weitao / Zhang Kuanhuai
    Modification: Created file

******************************************************************************/
#include <asm/hardware.h>
#include <linux/kernel.h>
#ifndef _HISILICON_NAND_H
#define _HISILICON_NAND_H
#include <asm/io.h>

/* -----------------------------------------------------------*/
/* the macro prefix with CONFIG_ is configuration parameter   */

#define CONFIG_HINAND_PAGE_2K
#ifdef CONFIG_HINAND_PAGE_2K
#define CONFIG_HINAND_PAGE_SIZE    2048
#define CONFIG_HINAND_OOB_SIZE     64
#define CONFIG_HINAND_ECC_TYPE         1
#define CONFIG_HINAND_PAGESIZE_BITS    1
#elif defined CONFIG_HINAND_PAGE_4K
#define CONFIG_HINAND_PAGE_SIZE    4096
#define CONFIG_HINAND_OOB_SIZE     128
#define CONFIG_HINAND_ECC_TYPE         2
#define CONFIG_HINAND_PAGESIZE_BITS    2
#endif

/* -----------------------------------------------------------*/

#define HINAND_BUFFER_SIZE   (2048+128)

#define HI_NAND_BASE_VIRT_REG_ADDR  0x10000000
#define HI_NAND_BASE_VIRT_REG_LEN   0x00010000
#define HI_NAND_BASE_VIRT_MEM_ADDR  0x70000000
#define HI_NAND_BASE_VIRT_MEM_LEN   0x04000000

#define HINAND_SYSREG_BASE 				(0x101e0000)
#define HINAND_SYSREG_PEREN             (HINAND_SYSREG_BASE + 0x38)
#define HINAND_SYSREG_PERDIS            (HINAND_SYSREG_BASE + 0X3C)
#define HINAND_SYSREG_PERSTAT           (HINAND_SYSREG_BASE + 0X44)
#define HINAND_SYSREG_PERIRSTREQN       (HINAND_SYSREG_BASE + 0X48)
#define HINAND_SYSREG_PINCTRL1          (HINAND_SYSREG_BASE + 0x58)

#define BIT_HINAND_SYSREG_PEREN_NFC_CLKEN       (1U << 20)
#define BIT_HINAND_SYSREG_PERDIS_NFC_CLKDEN     (1U << 20)
#define HINAND_SYSREG_PERIRSTREQN_NFC_RSTREQN   (1U << 22)
#define BIT_HINAND_SYSREG_PINCTRL1_GPIO_NF_MODE     (1U << 19)
#define HINAND_NFC_CON        0X000
#define HINAND_NFC_PWIDTH     0X004
#define HINAND_NFC_OPIDLE     0X008
#define HINAND_NFC_CMD        0X00C
#define HINAND_NFC_ADDRL      0X010
#define HINAND_NFC_ADDRH      0X014
#define HINAND_NFC_DATA_NUM   0X018 
#define HINAND_NFC_OP         0X01C
#define HINAND_NFC_STATUS     0X020
#define HINAND_NFC_INTEN      0X024
#define HINAND_NFC_INTS       0X028
#define HINAND_NFC_INTCLR     0X02C
#define HINAND_NFC_LOCK       0X030
#define HINAND_NFC_LOCK_SA0   0X034
#define HINAND_NFC_LOCK_SA1   0X038
#define HINAND_NFC_LOCK_SA2   0X03C
#define HINAND_NFC_LOCK_SA3   0X040
#define HINAND_NFC_LOCK_EA0   0X044
#define HINAND_NFC_LOCK_EA1   0X048
#define HINAND_NFC_LOCK_EA2   0X04C
#define HINAND_NFC_LOCK_EA3   0X050
#define HINAND_NFC_EXPCMD     0X054
#define HINAND_NFC_EXBCMD     0X058
#define HINAND_NFC_ECC_TEST   0X05C

/*bits of HINAND_NFC_CON*/
#define BIT_HINAND_NFC_CON_EDO_EN              (1U << 12)
#define BIT_HINAND_NFC_CON_ECC_TYPE            (CONFIG_HINAND_ECC_TYPE << 10)
#define BIT_HINAND_NFC_CON_EXT_DATA_ECC_EN     (1U << 9)
#define BIT_HINAND_NFC_CON_PROTECTION_EN       (1U << 8)
#define BIT_HINAND_NFC_CON_READY_BUSY_SEL      (1U << 7)
#define BIT_HINAND_NFC_CON_CS_CTRL             (1U << 6)
#define BIT_HINAND_NFC_CON_ECC_EN              (1U << 5)
#define BIT_HINAND_NFC_CON_BUS_WIDTH           (1U << 3)
#define BIT_HINAND_NFC_CON_PAGESIZE            (CONFIG_HINAND_PAGESIZE_BITS << 1)
#define BIT_HINAND_NFC_CON_OP_MODE             (1U << 0)
                                               
/*bits of HINAND_NFC_PWIDTH*/                  
#define BIT_HINAND_NFC_PWIDTH_RW_HCNT          (1U << 8)
#define BIT_HINAND_NFC_PWIDTH_R_LCNT           (1U << 4)
#define BIT_HINAND_NFC_PWIDTH_LCNT             (1U << 0)
                                               
/*bits of HINAND_NFC_OPIDLE*/                  
#define BIT_HINAND_NFC_OPIDLE_CMD1_WAIT        (1U << 16)
#define BIT_HINAND_NFC_OPIDLE_ADDR_WAIT        (1U << 12)
#define BIT_HINAND_NFC_OPIDLE_WRITE_DATA_WAIT  (1U << 8)
#define BIT_HINAND_NFC_OPIDLE_CMD2_WAIT        (1U << 4)
#define BIT_HINAND_NFC_OPIDLE_WAIT_READY_WAIT  (1U << 0)
                                               
/*bits of HINAND_NFC_CMD*/                     
#define BIT_HINAND_NFC_CMD_READ_STATUS_CMD     (1U << 16)
#define BIT_HINAND_NFC_CMD_CMD2                (1U << 8)
#define BIT_HINAND_NFC_CMD_CMD1                (1U << 0)

/*bits of HINAND_NFC_OP*/
#define BIT_HINAND_NFC_OP_ADDRESS_CYCLE        (1U << 9)
#define BIT_HINAND_NFC_OP_ADDRESS_CYCLE_BIT    (0x9)
#define BIT_HINAND_NFC_OP_NF_CS                (1U << 7)
#define BIT_HINAND_NFC_OP_NF_CS_BIT            (0x7)
#define BIT_HINAND_NFC_OP_CMD1_EN              (1U << 6)
#define BIT_HINAND_NFC_OP_ADDR_EN              (1U << 5)
#define BIT_HINAND_NFC_OP_WRITE_DATA_EN        (1U << 4)
#define BIT_HINAND_NFC_OP_CMD2_EN              (1U << 3)
#define BIT_HINAND_NFC_OP_WAIT_READY_EN        (1U << 2)
#define BIT_HINAND_NFC_OP_READ_DATA_EN         (1U << 1)
#define BIT_HINAND_NFC_OP_READ_STATUS_EN       (1U << 0)
#define BIT_HINAND_NFC_OP_ADDRESS_CYCLE_MASK   (0x7)
        
/*bits of HINAND_NFC_STATUS*/  
#define HINAND_NFC_STATUS_NF_STATUS            (1U << 5)
#define HINAND_NFC_STATUS_NF3_READY            (1U << 4)
#define HINAND_NFC_STATUS_NF2_READY            (1U << 3)
#define HINAND_NFC_STATUS_NF1_READY            (1U << 2)
#define HINAND_NFC_STATUS_NF0_READY            (1U << 1)
#define HINAND_NFC_STATUS_NFC_READY            (1U << 0)

/*bits of int*/                                
#define BIT_HINAND_NFC_INT_WR_BUF_ERR_INT      (1U << 8)
#define BIT_HINAND_NFC_INT_WR_BUF_BUSY_INT     (1U << 7)
#define BIT_HINAND_NFC_INT_ERR_INVALID         (1U << 6)
#define BIT_HINAND_NFC_INT_ERR_VALID           (1U << 5)
#define BIT_HINAND_NFC_INT_CS3_DONE            (1U << 4)
#define BIT_HINAND_NFC_INT_CS2_DONE            (1U << 3)
#define BIT_HINAND_NFC_INT_CS1_DONE            (1U << 2)
#define BIT_HINAND_NFC_INT_CS0_DONE            (1U << 1)
#define BIT_HINAND_NFC_INT_OP_DONE             (1U << 0)

#define BIT_HINAND_NFC_ECC_TEST_EMPTY              (1U << 12)
#define BIT_HINAND_NFC_ECC_TEST_RSY                (1U << 3)
#define BIT_HINAND_NFC_ECC_TEST_ECC_MASK           (1U << 2)
#define BIT_HINAND_NFC_ECC_TEST_DEC_ONLY           (1U << 1)
#define BIT_HINAND_NFC_ECC_TEST_ENC_ONLY           (1U << 0)


struct hinand_data {
		struct nand_chip	chip;
		struct mtd_info		mtd;
		void __iomem		*iobase;
		unsigned int            page_offset;
		unsigned int            command;
		unsigned int hinand_address_cycle;
		unsigned int hinand_address_buf[2];
        unsigned int column_addr;
        char *       buffer;
#ifdef CONFIG_MTD_PARTITIONS
		int			nr_parts;
		struct mtd_partition	*parts;
#endif
};

static struct nand_ecclayout hinand_oob_64_1bit =
{
	.eccbytes    = 20,
	.eccpos      = {
		11, 12, 13, 14, 15,
		27, 28, 29, 30, 31,
		43, 44, 45, 46, 47,
		59, 60, 61, 62, 63
	},
	.oobfree = {{2, 9}, {16, 11}, {32, 11}, {48, 11}}
};

static struct nand_ecclayout hinand_oob_64_4bit =
{
	.eccbytes    = 40,
	.eccpos      = {
		6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
		22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
		38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
		54, 55, 56, 57, 58, 59, 60, 61, 62, 63
	},
	.oobfree = {{2, 4}, {16, 6}, {32, 6}, {48, 6}}
};


static struct nand_ecclayout hinand_oob_128_4bit =
{
	.eccbytes    = 80,
	.eccpos      = {
		6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
		22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
		38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
		54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
		70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
		86, 87, 88, 89, 90, 91, 92, 93, 94, 95,
		102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
		118, 119, 120, 121, 122, 123, 124, 125, 126, 127
	},
	.oobfree = {{2, 4}, {16, 6}, {32, 6}, {48, 6}, 
	            {64, 6}, {80, 6}, {96, 6}, {112, 6}}
};

#ifdef CONFIG_MTD_PARTITIONS
#define	mtd_has_partitions()	(1)
#else
#define	mtd_has_partitions()	(0)
#endif


#define HINAND_ADDRESS_CYCLE_MASK              0x4
#define NAND_ID_NUM                            0x4
#define HINAND_STATUS_CMD                      0x700000

#define hinand_get_cs(data)  ((hinand_readl( data, HINAND_NFC_OP ) >> 0x7) & 0x3)
#define hinand_set_cs(data, num)  do {\
		unsigned int reg = 0;\
		reg |= (num << BIT_HINAND_NFC_OP_NF_CS_BIT); \
		hinand_writel( data, reg, HINAND_NFC_OP); \
}while(0);
        
#define hinand_readl(ld, ofs)        readl(ld->iobase + (ofs))
#define hinand_writel(ld, v, ofs)    writel(v, ld->iobase + (ofs))

#define CHIP_DELAY 25
#define HINAND_MAX_CHIP 1
#define HINAND_NFC_CON_ECC_ENABLE (BIT_HINAND_NFC_CON_ECC_TYPE         |  \
									BIT_HINAND_NFC_CON_READY_BUSY_SEL  |  \
									BIT_HINAND_NFC_CON_ECC_EN          |  \
									BIT_HINAND_NFC_CON_OP_MODE         |  \
									BIT_HINAND_NFC_CON_PAGESIZE)

#define HINAND_NFC_CON_ECC_DISABLE (BIT_HINAND_NFC_CON_READY_BUSY_SEL  |  \
									BIT_HINAND_NFC_CON_OP_MODE         |  \
									BIT_HINAND_NFC_CON_PAGESIZE)

#endif  
