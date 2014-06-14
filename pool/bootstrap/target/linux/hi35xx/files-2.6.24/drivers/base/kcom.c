
#include <linux/autoconf.h>

#include <linux/module.h>
#include <linux/ioport.h>
#include <linux/init.h>
#include <linux/sysrq.h>
#include <linux/device.h>
#include <linux/vmalloc.h>
#include <linux/delay.h>
#include <linux/linkage.h>
#include <linux/errno.h>

#include <asm/io.h>
#include <asm/irq.h>
#include <linux/amba/bus.h>
#include <linux/amba/clcd.h>
#include <asm/hardware/arm_timer.h>
#include <asm/cacheflush.h>
#include <asm/memory.h>

#include <linux/kcom.h>

#define KCOM_PR_PREF "kcom"

#undef pr_debug
#define KCOM_DEBUG_LEVEL 255

#if KCOM_DEBUG_LEVEL==255
#define pr_debug(level, s...) 
#else
#define pr_debug(level, s...) do{ if((level)>=1){ printk(KERN_INFO "[%s, %d]: ", __FUNCTION__, __LINE__); printk( s );} }while(0)
#endif

#ifndef CONFIG_MODULE_UNLOAD
#define module_refcount(p) 1
#endif

#undef pr_error
#define pr_error(s...) do{ printk(KERN_ERR KCOM_PR_PREF "<%s>:", __FUNCTION__); printk(" " s); }while(0)

#define pr_struct(p, member, format) pr_debug(10, #member "	= " format"\n", (p)->member)

#define TRY_KCOM_MODULE_GET(__module) ({ struct module *__kcom_m_p = __module; int ret; \
	pr_debug(0, "befor module get, %s, %d\n", __kcom_m_p?__kcom_m_p->name:"NULL", \
			__kcom_m_p ? module_refcount(__kcom_m_p):0); \
	ret = try_module_get(__kcom_m_p); \
	pr_debug(0, "after module get, %s, %d\n", __kcom_m_p?__kcom_m_p->name:"NULL", \
			__kcom_m_p ? module_refcount(__kcom_m_p):0); ret; })

#define KCOM_MODULE_PUT(__module) do{ struct module *__kcom_m_p = __module; \
	pr_debug(0, "befor module put, %s, %d\n", __kcom_m_p?__kcom_m_p->name:"NULL", \
			__kcom_m_p ? module_refcount(__kcom_m_p):0); \
	module_put(__kcom_m_p); \
	pr_debug(0, "after module put, %s, %d\n", __kcom_m_p?__kcom_m_p->name:"NULL", \
			__kcom_m_p ? module_refcount(__kcom_m_p):0); }while(0)


static void kcom_request_sync(void);

#define to_kcom_obj_attr(_attr) container_of(_attr, struct kcom_obj_attribute, attr)
#define to_kcom_obj(obj) container_of(obj, struct kcom_object, kobj)

static ssize_t kcom_obj_attr_show(struct kobject * kobj, struct attribute * attr, char * buf)
{
	struct kcom_obj_attribute * kcom_obj_attr = to_kcom_obj_attr(attr);
	struct kcom_object * kcom_obj = to_kcom_obj(kobj);
	ssize_t ret = -EIO;

	if (kcom_obj_attr->show)
		ret = kcom_obj_attr->show(kcom_obj, buf);

	return ret;
}


static ssize_t kcom_obj_attr_store(struct kobject * kobj, struct attribute * attr, const char * buf, size_t count)
{
	struct kcom_obj_attribute * kcom_obj_attr = to_kcom_obj_attr(attr);
	struct kcom_object * kcom_obj = to_kcom_obj(kobj);
	ssize_t ret = -EIO;

	if (kcom_obj_attr->store)
		ret = kcom_obj_attr->store(kcom_obj, buf, count);

	return ret;
}

static void kcom_obj_release(struct kobject * kobj)
{
	struct kcom_object * kcom_obj = to_kcom_obj(kobj);
	
	if(kcom_obj->release)
		kcom_obj->release(kcom_obj);
	else
		pr_debug(100, "object '%s<%s>' dose not have a release() function.\n",
				kcom_obj->kobj.name, kcom_obj->uuid);
}

