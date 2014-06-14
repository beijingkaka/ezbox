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

#include <linux/init.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/partitions.h>
#include <linux/delay.h>

#include <asm/io.h>
#include <asm/errno.h>
#include <linux/platform_device.h>
#include "hinand.h"

#undef debug
#define debug(fmt, args...)
//#define debug(fmt, args...) printk(fmt, ##args)

void printreg(struct hinand_data *data)
{
	unsigned int i, reg;

	for(i = 0; i < 0x30; i+=4)
	{
        reg = hinand_readl(data, i);
		printk("reg %x is %x ", i, reg);
	}
    printk("\n");
}

static void hinand_ecc_encipher(struct hinand_data *data)
{
	hinand_writel(data, HINAND_NFC_CON_ECC_ENABLE, HINAND_NFC_CON);
	hinand_writel(data, BIT_HINAND_NFC_ECC_TEST_ENC_ONLY, HINAND_NFC_ECC_TEST);
	while(!(hinand_readl(data, HINAND_NFC_ECC_TEST) & BIT_HINAND_NFC_ECC_TEST_ENC_ONLY));
	hinand_writel(data, HINAND_NFC_CON_ECC_DISABLE, HINAND_NFC_CON);
}

static void hinand_ecc_decipher(struct hinand_data *data)
{
    hinand_writel(data, HINAND_NFC_CON_ECC_ENABLE, HINAND_NFC_CON);
    hinand_writel(data, BIT_HINAND_NFC_ECC_TEST_DEC_ONLY, HINAND_NFC_ECC_TEST);
    while(!(hinand_readl(data, HINAND_NFC_ECC_TEST) & BIT_HINAND_NFC_ECC_TEST_ENC_ONLY));
    hinand_writel(data, HINAND_NFC_CON_ECC_DISABLE, HINAND_NFC_CON);
}
void hinand_init(struct mtd_info *mtd)
{
	struct nand_chip *chip = mtd->priv;
 	struct hinand_data *data = chip->priv;

    hinand_writel(data,
		BIT_HINAND_NFC_CON_READY_BUSY_SEL  | 
		BIT_HINAND_NFC_CON_BUS_WIDTH       |  		             
		BIT_HINAND_NFC_CON_OP_MODE         |
		BIT_HINAND_NFC_CON_PAGESIZE,
		HINAND_NFC_CON);

}

/* for 2k page, we use the buffer of controller directly */
static void hinand_read_2k(struct hinand_data *data)
{
    hinand_writel(data, HINAND_BUFFER_SIZE, HINAND_NFC_DATA_NUM);
    
	hinand_writel(data, data->hinand_address_buf[0] & 0xffff0000, HINAND_NFC_ADDRL);
	if(data->hinand_address_cycle > HINAND_ADDRESS_CYCLE_MASK)
	{
		hinand_writel(data, data->hinand_address_buf[1], HINAND_NFC_ADDRH);
	}

	hinand_writel(data, NAND_CMD_READSTART << 8 | NAND_CMD_READ0, HINAND_NFC_CMD);
	hinand_writel(data, HINAND_NFC_CON_ECC_DISABLE, HINAND_NFC_CON);
	hinand_writel(data, 
			BIT_HINAND_NFC_OP_CMD2_EN |
			BIT_HINAND_NFC_OP_ADDR_EN |
			BIT_HINAND_NFC_OP_CMD1_EN | 
			BIT_HINAND_NFC_OP_READ_DATA_EN  |
			BIT_HINAND_NFC_OP_WAIT_READY_EN |
			(data->hinand_address_cycle << BIT_HINAND_NFC_OP_ADDRESS_CYCLE_BIT),
			HINAND_NFC_OP);
    while((hinand_readl( data, HINAND_NFC_STATUS ) & 0x1) == 0x0);

    hinand_ecc_decipher(data);
    
	data->hinand_address_cycle = 0x0;
}

