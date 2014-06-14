
#ifndef __MODULE_KERNEL_OBJECT_H
#define __MODULE_KERNEL_OBJECT_H

#define KCOM_KOBJ_NAME "kcom"
#define KCOM_UUID_LEN KOBJ_NAME_LEN

#define KCOM_TYPE_OBJECT	0x00
#define KCOM_TYPE_CLASS		0x01

struct kcom_object {
	char uuid[KCOM_UUID_LEN];

	void (*release)(struct kcom_object *kcom);

	struct kcom_object *parent;
	struct module *owner;

	union {
		struct {
			int type	:4;
		};
		unsigned long w32;
	}ta;

	atomic_t ref;

	struct kcom_obj_attribute *kcom_attrs;
	struct list_head childs;
	struct kobject kobj;
};

#define kcom_to_instance(obj, type, member) container_of(obj, type, member)

#define KCOM_OBJ_INIT(_name, _uuid, _parent, _owner, _type, _release) \
	{ \
		.uuid = _uuid, \
		.kobj = { .k_name = _name, }, \
		.release = _release, \
		.parent = _parent, \
		.owner = _owner, \
		.ta = {{ .type = _type }}, \
	}

struct kcom_obj_attribute {
	struct attribute attr;
        ssize_t (*show)(struct kcom_object*, char *);
        ssize_t (*store)(struct kcom_object*, const char *, size_t);
};

int kcom_register(struct kcom_object *kcom);
void kcom_unregister(struct kcom_object *kcom);
struct kcom_object *kcom_get(struct kcom_object *kcom);
int kcom_getby_uuid(const char *uuid, struct kcom_object **ppv);
void kcom_put(struct kcom_object *kcom);


struct kcom_obj_request;

typedef void (*kcom_obj_request_notify_t)(struct kcom_object *kcom, struct kcom_obj_request *self);

struct kcom_obj_request {
	char uuid[KCOM_UUID_LEN];
	kcom_obj_request_notify_t notify;
	struct module *owner;

	struct list_head list;
};

int kcom_request_add(struct kcom_obj_request *req);
void kcom_request_del(struct kcom_obj_request *req);


/*
 * Helper APIs
 */
#define KCOM_GET_INSTANCE(uuid, instance_ptr) ({ \
		struct kcom_object *__kcom_get_instance_ptr = NULL; \
		instance_ptr = NULL; \
		if(!kcom_getby_uuid(uuid, &__kcom_get_instance_ptr))  \
			instance_ptr = kcom_to_instance(__kcom_get_instance_ptr, typeof(*instance_ptr), kcom); \
		instance_ptr ==NULL; \
	})

#define KCOM_PUT_INSTANCE(instance_ptr) ({ \
		kcom_put(&instance_ptr->kcom); \
		instance_ptr = NULL; \
	})

#endif