static struct sysfs_ops kcom_obj_sysfs_ops = {
	.show   = kcom_obj_attr_show,
	.store  = kcom_obj_attr_store,
};

static struct kobj_type ktype_kcom_obj = {
	.sysfs_ops      = &kcom_obj_sysfs_ops,
	.release        = kcom_obj_release,
};

static DECLARE_RWSEM(kcom_rw_sem);
static decl_subsys(kcom, &ktype_kcom_obj, NULL);

int kcom_create_file(struct kcom_object *kcom, struct kcom_obj_attribute *attr)
{
	return sysfs_create_file(&kcom->kobj, &attr->attr);
}

EXPORT_SYMBOL(kcom_create_file);

void kcom_remove_file(struct kcom_object *kcom, struct kcom_obj_attribute *attr)
{
	sysfs_remove_file(&kcom->kobj, &attr->attr);
}

EXPORT_SYMBOL(kcom_remove_file);

static ssize_t kcom_obj_show_uuid(struct kcom_object *kcom, char *buf)
{
	return sprintf(buf, "%s\n", kcom->uuid);
}

static struct kcom_obj_attribute kcom_attr_uuid = __ATTR(uuid, 0444, kcom_obj_show_uuid, NULL);

static ssize_t kcom_obj_show_type(struct kcom_object *kcom, char *buf)
{
	int len;

	switch(kcom->ta.type)
	{
		case KCOM_TYPE_CLASS:
			len = sprintf(buf, "%s\n", "class");
			break;

		default:
			len = sprintf(buf, "%s\n", "object");
	}

	return len;
}

static struct kcom_obj_attribute kcom_attr_type = __ATTR(type, 0444, kcom_obj_show_type, NULL);

static ssize_t kcom_obj_show_ref(struct kcom_object *kcom, char *buf)
{
	return sprintf(buf, "%d\n", atomic_read(&kcom->ref));
}

static struct kcom_obj_attribute kcom_attr_ref = __ATTR(ref, 0444, kcom_obj_show_ref, NULL);

static ssize_t kcom_obj_show_kref(struct kcom_object *kcom, char *buf)
{
	return sprintf(buf, "%d\n", atomic_read(&kcom->kobj.kref.refcount));
}

static struct kcom_obj_attribute kcom_attr_kref = __ATTR(kref, 0444, kcom_obj_show_kref, NULL);

#ifdef CONFIG_MODULE_UNLOAD
static ssize_t kcom_obj_show_mref(struct kcom_object *kcom, char *buf)
{
	return sprintf(buf, "%d\n", kcom->owner?module_refcount(kcom->owner):0);
}

static struct kcom_obj_attribute kcom_attr_mref = __ATTR(mref, 0444, kcom_obj_show_mref, NULL);
#endif

static ssize_t kcom_obj_show_owner(struct kcom_object *kcom, char *buf)
{
	return sprintf(buf, "%s\n", kcom->owner?kcom->owner->name:"<NULL>");
}

static struct kcom_obj_attribute kcom_attr_owner = __ATTR(owner, 0444, kcom_obj_show_owner, NULL);

static struct kcom_object *__kcom_get(struct kcom_object *kcom)
{
	if(kcom) {
		kcom = to_kcom_obj(kobject_get(&kcom->kobj));
		if(!TRY_KCOM_MODULE_GET(kcom->owner)) {
			kobject_put(&kcom->kobj);
			return NULL;
		}
		atomic_inc(&kcom->ref);
	}

	return kcom;
}

struct kcom_object *kcom_get(struct kcom_object *kcom)
{
	down_read(&kcom_rw_sem);
	kcom = __kcom_get(kcom);
	up_read(&kcom_rw_sem);

	return kcom;
}

EXPORT_SYMBOL(kcom_get);

static void __kcom_put(struct kcom_object *kcom)
{
	if(kcom) {
		if(atomic_read(&kcom->ref)==0) {
			WARN_ON(1);
			return;
		}
		atomic_dec(&kcom->ref);
		kobject_put(&kcom->kobj);
		KCOM_MODULE_PUT(kcom->owner);
	}
}