static void hinand_program_2k(struct hinand_data *data)
{    
    if(unlikely(!(data->column_addr)))
    {
        memset(data->chip.IO_ADDR_W, 0xff, data->column_addr);
    }

    hinand_writel(data, HINAND_BUFFER_SIZE, HINAND_NFC_DATA_NUM);

	hinand_writel(data, data->hinand_address_buf[0] & 0xffff0000, HINAND_NFC_ADDRL);
	if(data->hinand_address_cycle > HINAND_ADDRESS_CYCLE_MASK)
	{
		hinand_writel(data, data->hinand_address_buf[1], HINAND_NFC_ADDRH);
	}

    hinand_ecc_encipher(data);

	hinand_writel(data, NAND_CMD_PAGEPROG << 8 | HINAND_STATUS_CMD | NAND_CMD_SEQIN, HINAND_NFC_CMD);
	hinand_writel(data, HINAND_NFC_CON_ECC_DISABLE, HINAND_NFC_CON);
	hinand_writel(data,  
			BIT_HINAND_NFC_OP_READ_STATUS_EN | 
			BIT_HINAND_NFC_OP_WAIT_READY_EN  | 
			BIT_HINAND_NFC_OP_CMD1_EN        | 
			BIT_HINAND_NFC_OP_CMD2_EN        |
			BIT_HINAND_NFC_OP_ADDR_EN        |
			BIT_HINAND_NFC_OP_WRITE_DATA_EN  |
			(data->hinand_address_cycle << BIT_HINAND_NFC_OP_ADDRESS_CYCLE_BIT),
			HINAND_NFC_OP);

    while((hinand_readl( data, HINAND_NFC_STATUS ) & 0x1) == 0x0);
    
	data->hinand_address_cycle = 0x0;
}


/* 
    read function for 4k pagesize.
    the controller's buffer is 2k bytes. we have to use a 4k buffer in driver.
*/
static void hinand_read_4k(struct hinand_data *data)
{
	hinand_writel(data, data->hinand_address_buf[0] & 0xffff0000, HINAND_NFC_ADDRL);
	if(data->hinand_address_cycle > HINAND_ADDRESS_CYCLE_MASK)
	{
		hinand_writel(data, data->hinand_address_buf[1], HINAND_NFC_ADDRH);
	}

	hinand_writel(data, NAND_CMD_READSTART << 8 | NAND_CMD_READ0, HINAND_NFC_CMD);

	hinand_writel(data, HINAND_NFC_CON_ECC_DISABLE, HINAND_NFC_CON);
    hinand_writel(data, (CONFIG_HINAND_PAGE_SIZE+CONFIG_HINAND_OOB_SIZE)/2, HINAND_NFC_DATA_NUM);
	hinand_writel(data, 
			BIT_HINAND_NFC_OP_CMD2_EN |
			BIT_HINAND_NFC_OP_ADDR_EN |
			BIT_HINAND_NFC_OP_CMD1_EN | 
			BIT_HINAND_NFC_OP_READ_DATA_EN  |
			BIT_HINAND_NFC_OP_WAIT_READY_EN |
			(data->hinand_address_cycle << BIT_HINAND_NFC_OP_ADDRESS_CYCLE_BIT),
			HINAND_NFC_OP);
	
	while((hinand_readl( data, HINAND_NFC_STATUS ) & 0x1) == 0x0);

    hinand_ecc_decipher(data);

	memcpy(data->buffer, (unsigned char *)(data->chip.IO_ADDR_R), CONFIG_HINAND_PAGE_SIZE/2);
	memcpy(data->buffer+CONFIG_HINAND_PAGE_SIZE, 
        (unsigned char *)(data->chip.IO_ADDR_R+CONFIG_HINAND_PAGE_SIZE/2), CONFIG_HINAND_OOB_SIZE/2);

	hinand_writel(data, 
			BIT_HINAND_NFC_OP_READ_DATA_EN,
			HINAND_NFC_OP);

	while((hinand_readl( data, HINAND_NFC_STATUS ) & 0x1) == 0x0);

    hinand_ecc_decipher(data);

	memcpy(data->buffer + CONFIG_HINAND_PAGE_SIZE/2, 
        (unsigned char *)(data->chip.IO_ADDR_R), CONFIG_HINAND_PAGE_SIZE/2);
    
	memcpy(data->buffer + CONFIG_HINAND_PAGE_SIZE+CONFIG_HINAND_OOB_SIZE/2, 
        (unsigned char *)(data->chip.IO_ADDR_R + CONFIG_HINAND_PAGE_SIZE/2), CONFIG_HINAND_OOB_SIZE/2);

	data->hinand_address_cycle = 0x0;
}

