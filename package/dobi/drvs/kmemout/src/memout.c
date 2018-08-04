/*
 * To test memory allocation in kernel.
 *
 * Hua Shao <nossiac@163.com>
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the Free
 *  Software Foundation, version 2.
 *
 */


#include <linux/module.h>  /* Needed by all modules */
#include <linux/kernel.h>  /* Needed for KERN_ALERT */
#include <linux/slab.h>    /* Needed for kmalloc */
#include <linux/module.h>       /* For module specific items */
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/errno.h>

struct myring {
    struct sk_buff * data;
    struct list_head list;
};

static unsigned long __num__ = 0xFFFFFFFF;
static unsigned long __size__ = 1024;
static char __mode__ = 'k';
static LIST_HEAD(__ring__);

ssize_t get_alloc_size(struct file *file, char __user *buf, size_t len, loff_t *ppos) {
    char tmp[16];
    ssize_t left = 0;
    char * start = NULL;

    // printk("<api> %s(%p, 0x%p, %d, %lld).\n", __FUNCTION__, file, buf, len, *ppos);

    snprintf(tmp, sizeof(tmp), "%lu\n", __size__);
    start = tmp + (*ppos);
    left = strlen(tmp) - (*ppos);
    if (left <= 0) return 0;

    len = len>left?left:len; // actual read len

    BUG_ON((left-len) < 0);

    copy_to_user(buf, start, len);

    return len;
}

ssize_t set_alloc_size(struct file * fp, const char __user * buf, size_t len, loff_t * off) {
    kstrtoul_from_user(buf, len, 10, &__size__);
    printk("alloc size set to %lu\n", __size__);
    return len;
}


ssize_t get_alloc_num(struct file *file, char __user *buf, size_t len, loff_t *ppos) {
    char tmp[16];
    ssize_t left = 0;
    char * start = NULL;

    // printk("<api> %s(%p, 0x%p, %d, %lld).\n", __FUNCTION__, file, buf, len, *ppos);

    snprintf(tmp, sizeof(tmp), "%lu\n", __num__);
    start = tmp + (*ppos);
    left = strlen(tmp) - (*ppos);
    if (left <= 0) return 0;

    len = len>left?left:len; // actual read len

    BUG_ON((left-len) < 0);

    copy_to_user(buf, start, len);

    return len;
}


ssize_t set_alloc_num(struct file * fp, const char __user * buf, size_t len, loff_t * off) {
    kstrtoul_from_user(buf, len, 10, &__num__);
    printk("alloc num set to %lu\n", __num__);
    return len;
}

ssize_t alloc_start(struct file * fp, const char __user * buf, size_t len, loff_t * off) {
    int i;
    void * data = NULL;
    struct myring * tmp = NULL;
    char kbuf[12];

    size_t buf_size = min(len, sizeof(kbuf) - 1);
    if (copy_from_user(kbuf, buf, buf_size))
        return -EFAULT;

    kbuf[buf_size] = '\0';
    if (kbuf[0] == 'v')
        __mode__ = 'v';

    for (i = 0; i < __num__; i++) {
        printk("alloc %d, %p, size %lu\n", i, data, __size__);
        data = kmalloc(__size__, GFP_KERNEL);
        // data = kmalloc(__size__, GFP_ATOMIC);
        if (unlikely(!data)) {
            printk("failed to alloc size %lu\n", __size__);
            break;
        }

        if (__mode__ == 'v')
            tmp = vmalloc(sizeof(struct myring));
           else
            tmp = kmalloc(sizeof(struct myring), GFP_KERNEL);
        if (unlikely(!tmp)) {
            printk("failed to alloc myring of size %d\n", sizeof(struct myring));
            break;
        }
        tmp->data = data;
        list_add(&tmp->list, &__ring__);
    }

    return len;
}

ssize_t free_start(struct file * fp, const char __user * buf, size_t len, loff_t * off) {
    struct myring *tmp, *next;
    int i = 0;
    list_for_each_entry_safe(tmp, next, &__ring__, list) {
        printk("free [%d] %p\n", i++, tmp);
        if (__mode__ == 'v')
            vfree(tmp->data);
           else
            kfree(tmp->data);
        list_del(&tmp->list);

        kfree(tmp);
    }

    return len;
}

static const struct file_operations alloc_size_ops = {
    .owner      = THIS_MODULE,
    .write      = set_alloc_size,
    .read       = get_alloc_size,
};
static const struct file_operations alloc_num_fops = {
    .owner      = THIS_MODULE,
    .write      = set_alloc_num,
    .read       = get_alloc_num,
};
static const struct file_operations alloc_fops = {
    .owner      = THIS_MODULE,
    .write      = alloc_start,
};
static const struct file_operations free_fops = {
    .owner      = THIS_MODULE,
    .write      = free_start,
};


struct proc_dir_entry * procroot = NULL;

int __init kmemout_init(void) {

    printk(KERN_ALERT "hello\n");

    procroot = proc_mkdir("kmemout", NULL);
    proc_create("alloc_size", 0755, procroot, &alloc_size_ops);
    proc_create("alloc_num", 0755, procroot, &alloc_num_fops);
    proc_create("alloc_start", 0755, procroot, &alloc_fops);
    proc_create("free_start", 0755, procroot, &free_fops);

    return 0;
}


void __exit kmemout_exit(void) {
    printk(KERN_ALERT "bye\n");
    free_start(NULL, NULL, 0, NULL);
    remove_proc_subtree("kmemout", NULL);
}

module_init(kmemout_init);
module_exit(kmemout_exit);

MODULE_AUTHOR("Hua Shao <nossiac@163.com>");
MODULE_DESCRIPTION("To test memory allocation in kernel");
MODULE_VERSION("1.0");
MODULE_LICENSE("GPL");