void kcom_put(struct kcom_object *kcom)
{
	down_read(&kcom_rw_sem);
	__kcom_put(kcom);
	up_read(&kcom_rw_sem);
}

EXPORT_SYMBOL(kcom_put);

static int __kcom_getby_uuid(const char *uuid, struct kcom_object **ppv)
{
	struct kobject *p;

	*ppv = NULL;

	list_for_each_entry(p, &kcom_subsys.list, entry) {
		struct kcom_object *kcom;
		kcom = __kcom_get(to_kcom_obj(p));
		BUG_ON(kcom ==NULL);

		if(strcmp(kcom->uuid, uuid) ==0) {
			*ppv = kcom;
			return 0;
		}

		__kcom_put(kcom);
	}

	return -ENOENT;
}

int kcom_getby_uuid(const char *uuid, struct kcom_object **ppv)
{
	int ret;

	down_read(&kcom_rw_sem);
	ret = __kcom_getby_uuid(uuid, ppv);
	up_read(&kcom_rw_sem);

	return ret;
}

EXPORT_SYMBOL(kcom_getby_uuid);


int kcom_register(struct kcom_object *kcom)
{
	int ret;
	struct kcom_object *p;

	const char* tmp = kcom->kobj.k_name;
    	kcom->kobj.k_name = NULL;

	down_write(&kcom_rw_sem);

	__kcom_getby_uuid(kcom->uuid, &p);
	if(p) {
		pr_error("uuid conflict, %s already used.\n", p->uuid);
		__kcom_put(p);
		up_write(&kcom_rw_sem);
		return -1;
	}

	if(kcom->parent) {
		struct kcom_object *parent;
		__kcom_getby_uuid(kcom->parent->uuid, &parent);
		if(parent==NULL) {
			up_write(&kcom_rw_sem);
			pr_error("parent %s<%s> not found.\n", kcom->parent->kobj.k_name, kcom->parent->uuid);
			return -EIO;
		}

		if(!parent->ta.type & KCOM_TYPE_CLASS) {
			__kcom_put(parent);
			up_write(&kcom_rw_sem);
			pr_error("only kcom_class can have sub objects.\n");
			return -EIO;
		}
		if(!atomic_read(&parent->kobj.kref.refcount)) {
			__kcom_put(parent);
			pr_error("parent %s<%s> not valid.\n", parent->kobj.k_name, parent->uuid);
			up_write(&kcom_rw_sem);
			WARN_ON(1);
			return -EIO;
		}

		if(parent->owner != kcom->owner)
			parent = __kcom_get(parent);
		else atomic_inc(&parent->ref);
		__kcom_put(parent);

		kcom->parent = parent;
		kcom->kobj.parent = &kcom->parent->kobj;

	} else 
		kcom->kobj.parent = &kcom_subsys.kobj;

	kobj_set_kset_s(kcom, kcom_subsys);
	kobject_init(&kcom->kobj);
	atomic_set(&kcom->ref, 0);

	kobject_set_name(&kcom->kobj, tmp);
	
	ret = kobject_add(&kcom->kobj);
	if(ret) {
		pr_error("%s register failed, errno %d!\n", kcom->kobj.k_name, ret);

		if(kcom->parent) { 
			if(kcom->parent->owner != kcom->owner)
				__kcom_put(kcom->parent);
			else atomic_dec(&kcom->parent->ref);
		}

		up_write(&kcom_rw_sem);
		return ret;
	}

	kcom_create_file(kcom, &kcom_attr_uuid);
	kcom_create_file(kcom, &kcom_attr_type);
	kcom_create_file(kcom, &kcom_attr_ref);
	kcom_create_file(kcom, &kcom_attr_kref);
#ifdef CONFIG_MODULE_UNLOAD
	kcom_create_file(kcom, &kcom_attr_mref);
#endif
	kcom_create_file(kcom, &kcom_attr_owner);

	up_write(&kcom_rw_sem);

	kcom_request_sync(); 

	return 0;
}

EXPORT_SYMBOL(kcom_register);