/* program function for 4k pagesize */
static void hinand_program_4k(struct hinand_data *data)
{
    if(unlikely(!(data->column_addr)))
    {
        memset(data->buffer, 0xff, data->column_addr);
    }

  	hinand_writel(data, HINAND_NFC_CON_ECC_DISABLE, HINAND_NFC_CON);
    hinand_writel(data, (CONFIG_HINAND_PAGE_SIZE+CONFIG_HINAND_OOB_SIZE)/2, HINAND_NFC_DATA_NUM);
    memcpy((unsigned char *)(data->chip.IO_ADDR_W), 
        data->buffer, CONFIG_HINAND_PAGE_SIZE / 2);
    memcpy((unsigned char *)(data->chip.IO_ADDR_W) + CONFIG_HINAND_PAGE_SIZE / 2, 
        data->buffer + CONFIG_HINAND_PAGE_SIZE, CONFIG_HINAND_OOB_SIZE / 2);

    hinand_ecc_encipher(data);
    hinand_writel(data, data->hinand_address_buf[0] & 0xffff0000, HINAND_NFC_ADDRL);
    if(data->hinand_address_cycle > HINAND_ADDRESS_CYCLE_MASK)
    {
    	hinand_writel(data, data->hinand_address_buf[1], HINAND_NFC_ADDRH);
    }

    hinand_writel(data, NAND_CMD_PAGEPROG << 8 | HINAND_STATUS_CMD | NAND_CMD_SEQIN, HINAND_NFC_CMD);
    hinand_writel(data, HINAND_NFC_CON_ECC_DISABLE, HINAND_NFC_CON);
    hinand_writel(data,  
    		BIT_HINAND_NFC_OP_CMD1_EN        | 
    		BIT_HINAND_NFC_OP_ADDR_EN        |
    		BIT_HINAND_NFC_OP_WRITE_DATA_EN  |
    		(data->hinand_address_cycle << BIT_HINAND_NFC_OP_ADDRESS_CYCLE_BIT),
    		HINAND_NFC_OP);

    while((hinand_readl( data, HINAND_NFC_STATUS ) & 0x1) == 0x0);

    memcpy((unsigned char *)(data->chip.IO_ADDR_W),
        data->buffer + CONFIG_HINAND_PAGE_SIZE/2, CONFIG_HINAND_PAGE_SIZE/2);
    memcpy((unsigned char *)(data->chip.IO_ADDR_W) + CONFIG_HINAND_PAGE_SIZE/2, 
        data->buffer + CONFIG_HINAND_PAGE_SIZE + CONFIG_HINAND_OOB_SIZE/2, CONFIG_HINAND_OOB_SIZE/2);

    hinand_ecc_encipher(data);

    hinand_writel(data, 0x1d, HINAND_NFC_OP);

    while((hinand_readl( data, HINAND_NFC_STATUS ) & 0x1) == 0x0);

    data->hinand_address_cycle = 0x0;
}

/*
 * hardware specific funtion for controlling .
 */
