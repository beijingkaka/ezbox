/*
 * linux/drivers/char/himedia.c
 *
 * Generic misc open routine by Johan Myreen
 *
 * Based on code from Linus
 *
 * Teemu Rantanen's Microsoft Busmouse support and Derrick Cole's
 *   changes incorporated into 0.97pl4
 *   by Peter Cervasio (pete%q106fm.uucp@wupost.wustl.edu) (08SEP92)
 *   See busmouse.c for particulars.
 *
 * Made things a lot mode modular - easy to compile in just one or two
 * of the misc drivers, as they are now completely independent. Linus.
 *
 * Support for loadable modules. 8-Sep-95 Philip Blundell <pjb27@cam.ac.uk>
 *
 * Fixed a failing symbol register to free the device registration
 *		Alan Cox <alan@lxorguk.ukuu.org.uk> 21-Jan-96
 *
 * Dynamic minors and /proc/mice by Alessandro Rubini. 26-Mar-96
 *
 * Renamed to misc and miscdevice to be more accurate. Alan Cox 26-Mar-96
 *
 * Handling of mouse minor numbers for kerneld:
 *  Idea by Jacques Gelinas <jack@solucorp.qc.ca>,
 *  adapted by Bjorn Ekwall <bj0rn@blox.se>
 *  corrected by Alan Cox <alan@lxorguk.ukuu.org.uk>
 *
 * Changes for kmod (from kerneld):
 *	Cyrus Durgin <cider@speakeasy.org>
 *
 * Added devfs support. Richard Gooch <rgooch@atnf.csiro.au>  10-Jan-1998
 * Changed for hisilicon-media devices by Liu Jiandong 10-Dec-2007
 */

#include <linux/module.h>
#include <linux/autoconf.h>

#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/himedia.h>
#include <linux/kernel.h>
#include <linux/major.h>
#include <linux/slab.h>
#include <linux/mutex.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/stat.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/tty.h>
#include <linux/kmod.h>

/*
 * Head entry for the doubly linked himedia_device list
 */
static LIST_HEAD(himedia_list);
static DEFINE_MUTEX(himedia_sem);

/*
 * Assigned numbers, used for dynamic minors
 */
#define DYNAMIC_MINORS 64 /* like dynamic majors */
static unsigned char himedia_minors[DYNAMIC_MINORS / 8];

extern int pmu_device_init(void);

#ifdef CONFIG_PROC_FS
static void *himedia_seq_start(struct seq_file *seq, loff_t *pos)
{
	mutex_lock(&himedia_sem);
	return seq_list_start(&himedia_list, *pos);
}

static void *himedia_seq_next(struct seq_file *seq, void *v, loff_t *pos)
{
	return seq_list_next(v, &himedia_list, pos);
}

static void himedia_seq_stop(struct seq_file *seq, void *v)
{
	mutex_unlock(&himedia_sem);
}

static int himedia_seq_show(struct seq_file *seq, void *v)
{
	const struct himedia_device *p = list_entry(v, struct himedia_device, list);

	seq_printf(seq, "%3i %s\n", p->minor, p->name ? p->name : "");
	return 0;
}


static struct seq_operations himedia_seq_ops = {
	.start = himedia_seq_start,
	.next  = himedia_seq_next,
	.stop  = himedia_seq_stop,
	.show  = himedia_seq_show,
};

static int himedia_seq_open(struct inode *inode, struct file *file)
{
	return seq_open(file, &himedia_seq_ops);
}

static struct file_operations himedia_proc_fops = {
	.owner	 = THIS_MODULE,
	.open    = himedia_seq_open,
	.read    = seq_read,
	.llseek  = seq_lseek,
	.release = seq_release,
};
#endif

static int himedia_open(struct inode * inode, struct file * file)
{
	int minor = iminor(inode);
	struct himedia_device *c;
	int err = -ENODEV;
	const struct file_operations *old_fops, *new_fops = NULL;
	
	mutex_lock(&himedia_sem);
	
	list_for_each_entry(c, &himedia_list, list) {
		if (c->minor == minor) {
			new_fops = fops_get(c->fops);		
			break;
		}
	}
		
	if (!new_fops) {
		mutex_unlock(&himedia_sem);
		request_module("char-major-%d-%d", HIMEDIA_DEVICE_MAJOR, minor);
		mutex_lock(&himedia_sem);

		list_for_each_entry(c, &himedia_list, list) {
			if (c->minor == minor) {
				new_fops = fops_get(c->fops);
				break;
			}
		}
		if (!new_fops)
			goto fail;
	}

	err = 0;
	old_fops = file->f_op;
	file->f_op = new_fops;
	if (file->f_op->open) {
		err=file->f_op->open(inode,file);
		if (err) {
			fops_put(file->f_op);
			file->f_op = fops_get(old_fops);
		}
	}
	fops_put(old_fops);
fail:
	mutex_unlock(&himedia_sem);
	return err;
}

