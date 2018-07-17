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

#define MODULE
#define LINUX
#define __KERNEL__

#include <linux/module.h>  /* Needed by all modules */
#include <linux/kernel.h>  /* Needed for KERN_ALERT */
#include <linux/slab.h>    /* Needed for kmalloc */

int __init memout_init(void)
{
    int i=0;
    for (i=0; i<10000; i++)
    {
        printk(KERN_ALERT "%d\n", i);
        kmalloc(4096, GFP_KERNEL);
    }
    return 0;
}


void __exit memout_exit(void)
{
    printk(KERN_ALERT "bye\n");
}

module_init(memout_init);
module_exit(memout_exit);

MODULE_AUTHOR("Hua Shao <nossiac@163.com>");
MODULE_DESCRIPTION("To trigger oom-killer");
MODULE_VERSION("2.0");
MODULE_LICENSE("GPL");