static void hinand_cmd_ctrl(struct mtd_info *mtd, int dat,
		unsigned int ctrl)
{
	unsigned int hinand_address = 0, hinand_address_offset;
	struct nand_chip *chip = mtd->priv;
	struct hinand_data *data = chip->priv;

    debug("%s ctrl 0x%x command 0x%x page offset %d\n", __FUNCTION__, ctrl, dat, data->page_offset);

	if( ctrl & NAND_ALE )
	{
		if(ctrl & NAND_CTRL_CHANGE)
		{
			data->hinand_address_cycle = 0x0;
			data->hinand_address_buf[0] = 0x0;
			data->hinand_address_buf[1] = 0x0;
		}
		hinand_address_offset =  data->hinand_address_cycle << 3;

		if(data->hinand_address_cycle >= HINAND_ADDRESS_CYCLE_MASK)
		{
			hinand_address_offset = (data->hinand_address_cycle - HINAND_ADDRESS_CYCLE_MASK) << 3;
			hinand_address = 1;
		}

		data->hinand_address_buf[hinand_address] |= ((dat & 0xff) << hinand_address_offset);

		data->hinand_address_cycle ++;

	}

	if( ( ctrl & NAND_CLE ) && ( ctrl & NAND_CTRL_CHANGE ) )
	{
		data->command = dat & 0xff;
		switch(data->command) {
			case NAND_CMD_PAGEPROG:
                if(CONFIG_HINAND_PAGE_SIZE == 2048)
                {
                    hinand_program_2k(data);
                }
                else if(CONFIG_HINAND_PAGE_SIZE == 4096)
                {
                    hinand_program_4k(data);
                }
				break;

			case NAND_CMD_READSTART: 
                if(CONFIG_HINAND_PAGE_SIZE == 2048)
                {
                    hinand_read_2k(data);
                }
                else if(CONFIG_HINAND_PAGE_SIZE == 4096)
                {
                    hinand_read_4k(data);
                }
				break;

			case NAND_CMD_ERASE2:
        		hinand_writel(data, data->hinand_address_buf[0], HINAND_NFC_ADDRL);
        		if(data->hinand_address_cycle > HINAND_ADDRESS_CYCLE_MASK)
        		{
        			hinand_writel(data, data->hinand_address_buf[1], HINAND_NFC_ADDRH);
        		}

				hinand_writel(data, (data->command << 8) | HINAND_STATUS_CMD | NAND_CMD_ERASE1, HINAND_NFC_CMD);
				hinand_writel(data, HINAND_NFC_CON_ECC_DISABLE, HINAND_NFC_CON);
				hinand_writel(data,
						BIT_HINAND_NFC_OP_READ_STATUS_EN | 
						BIT_HINAND_NFC_OP_WAIT_READY_EN  | 
						BIT_HINAND_NFC_OP_CMD2_EN        |
						BIT_HINAND_NFC_OP_CMD1_EN        |
						BIT_HINAND_NFC_OP_ADDR_EN        |
						(data->hinand_address_cycle << BIT_HINAND_NFC_OP_ADDRESS_CYCLE_BIT),
						HINAND_NFC_OP);
        		while((hinand_readl( data, HINAND_NFC_STATUS ) & 0x1) == 0x0);
				break;

			case NAND_CMD_READID:

				hinand_writel(data, data->command, HINAND_NFC_CMD);
				hinand_writel(data, HINAND_NFC_CON_ECC_DISABLE, HINAND_NFC_CON);
				hinand_writel(data,  
						BIT_HINAND_NFC_OP_CMD1_EN       | 
                        BIT_HINAND_NFC_OP_ADDR_EN       |
                        BIT_HINAND_NFC_OP_READ_DATA_EN  |
						BIT_HINAND_NFC_OP_WAIT_READY_EN | 
						(1 << BIT_HINAND_NFC_OP_ADDRESS_CYCLE_BIT),
						HINAND_NFC_OP);
				data->hinand_address_cycle = 0x0;

        		while((hinand_readl( data, HINAND_NFC_STATUS ) & 0x1) == 0x0);
                if(CONFIG_HINAND_PAGE_SIZE == 4096)
                {
                    memcpy(data->buffer, (unsigned char *)(chip->IO_ADDR_R), 0x10);
                }
				break;
	
			case NAND_CMD_STATUS:

				hinand_writel(data, HINAND_STATUS_CMD, HINAND_NFC_CMD);
				hinand_writel(data, HINAND_NFC_CON_ECC_DISABLE, HINAND_NFC_CON);
				hinand_writel(data,
						BIT_HINAND_NFC_OP_READ_STATUS_EN | 
						BIT_HINAND_NFC_OP_WAIT_READY_EN ,
						HINAND_NFC_OP);
        		while((hinand_readl( data, HINAND_NFC_STATUS ) & 0x1) == 0x0);
				break;

			case NAND_CMD_SEQIN:
			case NAND_CMD_ERASE1:
			case NAND_CMD_READ0:
				break;
				
			default :
				break;
		}
	}

	if( (dat == NAND_CMD_NONE) && data->hinand_address_cycle)
	{
		if(data->command == NAND_CMD_SEQIN || data->command == NAND_CMD_READ0)
		{
			data->page_offset = 0x0;
            data->column_addr = data->hinand_address_buf[0] & 0xffff;
		}

        debug("command 0x%x, address is 0x%x, 0x%x, cycle %d\n", data->command, 
            data->hinand_address_buf[0], data->hinand_address_buf[1], 
            data->hinand_address_cycle);

	}

}
/*
 * hinand_dev_ready()
 *
 * returns 0 if the nand is busy, 1 if it is ready
 */