void kcom_unregister(struct kcom_object *kcom)
{
	down_write(&kcom_rw_sem);

	if(atomic_read(&kcom->ref)) {
		pr_error("can not remove kcom_object \'%s<%s>\', reference %d.\n",
				kcom->kobj.k_name, kcom->uuid, atomic_read(&kcom->ref));
		up_write(&kcom_rw_sem);
		return;
	}
	if(!atomic_read(&kcom->kobj.kref.refcount)) {
		pr_error("object %s<%s> not valid.\n", kcom->kobj.k_name, kcom->uuid);
		up_write(&kcom_rw_sem);
		WARN_ON(1);
		return ;
	}

	if(kcom->parent) { 
		if(kcom->parent->owner != kcom->owner)
			__kcom_put(kcom->parent);
		else atomic_dec(&kcom->parent->ref);
	}

	kcom_remove_file(kcom, &kcom_attr_kref);
	kcom_remove_file(kcom, &kcom_attr_ref);
	kcom_remove_file(kcom, &kcom_attr_uuid);
	kcom_remove_file(kcom, &kcom_attr_type);

	kobject_unregister(&kcom->kobj);

	up_write(&kcom_rw_sem);
}

EXPORT_SYMBOL(kcom_unregister);


static DECLARE_MUTEX(kcom_notify_sem);
static LIST_HEAD(kcom_notify);

static void kcom_request_sync(void)
{
	LIST_HEAD(tmp_list);

	down(&kcom_notify_sem);

	while(!list_empty(&kcom_notify)) {
		struct list_head *p;
		struct kcom_obj_request *mor;
		struct kcom_object *kcom;

		p = kcom_notify.next;
		list_del(p);

		mor = list_entry(p, struct kcom_obj_request, list);
		kcom_getby_uuid(mor->uuid, &kcom);

		if(kcom) {
			struct module *pmod = mor->owner;
			up(&kcom_notify_sem);
			mor->notify(kcom, mor);
			KCOM_MODULE_PUT(pmod);
			down(&kcom_notify_sem);
		} else {
			list_add(p, &tmp_list);
		}
	}

	list_add(&kcom_notify, &tmp_list);
	list_del(&tmp_list);

	up(&kcom_notify_sem);
}

#define KMOS_OBJ_REQUEST_PENDING(p) ({ struct kcom_obj_request *__req=(p); ( \
	!( (__req->list.next==LIST_POISON1 && __req->list.prev==LIST_POISON2) || \
			list_empty(&__req->list) || \
			(__req->list.next==NULL && __req->list.prev==NULL) ) \
		); })

int kcom_request_add(struct kcom_obj_request *req)
{
	down(&kcom_notify_sem);

	if(KMOS_OBJ_REQUEST_PENDING(req)) {
		up(&kcom_notify_sem);
		pr_error("kcom_obj_request is pending, or list not initialized.\n");
		WARN_ON(1);
		return -1;
	}

	if(!TRY_KCOM_MODULE_GET(req->owner))
		return -1;

	list_add(&req->list, &kcom_notify);

	up(&kcom_notify_sem);

	kcom_request_sync();

	return 0;
}

EXPORT_SYMBOL(kcom_request_add);

void kcom_request_del(struct kcom_obj_request *req)
{
	down(&kcom_notify_sem);
	if(KMOS_OBJ_REQUEST_PENDING(req)) {
		list_del(&req->list);
		KCOM_MODULE_PUT(req->owner);
	}
	up(&kcom_notify_sem);

}

EXPORT_SYMBOL(kcom_request_del);

static int __init kcom_init(void)
{
	int ret;

	ret = subsystem_register(&kcom_subsys);
	if(ret) {
		pr_error("subsystem_register failed, errno %d.\n", ret);
		return ret;
	}

	return ret;
}

static void __exit kcom_exit(void)
{
	subsystem_unregister(&kcom_subsys);
}


module_init(kcom_init);
module_exit(kcom_exit);

MODULE_AUTHOR("liujiandong");
MODULE_DESCRIPTION("Kernel object test");
MODULE_LICENSE("GPL");

