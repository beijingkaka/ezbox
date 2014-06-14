/* ###########################################################################
# (c) Copyright Conexant Systems, Inc. 2004
#
# Conexant Systems, Inc. Confidential and Proprietary
#
# The following software source code ("Software") is strictly confidential and
# is proprietary to Conexant Systems, Inc. and its subsidiaries (collectively,
# "Conexant"). It may only be read, used, copied, adapted, modified or 
# otherwise dealt with by you if you have entered into a license agreement 
# with Conexant and then subject to the terms of that agreement and any other 
# applicable confidentiality agreement between you and Conexant.
#
# If you are in any doubt as to whether you are entitled to access, read,
# use, copy, adapt, modify or otherwise deal with the Software or whether you
# are entitled to disclose the Software to any other person you should
# contact Conexant.  If you have not entered into a license agreement
# with Conexant granting access to this Software you should forthwith return
# all media, copies and printed listings containing the Software to Conexant.
#
# Conexant reserves the right to take legal action against you should you
# breach the above provisions.
#
# If you are unsure, or to report violations, please contact
# licensee.support@conexant.com
# ##########################################################################*/

/*******************************************************************************
*
*                              I N C L U D E   F I L E S
*
*******************************************************************************/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/interrupt.h>
#include <linux/cdev.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/semaphore.h>
#include <asm/arch/irqs.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <stddef.h>
#include "asm-arm/arch-solos/hardware/solosw_gpio.h"
#include "asm-arm/arch-solos/hardware/solosw_gpio_if.h"
#include <linux/device.h>

/*******************************************************************************
*
*                            L O C A L   C O N S T A N T S
*
*******************************************************************************/
#define SOLOSW_GPIO_NAME     		"SOLOSW_GPIO"
#define SOLOSW_GPIO_IOC_MAGIC		'Z'

#define SOLOSW_GPIO_INPUT		_IOW(SOLOSW_GPIO_IOC_MAGIC, 1,solosw_pin_t)
#define SOLOSW_GPIO_OUTPUT	_IOW(SOLOSW_GPIO_IOC_MAGIC, 2,solosw_pin_t)
#define SOLOSW_GPIO_SET		_IOW(SOLOSW_GPIO_IOC_MAGIC, 3,solosw_pin_t)
#define SOLOSW_GPIO_CLEAR		_IOW(SOLOSW_GPIO_IOC_MAGIC, 4,solosw_pin_t)
#define SOLOSW_GPIO_SET_MODE	_IOW(SOLOSW_GPIO_IOC_MAGIC, 5,solosw_pin_t)
#define SOLOSW_GPIO_GET_MODE	_IOR(SOLOSW_GPIO_IOC_MAGIC, 6,solosw_pin_t)

#ifndef SOLOSW_GPIO_SYSFS_IF
	#define SOLOSW_GPIO_SYSFS_IF 1
#endif	
static char * solosw_gpio_modename[SOLOSW_GPIO_NUM_MODES+1] = 
{
   "",
    "INPUT",       
    "INVINPUT",   
    "FUNCTION0", 
    "FUNCTION1",
    "FUNCTION2", 
    "FUNCTION3",
    "FUNCTION4",
    "FUNCTION5",
    "FUNCTION6",
    "RESERVED1",
    "RESERVED2",
    "OUTPUT",   
    "INVOUTPUT", 
    "OPENDRAIN",
    "OPENSOURCE"
};

#if SOLOSW_GPIO_SYSFS_IF
/* Prevent code redundancy, create a macro for solosw_show_* functions. */
#define solosw_gpio_show_function(attr_name)		\
static ssize_t solosw_gpio_show_gpio##attr_name(struct device *dev, char *buf)	\
{									\
	U32 gpioNum = attr_name;\
	U32 mode;\
	U32 val;\
	solosw_gpio_get_mode(gpioNum,&mode);\
	PRINTK(KERN_WARNING "mode of GPIO%d is %s \n", gpioNum, solosw_gpio_modename[mode]); \
	if((mode == INPUT) ||(mode == INVINPUT))\
		val = solosw_gpio_check_input(gpioNum);\
	else\
		val = 0;\
	sprintf(buf,"mode = %s, val = %d\n",solosw_gpio_modename[mode],val);\
	return strlen(buf);\
}