static int hinand_dev_ready(struct mtd_info *mtd)
{
	return 0x1;
}

static void hinand_select_chip(struct mtd_info *mtd, int chip)
{
	struct nand_chip *_chip = mtd->priv;
	struct hinand_data *data = _chip->priv;

	if (chip > 4)
	{
		BUG();
	}

	if (chip < 0)
	{
		return;
	}

	hinand_set_cs(data, chip);
	while((hinand_readl( data, HINAND_NFC_STATUS ) & 0x1) == 0x0);
}

static uint8_t hinand_read_byte(struct mtd_info *mtd)
{
	struct nand_chip *chip = mtd->priv;
	unsigned int reg;
	struct hinand_data *data = chip->priv;
	if(data->command == NAND_CMD_STATUS)
	{
		reg = ( ( hinand_readl(data, HINAND_NFC_STATUS) >> 0x5 ) & 0xff );
		return reg;
	}

    if(CONFIG_HINAND_PAGE_SIZE == 2048)
    {
    	reg = readb(chip->IO_ADDR_R + data->column_addr + data->page_offset);
    }
    else if(CONFIG_HINAND_PAGE_SIZE == 4096)
    {
    	reg = readb(data->buffer + data->column_addr + data->page_offset);
    }

    debug("%s 0x%x 0x%x 0x%x\n", __FUNCTION__, data->column_addr, data->page_offset, reg);
    
    data->page_offset ++;
    
	return reg;
}

static u16 hinand_read_word(struct mtd_info *mtd)
{
	struct nand_chip *chip = mtd->priv;
	struct hinand_data *data = chip->priv;
	u16    read_data = 0;

    if(CONFIG_HINAND_PAGE_SIZE == 2048)
    {
    	read_data = readw(chip->IO_ADDR_R + data->column_addr + data->page_offset);
    }
    else if(CONFIG_HINAND_PAGE_SIZE == 4096)
    {
    	read_data = readw(data->buffer + data->column_addr + data->page_offset);
    }

	data->page_offset += 2;

    debug("%s column %d page offset %d, data 0x%x\n", __FUNCTION__, 
        data->column_addr, data->page_offset, read_data);

	return read_data;
}

static void hinand_read_buf(struct mtd_info *mtd, uint8_t *buf, int len)
{
	struct nand_chip *chip = mtd->priv;
	struct hinand_data *data = chip->priv;

    debug("%s column %d page offset %d, len %d\n", __FUNCTION__, 
        data->column_addr, data->page_offset, len);

    if(CONFIG_HINAND_PAGE_SIZE == 2048)
    {
    	memcpy(buf, (unsigned char *)(chip->IO_ADDR_R  + data->column_addr + data->page_offset), len);
        
    }
    else if(CONFIG_HINAND_PAGE_SIZE == 4096)
    {
    	memcpy(buf, data->buffer + data->column_addr + data->page_offset, len);
    }

	data->page_offset += len;
}

