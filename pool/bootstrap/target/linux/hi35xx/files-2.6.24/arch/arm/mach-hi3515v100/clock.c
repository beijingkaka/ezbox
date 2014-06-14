/* linux/arch/arm/mach-hi3515_v100/clock.c
*
* Copyright (c) 2006 Hisilicon Co., Ltd. 
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

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/errno.h>
#include <linux/err.h>

#include <linux/string.h>

#include <asm/semaphore.h>
#include <linux/clk.h>

#include <asm/arch/system.h>

#include <asm/arch/clock.h>
#include <asm/arch/early-debug.h>

#include <linux/proc_fs.h>

static LIST_HEAD(clocks);
static DECLARE_MUTEX(clocks_sem);

typedef int (*clk_walker_t)(struct clk*, int, unsigned long []);

int hisilicon_clk_default_notifier(struct clk *clk, 
		unsigned int cmd, unsigned long wparam, unsigned long lparam)
{
	int ret = 0;

	switch(cmd)
	{
	case HISILICON_CLK_PARENT_DEL:
		hisilicon_trace(2,"HISILICON_CLK_PARENT_DEL: %s, P: %s", clk->name, clk->parent_name);
		break;

	case HISILICON_CLK_SETRATE:
		clk->rate = wparam;
		break;

	default:
		break;
	}

	return ret;
}

static int __clk_walk_r(struct list_head *clk_list, clk_walker_t __walk_callback, int level, unsigned long data[])
{
	struct clk *p;
	int ret = 0;

	list_for_each_entry(p, clk_list, list)
	{
		if(__walk_callback(p, level, data))
			break;
		ret = __clk_walk_r(&p->child_list, __walk_callback,level+1, data);
		if(ret)
			break;
	}

	return ret;
}

static int __clk_walk(clk_walker_t __walk_callback, unsigned long data[])
{
	if(__walk_callback == NULL)
		return 0;

	return __clk_walk_r(&clocks, __walk_callback, 0, data);
}

static int __clk_get_walker(struct clk *clk, int level, unsigned long data[])
{
	const char *name = (const char*)data[0];

	if (strcmp(name, clk->name) == 0 && try_module_get(clk->owner))
	{
		data[1] = (unsigned long)clk;
		return 1;
	}

	return 0;
}
static struct clk *__clk_get(struct device *dev, const char *name)
{
	unsigned long _clk_get_data[2] = {(unsigned long)name, 0};

	if(name==NULL)
		return NULL;
	if(strlen(name)==0)
		return NULL;

	__clk_walk(__clk_get_walker, _clk_get_data);

	return (struct clk*)_clk_get_data[1];
}
struct clk *clk_get(struct device *dev, const char *name)
{
	struct clk *clk = NULL;

	down(&clocks_sem);
	clk = __clk_get(dev, name);
	up(&clocks_sem);

	return clk;
}

static int __clk_info_walker(struct clk *clk, int level, unsigned long data[])
{
	char *buf = (char*)data[0];
	int len=(int)data[1];
	int pos=0;

	while(level-- && len>pos)
		pos += snprintf(buf+pos,len-pos,".   ");

	pos += snprintf(buf+pos,len-pos,"|---");
	pos += snprintf(buf+pos,len-pos," %s, %lu, %s\n", clk->name, clk->rate, clk->parent_name);

	if(pos>=len)
	{
		pos=len;
		buf[pos-1]='\0';
	}
	data[0]=(unsigned long)buf+pos;
	data[1]=len-pos;
	data[2]++;

	return pos>=len;
}
int clk_info(char *buf, int len)
{
	unsigned long _clk_info_data[3] = {(unsigned long)buf, len, 0};
	int t;

	down(&clocks_sem);
	__clk_walk(__clk_info_walker, _clk_info_data);

	t = (int)_clk_info_data[1];
	t -= snprintf((char*)_clk_info_data[0],t, "%d clocks total\n", (int)_clk_info_data[2]);
	if(t<=0)
	{
		t = 0;
		buf[len-1] = '\0';
	}
	up(&clocks_sem);

	return len-t;
}


void clk_put(struct clk *clk)
{
	module_put(clk->owner);
}

int clk_enable(struct clk *clk)
{
	return 0;
}

void clk_disable(struct clk *clk)
{
}

int clk_use(struct clk *clk)
{
	return 0;
}

void clk_unuse(struct clk *clk)
{
}

unsigned long clk_get_rate(struct clk *clk)
{
	return clk->rate;
}

long clk_round_rate(struct clk *clk, unsigned long rate)
{
	return rate;
}

int clk_notify_childs(struct clk *clk, unsigned int cmd, unsigned long wparam, unsigned long lparam)
{
	struct clk *p;

	list_for_each_entry(p, &clk->child_list, list)
		p->notifier(p, cmd, wparam, lparam);

	return 0;
}

int clk_set_rate(struct clk *clk, unsigned long rate)
{
	return clk->notifier(clk, HISILICON_CLK_SETRATE, rate, clk_get_rate(clk));
}

int clk_set_parent(struct clk *clk, struct clk *parent)
{
	down(&clocks_sem);
	if(!list_empty(&clk->list))
		list_del(&clk->list);
	list_add(&clk->list, &parent->child_list);
	strcpy(clk->parent_name, parent->name);
	up(&clocks_sem);

	return 0;
}

struct clk *clk_get_parent(struct clk *clk)
{
	return clk_get(NULL, clk->parent_name);
}

int clk_register(struct clk *clk)
{
	struct list_head *_clk_head = &clocks;
	struct clk *parent = NULL;

	hisilicon_trace(2,"%s", clk->name);

	if(clk_get(NULL, clk->name))
	{
		printk(KERN_WARNING "clock %s exist, register failed.\n", clk->name);
		clk_put(clk);
		return -1;
	}

	if(strlen(clk->parent_name)==0)
		strcpy(clk->parent_name, "<NULL>");
	parent = clk_get(NULL, clk->parent_name);
	if(parent)
		_clk_head = &parent->child_list;

	INIT_LIST_HEAD(&clk->child_list);
	if(clk->notifier == NULL)
		clk->notifier = hisilicon_clk_default_notifier;

	down(&clocks_sem);
	list_add(&clk->list, _clk_head);
	up(&clocks_sem);

	if(parent)
		clk_put(parent);

	//return clk->notifier(clk, HISILICON_CLK_REGISTER, 0, 0);
	return 0;
}

void clk_unregister(struct clk *clk)
{
	hisilicon_trace(2,"%s", clk->name);

	down(&clocks_sem);
	list_del(&clk->list);
	up(&clocks_sem);

	if(list_empty(&clk->child_list))
		return;

	while(!list_empty(&clk->child_list))
	{
		struct clk *p;

		down(&clocks_sem);
		p = list_entry(clk->child_list.next, struct clk, list);
		list_del(&p->list);
		list_add(&p->list, &clocks);
		up(&clocks_sem);

		p->notifier(p, HISILICON_CLK_PARENT_DEL, 0, 0);
	}
	
}

#ifdef CONFIG_PROC_FS

static int clk_proc_get_info(char *buf, char **start, off_t off, int count)
{
	return clk_info(buf, count);
}

static int __init clk_info_init(void)
{
	create_proc_info_entry("clocks", 0444, NULL, clk_proc_get_info);

	return 0;
}

module_init(clk_info_init);

#endif /* CONFIG_PROC_FS */

static int __init clk_init(void)
{
	printk(KERN_INFO "Hisilicon clock system V0.01\n");

	return 0;
}
arch_initcall(clk_init);


EXPORT_SYMBOL(clk_register);
EXPORT_SYMBOL(clk_unregister);
EXPORT_SYMBOL(clk_set_rate);
EXPORT_SYMBOL(clk_get_rate);
EXPORT_SYMBOL(clk_round_rate);
EXPORT_SYMBOL(clk_use);
EXPORT_SYMBOL(clk_unuse);
EXPORT_SYMBOL(clk_enable);
EXPORT_SYMBOL(clk_disable);
EXPORT_SYMBOL(clk_get);
EXPORT_SYMBOL(clk_put);
EXPORT_SYMBOL(clk_set_parent);
EXPORT_SYMBOL(clk_get_parent);
EXPORT_SYMBOL(clk_info);
EXPORT_SYMBOL(clk_notify_childs);
EXPORT_SYMBOL(hisilicon_clk_default_notifier);
