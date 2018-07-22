/*
 *  A module to crash the kernel in various way.
 *  For the purpose of studying kernel crash debuging.
 *
 *  (C) Copyright Hua Shao <nossiac@163.com>,
 *
 */



#include <linux/kernel.h>       /* For printk/panic/... */
#include <linux/module.h>       /* For module specific items */
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/delay.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/errno.h>


static char * __help__ = 
    " echo 1 > /proc/kcrash/spinlock\n"
    "     to acquire a spinlock twice\n"
    " echo 1 > /proc/kcrash/deadloop\n"
    "     to fall into a endless loop\n"
    ;


static int kcrash_help(struct seq_file *m, void *v)
{
    seq_printf(m, "%s\n",__help__);
    return 0;
}

static int kcrash_help_open(struct inode *inode, struct file *file)
{
    return single_open(file, kcrash_help, NULL);
}


ssize_t kcrash_deadloop_handler(struct file * fp, const char __user * buf, size_t len, loff_t * off)
{
    printk("entering deadloop!\n");
    while(1) udelay(1000);

    /* if rcu stall detection is enabled, you will see "rcu_sched self-detected stall on CPU x" soon*/

	return len;
}


ssize_t kcrash_spinlock_handler(struct file * fp, const char __user * buf, size_t len, loff_t * off)
{
    
    spinlock_t mylock;
    spin_lock_init(&mylock);
    printk("spinlock inited!\n");
    spin_lock(&mylock);
    printk("spinlock locking ok\n");
    spin_unlock(&mylock);
    printk("spinlock unlocking ok!\n");
    printk("now try to aquire spinlock twice!\n");
    spin_lock(&mylock);
    printk("spinlock locked 1!\n");
    spin_lock(&mylock);
    printk("spinlock locked 2!\n");
    // spin_lock_irqsave()
    while(1) udelay(1000);

    return len;
}

static const struct file_operations kcrash_help_fops = {
    .owner      = THIS_MODULE,
    .open       = kcrash_help_open,
    .read       = seq_read,
    .llseek     = seq_lseek,
    .release    = single_release,
};

static const struct file_operations kcrash_deadloop_fops = {
    .owner      = THIS_MODULE,
    .write		= kcrash_deadloop_handler,
};


static const struct file_operations kcrash_spinlock_fops = {
    .owner      = THIS_MODULE,
    .write      = kcrash_spinlock_handler,
};

static int __init kcrash_init(void)
{
    int err = 0;

    struct proc_dir_entry * parent = proc_mkdir("kcrash", NULL);
    proc_create("help", 0755, parent, &kcrash_help_fops);
    proc_create("deadloop", 0755, parent, &kcrash_deadloop_fops);
    proc_create("spinlock", 0755, parent, &kcrash_spinlock_fops);
    return err;
}


static void __exit kcrash_exit(void)
{
    remove_proc_entry("kcrash/deadloop", NULL);
    remove_proc_entry("kcrash/spinlock", NULL);
    remove_proc_entry("kcrash", NULL);
    // remove_proc_subtree("kcrash");
}


module_init(kcrash_init);
module_exit(kcrash_exit);

MODULE_AUTHOR("Hua Shao <nossiac@163.com>");
MODULE_DESCRIPTION("A module to tickle the kernel!");
MODULE_LICENSE("GPL");