static void hinand_write_buf(struct mtd_info *mtd, const uint8_t *buf, int len)
{
	struct nand_chip *chip = mtd->priv;
	struct hinand_data *data = chip->priv;

    debug("%s column %d page offset %d, len %d\n", __FUNCTION__, 
        data->column_addr, data->page_offset, len);

    if(CONFIG_HINAND_PAGE_SIZE == 2048)
    {
    	memcpy((unsigned char *)(chip->IO_ADDR_W + data->column_addr + data->page_offset), buf, len);
    }
    else if(CONFIG_HINAND_PAGE_SIZE == 4096)
    {
    	memcpy(data->buffer + data->column_addr + data->page_offset, buf, len);
    }

	data->page_offset += len;
}

static int hinand_probe(struct device* dev)
{
   	struct platform_device *pdev = to_platform_device(dev);
	struct hinand_data *data;
	int res = 0;

    debug("%s\n", __FUNCTION__);

	/* Allocate memory for the device structure (and zero it) */
	data = kzalloc(sizeof(struct hinand_data), GFP_KERNEL);
	if (!data) {
		dev_err(&pdev->dev, "failed to allocate device structure.\n");
		return -ENOMEM;
	}
    
	data->iobase = ioremap_nocache(pdev->resource[0].start,
					pdev->resource[0].end - pdev->resource[0].start + 1);
	if (data->iobase == NULL) 
	{
		dev_err(&pdev->dev, "ioremap failed\n");
		kfree(data);
		return -EIO;
	}

	data->chip.IO_ADDR_R = ioremap_nocache(pdev->resource[1].start,
					pdev->resource[1].end - pdev->resource[1].start + 1);
	if (data->chip.IO_ADDR_R == NULL) {
		dev_err(&pdev->dev, "ioremap failed\n");
		iounmap(data->iobase);
		kfree(data);
		return -EIO;
	}
    data->chip.IO_ADDR_W = data->chip.IO_ADDR_R;
	dev->driver_data = data;
	data->chip.priv = data;
	data->mtd.priv = &data->chip;
	data->mtd.name = (char*)(pdev->name);
	data->mtd.owner = THIS_MODULE;
	data->chip.cmd_ctrl = hinand_cmd_ctrl;
	data->chip.dev_ready = hinand_dev_ready;
	data->chip.select_chip = hinand_select_chip;
	data->chip.chip_delay = CHIP_DELAY;
	data->chip.options |= NAND_NO_AUTOINCR;
	data->chip.read_byte = hinand_read_byte;
	data->chip.read_word = hinand_read_word;
	data->chip.write_buf = hinand_write_buf;
	data->chip.read_buf = hinand_read_buf;

    if(CONFIG_HINAND_PAGE_SIZE == 2048)
    {
    	data->chip.ecc.layout = &hinand_oob_64_1bit;
    }
    else if(CONFIG_HINAND_PAGE_SIZE == 4096)
    {
    	data->chip.ecc.layout = &hinand_oob_128_4bit;
    }
    
	data->chip.ecc.mode = NAND_ECC_NONE;
	data->hinand_address_cycle = 0;
	data->hinand_address_buf[0] = 0;
	data->hinand_address_buf[1] = 0;
    data->buffer = kmalloc(HINAND_BUFFER_SIZE*2, GFP_KERNEL);
    memset(data->chip.IO_ADDR_W, 0xff, HINAND_BUFFER_SIZE);

    if(CONFIG_HINAND_PAGE_SIZE == 4096)
    {
        data->buffer = kmalloc(HINAND_BUFFER_SIZE*2, GFP_KERNEL);
        memset(data->buffer, 0xff, HINAND_BUFFER_SIZE*2);
    }
	hinand_init(&data->mtd);
	
	if (nand_scan(&data->mtd, 1)) {
		res = -ENXIO;
		goto out;
	}

	/* partitions should match sector boundaries; and it may be good to
	 * use readonly partitions for writeprotected sectors (BP2..BP0).
	 */
	if (mtd_has_partitions()) {

    	struct mtd_partition	*parts = NULL;
    	int			nr_parts = 0, i;

#ifdef CONFIG_MTD_CMDLINE_PARTS
    	static const char *part_probes[] = { "cmdlinepart", NULL, };

    	nr_parts = parse_mtd_partitions(&data->mtd,
    			part_probes, &parts, 0);
#endif

    	if (nr_parts > 0) {
    		for (i = 0; i < nr_parts; i++) {
    			DEBUG(MTD_DEBUG_LEVEL2, "partitions[%d] = "
    				"{.name = %s, .offset = 0x%.8x, "
    					".size = 0x%.8x (%uKiB) }\n",
    				i, parts[i].name,
    				parts[i].offset,
    				parts[i].size,
    				parts[i].size / 1024);
    		}
            printk("%s %d", __FUNCTION__, __LINE__);

    		return add_mtd_partitions(&data->mtd, parts, nr_parts);
    	}
	}
    
    printk("%s %d", __FUNCTION__, __LINE__);
    
 	return add_mtd_device(&data->mtd) == 1 ? -ENODEV : 0;
    
out:
	nand_release(&data->mtd);

	dev->driver_data = NULL;

	iounmap(data->iobase);

	iounmap(data->chip.IO_ADDR_W);

	kfree(data);

	return res;
}