#define solosw_gpio_store_function(attr_name)		\
static ssize_t solosw_gpio_store_gpio##attr_name(struct device *dev, const char *buf, size_t count)	\
{ \
	PRINTK(KERN_WARNING "Permission Denied !!! \n"); \
	return count;\
}

/* All of our attributes are read attributes. */
#define solosw_gpio_dev_rd_attr(attr_name)		\
	solosw_gpio_show_function(attr_name)		\
	solosw_gpio_store_function(attr_name)                    \
static DEVICE_ATTR(gpio##attr_name, S_IRWXUGO, solosw_gpio_show_gpio##attr_name, solosw_gpio_store_gpio##attr_name)

solosw_gpio_dev_rd_attr (0);
solosw_gpio_dev_rd_attr (1);
solosw_gpio_dev_rd_attr (2);
solosw_gpio_dev_rd_attr (3);
solosw_gpio_dev_rd_attr (4);
solosw_gpio_dev_rd_attr (5);
solosw_gpio_dev_rd_attr (6);
solosw_gpio_dev_rd_attr (7);
solosw_gpio_dev_rd_attr (8);
solosw_gpio_dev_rd_attr (9);
solosw_gpio_dev_rd_attr (10);
solosw_gpio_dev_rd_attr (11);
solosw_gpio_dev_rd_attr (12);
solosw_gpio_dev_rd_attr (13);
solosw_gpio_dev_rd_attr (14);
solosw_gpio_dev_rd_attr (15);
solosw_gpio_dev_rd_attr (16);
solosw_gpio_dev_rd_attr (17);
#endif

#if CONFIG_PROC_FS
	#define PROC_DIR	"gpio_solosw"
	#define PROC_DEV	"gpiodev"
	#define PROC_FILE	"gpiofile"
#endif
/*******************************************************************************
*
*                               L O C A L   T Y P E S
*
*******************************************************************************/
typedef struct pin 
{
       U32 num;
       U32 val;
} solosw_pin_t;

typedef struct _SOLOSW_GPIO_DEVICE_DATA 
{
	int nr_registered_attrs;
	int device_registered;

} SOLOSW_GPIO_DEVICE_DATA;

solosw_gpio_mode_t defalut_solosw_gpio_modes[]=
{
	SOLOSW_GPIO0_MODE,
	SOLOSW_GPIO1_MODE,	
	SOLOSW_GPIO2_MODE,	
	SOLOSW_GPIO3_MODE,	
	SOLOSW_GPIO4_MODE,	
	SOLOSW_GPIO5_MODE,	
	SOLOSW_GPIO6_MODE,	
	SOLOSW_GPIO7_MODE,	
	SOLOSW_GPIO8_MODE,	
	SOLOSW_GPIO9_MODE,	
	SOLOSW_GPIO10_MODE,	
	SOLOSW_GPIO11_MODE,
	SOLOSW_GPIO12_MODE,
	SOLOSW_GPIO13_MODE,	
	SOLOSW_GPIO14_MODE,	
	SOLOSW_GPIO15_MODE,	
	SOLOSW_GPIO16_MODE,	
	SOLOSW_GPIO17_MODE
};

/*******************************************************************************
*
*                            L O C A L   P R O T O T Y P E S
*
*******************************************************************************/
static void __exit solosw_gpio_exit(void);

#if SOLOSW_GPIO_SYSFS_IF
	void solosw_gpio_sysfs_if_init(void);
	void solosw_gpio_sysfs_if_exit(void);
#endif

#if CONFIG_PROC_FS
	static int solosw_gpio_proc_if_init( void );
	static void solosw_gpio_proc_if_exit( void );
#endif

static int solosw_gpio_open(struct inode * inode, struct file * filp);
static int solosw_gpio_release(struct inode * inode, struct file * filp);
static int solosw_gpio_read(struct file *filp, char __user *buff,size_t count, loff_t *offp);
static int solosw_gpio_write(struct file *filp, const char __user *buff,size_t count, loff_t *offp);
static int solosw_gpio_ioctl(struct inode *inode, struct file * file, unsigned int cmd, unsigned long arg);

/*******************************************************************************
*
*                             L O C A L   V A R I A B L E S
*
*******************************************************************************/
U32 sw_gpio_nr_devs 	= 1;
U32 sw_gpio_major 		= 0;
U32 sw_gpio_minor 		= 0;

SOLOSW_GPIO_DEVICE_DATA sw_gpio_devdata;

struct cdev	*sw_gpio_cdev;

static struct file_operations sw_gpio_fops = 
{
       .owner	= THIS_MODULE,
       .open	= solosw_gpio_open,
       .read	= solosw_gpio_read,
       .write 	= solosw_gpio_write,
       .ioctl		= solosw_gpio_ioctl,
       .release	= solosw_gpio_release,
};

#if SOLOSW_GPIO_SYSFS_IF
struct device sw_gpio_device;

static struct device_attribute * const sw_gpio_dev_attrs[] = 
{
	&dev_attr_gpio0,
	&dev_attr_gpio1,	/* FIXME */
	&dev_attr_gpio2,
	&dev_attr_gpio3,
	&dev_attr_gpio4,
	&dev_attr_gpio5,
	&dev_attr_gpio6,
	&dev_attr_gpio7,
	&dev_attr_gpio8,
	&dev_attr_gpio9,
	&dev_attr_gpio10,
	&dev_attr_gpio11,
	&dev_attr_gpio12,
	&dev_attr_gpio13,
	&dev_attr_gpio14,
	&dev_attr_gpio15,
	&dev_attr_gpio16,
	&dev_attr_gpio17,
};
#endif

#if CONFIG_PROC_FS
	static struct proc_dir_entry *solosw_gpio_dir, *solosw_gpio_dev, *solosw_gpio_file;
#endif

/* This file-global variable is accessed by solosw_gpio_check_reserved() and solosw_gpio_reserve_pin() */
static U32 sw_gpio_reserve = 0;

/*******************************************************************************
*
*                              L O C A L   M E T H O D S
*
*******************************************************************************/
static int solosw_gpio_open(struct inode * inode, struct file * filp)
{
       return 0;
}

static int solosw_gpio_release(struct inode * inode, struct file * filp)
{
       return 0;
}

static int solosw_gpio_read(struct file *filp, char __user *buff,size_t count, loff_t *offp)
{     
	unsigned m = iminor(filp->f_dentry->d_inode);
	char  data;
	
	PRINTK(KERN_WARNING "Solosw gpio read\n");	

	if(buff == NULL)
		return  -EFAULT;	/* FIXME */
	
	data = (char)solosw_gpio_check_input(m);

	return copy_to_user(buff, &data, sizeof(data));	
}

static int solosw_gpio_write(struct file *filp, const char __user *buff,size_t count, loff_t *offp)
{	
       unsigned m = iminor(filp->f_dentry->d_inode);
       char c;

	PRINTK(KERN_WARNING "Solosw gpio write\n");	

	if(buff == NULL)
		return  -EFAULT;	/* FIXME */
	
	if (get_user(c, buff))
		return -EFAULT;

	switch(c)
	{
		case '0':	 solosw_gpio_deassert_output(m);
			 break;
		case '1':	 solosw_gpio_assert_output(m);
			 break;
		default:
			printk(KERN_WARNING "Invalid data passed , valid values are 0-1 \n");	
			return  -EFAULT;        /* FIXME */
	}

       return count;
}

static int solosw_gpio_ioctl(struct inode *inode, struct file * file, unsigned int cmd, unsigned long arg)
{
       solosw_pin_t pin;

   	PRINTK(KERN_WARNING "Solosw gpio ioctl\n");	

       if (copy_from_user(&pin, (solosw_pin_t*)arg, sizeof(solosw_pin_t)))
               return -EFAULT;
       
       switch (cmd) 
	{
               case SOLOSW_GPIO_SET_MODE:
                       if (solosw_gpio_set_mode(pin.num, pin.val ) !=  0)
                               return -EINVAL;
                       break;
					   
               case SOLOSW_GPIO_GET_MODE:
                       if (solosw_gpio_get_mode(pin.num, &(pin.val) ) !=  0) 	
                               return -EINVAL;
			  copy_to_user((solosw_pin_t*)arg, &pin, sizeof(solosw_pin_t));
                       break;
					   
               case SOLOSW_GPIO_INPUT:
                       pin.val = solosw_gpio_check_input(pin.num);
			  copy_to_user((solosw_pin_t*)arg, &pin, sizeof(solosw_pin_t));		   
                       break;
					   
               case SOLOSW_GPIO_OUTPUT:
			  if(pin.val == 1)
			  	solosw_gpio_assert_output_slow(pin.num);
			  else
			  	solosw_gpio_deassert_output_slow(pin.num);
                       break;
					   
               case SOLOSW_GPIO_SET:
                     solosw_gpio_assert_output(pin.num); 
                       break;        
					   
               case SOLOSW_GPIO_CLEAR:
                     solosw_gpio_deassert_output(pin.num);
                       break;		  
					   
               default:
                       return -ENOTTY;  //  .inappropriate ioctl for device,.       
       }

       return 0;
}

#if SOLOSW_GPIO_SYSFS_IF
void solosw_gpio_sysfs_if_init()
{
	SOLOSW_GPIO_DEVICE_DATA* pDrvData = &sw_gpio_devdata;
	int i;

	PRINTK(KERN_WARNING "Solosw sysfs init\n");

	memset(&sw_gpio_device, 0, sizeof (struct device));
	snprintf(sw_gpio_device.bus_id, BUS_ID_SIZE, "solosw_gpio");

	if (device_register(&sw_gpio_device))
		goto cleanup_error;
	
	pDrvData->device_registered = 1;

	for (i = 0; i < ARRAY_SIZE(sw_gpio_dev_attrs); i++) 
	{
		if(device_create_file(&sw_gpio_device, sw_gpio_dev_attrs[i])) 
		{
			goto cleanup_error;
		}
		pDrvData->nr_registered_attrs++;
	}

	return;

cleanup_error:
  	solosw_gpio_sysfs_if_exit(); /* clean up */

	return;

}

void solosw_gpio_sysfs_if_exit()
{
	SOLOSW_GPIO_DEVICE_DATA* pDrvData = &sw_gpio_devdata;
	int i;

	PRINTK(KERN_WARNING "Solosw sysfs exit\n");

	for (i = 0; i < pDrvData->nr_registered_attrs; i++)
		device_remove_file(&sw_gpio_device, sw_gpio_dev_attrs[i]);
	
	pDrvData->nr_registered_attrs = 0;

	if (pDrvData->device_registered) 
	{
		device_unregister(&sw_gpio_device);
		pDrvData->device_registered = 0;
	}
}
#endif

#if CONFIG_PROC_FS
static int solosw_proc_output (char *buf)
{
	char *p;
	unsigned i = 0;
	unsigned mode;
	
	p = buf;

	for (i =0; i < SOLOSW_GPIO_NUMS; i++)
	{
	 	solosw_gpio_get_mode(i, &mode);
		
		p += sprintf(p, "gpio%d\t  mode\t: %s",i, solosw_gpio_modename[mode]);

		if((mode == INPUT) || (mode == INVINPUT))
		{
			p += sprintf(p, "\t value \t: %d\n",solosw_gpio_check_input(i));
		}
		else
		{
			p += sprintf(p, "\t value : na\n");		
		}
	}
	
	return  p - buf;
}

static int solosw_read_proc(char *page, char **start, off_t off,int count, int *eof, void *data)
{
	int len = solosw_proc_output (page);
		
	if (len <= off+count) 
		*eof = 1;

	*start = page + off;
	len -= off;

	if (len>count) 
		len = count;

	if (len<0) 
		len = 0;

	return len;
}


static int __init solosw_gpio_proc_if_init( void )
{
	if(!create_proc_read_entry("gpio", 0, NULL, solosw_read_proc, NULL))
		PRINTK(KERN_WARNING "cound not create /gpio_solosw/gpio\n");
	 
		
#if 0
	int ret = 0;
	
	PRINTK(KERN_WARNING "Solosw proc_if init\n");

	/* Create "gpio_solosw" directory in /proc  */
	solosw_gpio_dir = proc_mkdir(PROC_DIR, NULL);

	if ( solosw_gpio_dir == NULL ) 
	{
		PRINTK(KERN_WARNING "can not create /proc/gpio_solosw  directory\n");
		ret = -ENOMEM;
		goto no_dir;
	}

	solosw_gpio_dir->owner = THIS_MODULE;

#if 0
	/* Create "gpiodev" device in /proc/gpio_solosw */
	solosw_gpio_dev = proc_mknod(PROC_DEV, S_IFCHR | 0666, solosw_gpio_dir, MKDEV(sw_gpio_major, sw_gpio_minor));

	if( solosw_gpio_dev == NULL )
	{
		PRINTK(KERN_WARNING "can not create /proc/gpio_solosw/gpiodev device  \n");
		ret = -ENOMEM;
		goto no_dev;
	}
	solosw_gpio_dev->owner = THIS_MODULE;
#else
	/* create "gpiofile" file in /proc/gpio_solosw */
	solosw_gpio_file = create_proc_entry(PROC_FILE, 0644, solosw_gpio_dir);

	if( solosw_gpio_file == NULL )
	{
		PRINTK(KERN_WARNING "can not create /proc/gpio_solosw/gpiofile file  \n");
		ret = -ENOMEM;
		goto no_file;
	}

	solosw_gpio_file->owner = THIS_MODULE;
	solosw_gpio_file->proc_fops = &sw_gpio_fops;

	return ret;

 #endif

#if 0
	no_dev:
		remove_proc_entry(PROC_DIR, NULL);
#else
	no_file:
		remove_proc_entry(PROC_DIR, NULL);
#endif
	no_dir:
		return ret;
#endif		
}

static void __exit solosw_gpio_proc_if_exit( void )
{
	PRINTK(KERN_WARNING "Solosw proc_if exit\n");
#if 0
	if( solosw_gpio_dev )
		remove_proc_entry(PROC_DEV, solosw_gpio_dir);
	
	if( solosw_gpio_dir ) 
		remove_proc_entry(PROC_DIR, NULL);
#endif
	remove_proc_entry ("gpio", NULL);
}
#endif

/*!
\brief Check if a pin has been reserved. 
\param GPIONum    32-bit unsigned    INPUT
\return 32-bit unsigned
  \retval SOLOSW_GPIO_PIN_FREE    : The pin is not reserved.
  \retval SOLOSW_GPIO_INVGPIO     : Invalid parameter (GPIONum > 63).
  \retval SOLOSW_GPIO_PIN_RESERVED: The pin is reserved.
*/
U32 solosw_gpio_check_reserved(U32 gpioNum){

  if(gpioNum >= SOLOSW_GPIO_NUMS)
    return -SOLOSW_GPIO_INVGPIO;

  if(sw_gpio_reserve & (1 << gpioNum ))
    return -SOLOSW_GPIO_PIN_RESERVED;

  return SOLOSW_GPIO_PIN_FREE;

}


/*!
\brief Mark a pin as reserved 
\param GPIONum    32-bit unsigned    INPUT
\return 32-bit unsigned
  \retval SOLOSW_GPIO_SUCCESS     : Success. 
  \retval SOLOSW_GPIO_INVGPIO     : Illegal GPIO number 
  \retval SOLOSW_GPIO_PIN_RESERVED: The pin is already reserved by another entity.
\note This function disables IRQs.
*/
U32 solosw_gpio_reserve_pin(U32 gpioNum){

  U32 check=SOLOSW_GPIO_PIN_RESERVED;
   
  if ( LOCK_SOLOSW_GPIO_FOR_WRITING )
  	return -ERESTARTSYS;

  if( (check = solosw_gpio_check_reserved(gpioNum)) != SOLOSW_GPIO_PIN_FREE ){

     UNLOCK_SOLOSW_GPIO_FOR_WRITING; 

     return check;
  }

  sw_gpio_reserve |= (1 << gpioNum);

  UNLOCK_SOLOSW_GPIO_FOR_WRITING; 

  return SOLOSW_GPIO_SUCCESS;

}


/*!
\brief Mark a pin as free (not reserved) 
\param GPIONum    32-bit unsigned    INPUT
\return 32-bit unsigned
  \retval SOLOSW_GPIO_SUCCESS : Success.
  \retval SOLOSW_GPIO_INVGPIO : Illegal GPIO number.
\note This function disables IRQs.
*/
  U32 solosw_gpio_release_pin(U32 gpioNum){

  if(gpioNum >= SOLOSW_GPIO_NUMS)
    return -SOLOSW_GPIO_INVGPIO;

  if ( LOCK_SOLOSW_GPIO_FOR_WRITING )
	return -ERESTARTSYS;

  sw_gpio_reserve &= ~(1 << gpioNum );

  UNLOCK_SOLOSW_GPIO_FOR_WRITING;

  return SOLOSW_GPIO_SUCCESS;

}

/* Set the default mode for all GPIOs */	
void solosw_gpio_set_default()
{
	int index = 0;

	for(index = 0; index < SOLOSW_GPIO_NUMS; index++)
	{
		solosw_gpio_set_mode(index, defalut_solosw_gpio_modes[index]);
	}
}

int __init solosw_gpio_init(void)
{
	dev_t dev;
	U32 result;

	PRINTK(KERN_WARNING "Solosw gpio int\n");

	INIT_SOLOSW_GPIO_LOCKING;
	
	/* Allocate the device number */
	if (sw_gpio_major) 
	{
		dev = MKDEV(sw_gpio_major, sw_gpio_minor);
		result = register_chrdev_region(dev, sw_gpio_nr_devs, "solos_gpio");
	} 
	else 
	{
		result = alloc_chrdev_region(&dev, sw_gpio_minor, sw_gpio_nr_devs, "solos_gpio");
		sw_gpio_major = MAJOR(dev);
	}
	
	if (result < 0) 
	{
		PRINTK(KERN_WARNING "Solosw gpio: can't get major %d\n", sw_gpio_major);
		return result;
	}

	/* register gpio driver */
	sw_gpio_cdev = cdev_alloc( );
	cdev_init(sw_gpio_cdev, &sw_gpio_fops);
	//sw_gpio_cdev.owner = THIS_MODULE;
	
	if ((result = cdev_add(sw_gpio_cdev, dev, 1))) 
	{
		kfree(sw_gpio_cdev);
	}

#if SOLOSW_GPIO_SYSFS_IF
	solosw_gpio_sysfs_if_init();
#endif

#if CONFIG_PROC_FS
	solosw_gpio_proc_if_init();
#endif

	solosw_gpio_set_default();

       return result;

}

static void __exit solosw_gpio_exit(void)
{
	PRINTK(KERN_WARNING "Solosw gpio exit\n");

	unregister_chrdev(sw_gpio_major, SOLOSW_GPIO_NAME);
     	cdev_del(sw_gpio_cdev);

#if SOLOSW_GPIO_SYSFS_IF
	solosw_gpio_sysfs_if_exit();
#endif

#if CONFIG_PROC_FS
	solosw_gpio_proc_if_exit();
#endif
}


MODULE_AUTHOR("Daya Shanker Patel");
MODULE_DESCRIPTION("SOLOSW GPIO driver");
MODULE_LICENSE("GPL");

module_init(solosw_gpio_init);
module_exit(solosw_gpio_exit);

