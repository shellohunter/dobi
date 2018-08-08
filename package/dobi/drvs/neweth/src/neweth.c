/*
 * RAETH FROM SCRATCH
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

int __init neweth_init(void)
{
    printk(KERN_ALERT "hi\n");
    return 0;
}


void __exit neweth_exit(void)
{
    printk(KERN_ALERT "bye\n");
}

module_init(neweth_init);
module_exit(neweth_exit);

MODULE_AUTHOR("Hua Shao <nossiac@163.com>");
MODULE_DESCRIPTION("RAETH FROM SCRATCH");
MODULE_VERSION("2.0");
MODULE_LICENSE("GPL");
