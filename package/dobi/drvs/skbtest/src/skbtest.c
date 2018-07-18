/*
 * To trigger oom-killer.
 *
 * Hua Shao (nossiac@163.com)
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
#include <linux/slab.h>
#include <linux/skbuff.h>
#include <linux/fs.h>
#include <linux/errno.h>

struct myring {
    struct sk_buff * skb;
    struct list_head list;
};

static unsigned long __num__ = 0xFFFFFFFF;
static unsigned long __size__ = 1024;
static LIST_HEAD(__ring__);

ssize_t get_skb_size(struct file *file, char __user *buf, size_t len, loff_t *ppos) {
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

ssize_t set_skb_size(struct file * fp, const char __user * buf, size_t len, loff_t * off) {
    kstrtoul_from_user(buf, len, 10, &__size__);
    printk("skb size set to %lu\n", __size__);
    return len;
}


ssize_t get_skb_num(struct file *file, char __user *buf, size_t len, loff_t *ppos) {
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


ssize_t set_skb_num(struct file * fp, const char __user * buf, size_t len, loff_t * off) {
    kstrtoul_from_user(buf, len, 10, &__num__);
    printk("skb num set to %lu\n", __num__);
    return len;
}

ssize_t skb_alloc_start(struct file * fp, const char __user * buf, size_t len, loff_t * off) {
    int i;
    struct sk_buff * skb = NULL;
    struct myring * tmp = NULL;
    for (i = 0; i < __num__; i++) {
        printk("skb %d, %p, size %lu\n", i, skb, __size__);
        skb = alloc_skb(__size__, GFP_ATOMIC);
        if (unlikely(!skb)) {
            printk("failed to alloc skb of size %lu\n", __size__);
            break;
        }

        tmp = kmalloc(sizeof(struct myring), GFP_KERNEL);
        if (unlikely(!tmp)) {
            printk("failed to alloc myring of size %d\n", sizeof(struct myring));
            break;
        }
        tmp->skb = skb;
        list_add(&tmp->list, &__ring__);
    }

    return len;
}

ssize_t skb_free_start(struct file * fp, const char __user * buf, size_t len, loff_t * off) {
    struct myring *tmp, *next;
    int i = 0;
    list_for_each_entry_safe(tmp, next, &__ring__, list) {
        printk("free skb[%d] %p\n", i++, tmp);
        kfree_skb(tmp->skb);
        list_del(&tmp->list);
        kfree(tmp);
    }

    return len;
}

static const struct file_operations skb_size_ops = {
    .owner      = THIS_MODULE,
    .write      = set_skb_size,
    .read       = get_skb_size,
};
static const struct file_operations skb_num_fops = {
    .owner      = THIS_MODULE,
    .write      = set_skb_num,
    .read       = get_skb_num,
};
static const struct file_operations skb_alloc_fops = {
    .owner      = THIS_MODULE,
    .write      = skb_alloc_start,
};
static const struct file_operations skb_free_fops = {
    .owner      = THIS_MODULE,
    .write      = skb_free_start,
};


struct proc_dir_entry * procroot = NULL;

int __init skbtest_init(void) {

    printk(KERN_ALERT "hello\n");

    procroot = proc_mkdir("skbtest", NULL);
    proc_create("skb_size", 0755, procroot, &skb_size_ops);
    proc_create("skb_num", 0755, procroot, &skb_num_fops);
    proc_create("skb_alloc", 0755, procroot, &skb_alloc_fops);
    proc_create("skb_free", 0755, procroot, &skb_free_fops);

    return 0;
}


void __exit skbtest_exit(void) {
    printk(KERN_ALERT "bye\n");
    skb_free_start(NULL, NULL, 0, NULL);
    remove_proc_subtree("skbtest", NULL);
}

module_init(skbtest_init);
module_exit(skbtest_exit);

MODULE_AUTHOR("Hua Shao <nossiac@163.com>");
MODULE_DESCRIPTION("To test slab allocation");
MODULE_VERSION("1.0");
MODULE_LICENSE("GPL");