static struct class *himedia_class;

static struct file_operations himedia_fops = {
	.owner		= THIS_MODULE,
	.open		= himedia_open,
};


/**
 *	himedia_register	-	register a himedia device
 *	@himedia: device structure
 *	
 *	Register a himedia device with the kernel. If the minor
 *	number is set to %HIMEDIA_DYNAMIC_MINOR a minor number is assigned
 *	and placed in the minor field of the structure. For other cases
 *	the minor number requested is used.
 *
 *	The structure passed is linked into the kernel and may not be
 *	destroyed until it has been unregistered.
 *
 *	A zero is returned on success and a negative errno code for
 *	failure.
 */
 
int himedia_register(struct himedia_device * himedia)
{
	struct himedia_device *c;
	dev_t dev;
	int err = 0;

	INIT_LIST_HEAD(&himedia->list);

	mutex_lock(&himedia_sem);
	list_for_each_entry(c, &himedia_list, list) {
		if (c->minor == himedia->minor) {
			mutex_unlock(&himedia_sem);
			return -EBUSY;
		}
	}

	if (himedia->minor == HIMEDIA_DYNAMIC_MINOR) {
		int i = DYNAMIC_MINORS;
		while (--i >= 0)
			if ( (himedia_minors[i>>3] & (1 << (i&7))) == 0)
				break;
		if (i<0) {
			mutex_unlock(&himedia_sem);
			return -EBUSY;
		}
		himedia->minor = i;
	}

	if (himedia->minor < DYNAMIC_MINORS)
		himedia_minors[himedia->minor >> 3] |= 1 << (himedia->minor & 7);
	dev = MKDEV(HIMEDIA_DEVICE_MAJOR, himedia->minor);

	himedia->this_device = device_create(himedia_class, himedia->parent, dev,
					  "%s", himedia->name);
	if (IS_ERR(himedia->this_device)) {
		err = PTR_ERR(himedia->this_device);
		goto out;
	}

	/*
	 * Add it to the front, so that later devices can "override"
	 * earlier defaults
	 */
	list_add(&himedia->list, &himedia_list);
 out:
	mutex_unlock(&himedia_sem);
	return err;
}

/**
 *	himedia_deregister - unregister a himedia device
 *	@himedia: device to unregister
 *
 *	Unregister a himedia device that was previously
 *	successfully registered with himedia_register(). Success
 *	is indicated by a zero return, a negative errno code
 *	indicates an error.
 */

int himedia_deregister(struct himedia_device * himedia)
{
	int i = himedia->minor;

	if (list_empty(&himedia->list))
		return -EINVAL;

	mutex_lock(&himedia_sem);
	list_del(&himedia->list);
	device_destroy(himedia_class, MKDEV(HIMEDIA_DEVICE_MAJOR, himedia->minor));
	if (i < DYNAMIC_MINORS && i>0) {
		himedia_minors[i>>3] &= ~(1 << (himedia->minor & 7));
	}
	mutex_unlock(&himedia_sem);
	return 0;
}

EXPORT_SYMBOL(himedia_register);
EXPORT_SYMBOL(himedia_deregister);

static int __init himedia_init(void)
{
#ifdef CONFIG_PROC_FS
	struct proc_dir_entry *ent;

	ent = create_proc_entry("himedia", 0, NULL);
	if (ent)
		ent->proc_fops = &himedia_proc_fops;
#endif
	himedia_class = class_create(THIS_MODULE, "himedia");
	if (IS_ERR(himedia_class))
		return PTR_ERR(himedia_class);

	if (register_chrdev(HIMEDIA_DEVICE_MAJOR,"himedia",&himedia_fops)) {
		printk("unable to get major %d for himedia devices\n",
		       HIMEDIA_DEVICE_MAJOR);
		class_destroy(himedia_class);
		return -EIO;
	}
	return 0;
}
subsys_initcall(himedia_init);