int hinand_remove(struct device *dev)
{
	struct hinand_data *data = dev->driver_data;
    
    nand_release(&data->mtd);
    iounmap(data->iobase);
    iounmap(data->chip.IO_ADDR_W);
    dev->driver_data = NULL;
    kfree(data);

    return 0;
}
static void hinand_platdev_release (struct device *dev)
{
    /* These don't need to do anything because the pdev structures are
     * statically allocated. */
}

static struct device_driver hinand_driver =
{
	.name   = "hinand",
	.bus    = &platform_bus_type,
	.probe  = hinand_probe,
	.remove = hinand_remove,
};

static struct resource hinand_resources[] = {
	[0] = {
		.start  = HI_NAND_BASE_VIRT_REG_ADDR,
		.end    = HI_NAND_BASE_VIRT_REG_ADDR + HI_NAND_BASE_VIRT_REG_LEN,
		.flags  = IORESOURCE_MEM,
	},
	[1] = {
		.start  = HI_NAND_BASE_VIRT_MEM_ADDR,
		.end    = HI_NAND_BASE_VIRT_MEM_ADDR + HI_NAND_BASE_VIRT_MEM_LEN,
		.flags  = IORESOURCE_MEM,
	},
};
static struct platform_device hinand_pdev =
{
	.name           = "hinand",
	.id             = -1,
	.dev                   = {
        .platform_data     = NULL,
        .dma_mask          = (u64*)~0,
        .coherent_dma_mask = (u64) ~0,
        .release           = hinand_platdev_release,
    },
	.num_resources  = ARRAY_SIZE(hinand_resources),
	.resource       = hinand_resources,
};
/*
 * Main initialization routine
 */
static int hinand_module_init(void)
{
	int retval;

//	printk("hinand_module_init\n");
	retval = driver_register (&hinand_driver);
	if (retval < 0)
	{
			return retval;
	}

	retval = platform_device_register (&hinand_pdev);
	if (retval < 0)
	{
			driver_unregister(&hinand_driver);
			return retval;
	}

	return retval;
}

static void __exit hinand_module_exit (void)
{
	driver_unregister (&hinand_driver);
	platform_device_unregister (&hinand_pdev);
}

module_init(hinand_module_init);
module_exit(hinand_module_exit);
MODULE_LICENSE ("GPL");
MODULE_AUTHOR ("Zhang Kuanhuai");
MODULE_DESCRIPTION ("Hisilicon NAND controller driver code");
