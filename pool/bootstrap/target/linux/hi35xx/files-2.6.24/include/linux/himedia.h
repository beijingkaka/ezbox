#ifndef _LINUX_HIMEDIA_DEVICE_H
#define _LINUX_HIMEDIA_DEVICE_H
#include <linux/module.h>
#include <linux/major.h>

#define HIMEDIA_DEVICE_MAJOR 218
#define HIMEDIA_DYNAMIC_MINOR 255

struct device;
struct class_device;

struct himedia_device  {
	int minor;
	const char *name;
	const struct file_operations *fops;
	struct list_head list;
	struct device *parent;
	struct device *this_device;
};

extern int himedia_register(struct himedia_device *);
extern int himedia_deregister(struct himedia_device *);

#define MODULE_ALIAS_HIMEDIA(minor)				\
	MODULE_ALIAS("himedia-char-major-" __stringify(HIMEDIA_DEVICE_MAJOR)	\
	"-" __stringify(minor))
#endif
