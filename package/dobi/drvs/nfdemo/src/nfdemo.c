/*
 * Demo code to create a netfilter extension.
 *
 * Hua Shao (nossiac@163.com)
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the Free
 *  Software Foundation, version 2.
 *
 */

#ifndef _LINUX_NETFILTER_XT_IPADDR_H
#define _LINUX_NETFILTER_XT_IPADDR_H

enum
{
	XT_IPADDR_SRC = 1 << 0,
	XT_IPADDR_DST = 1 << 1,
	XT_IPADDR_SRC_INV = 1 << 2,
	XT_IPADDR_DST_INV = 1 << 3,
};


#include <linux/module.h>  /* Needed by all modules */
#include <linux/kernel.h>  /* Needed for KERN_ALERT */
#include <linux/slab.h>    /* Needed for kmalloc */

int __init nfdemo_init(void)
{
    printk(KERN_ALERT "hello\n");
    return 0;
}


void __exit nfdemo_exit(void)
{
    printk(KERN_ALERT "bye\n");
}

module_init(nfdemo_init);
module_exit(nfdemo_exit);

MODULE_AUTHOR("Hua Shao <nossiac@163.com>");
MODULE_DESCRIPTION("Demo code to create a netfilter extension.");
MODULE_VERSION("2.0");
MODULE_LICENSE("GPL");

#endif /* _LINUX_NETFILTER_XT_IPADDR_H */
