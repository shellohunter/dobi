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
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/errno.h>


static char * __help__ = "this is the help message";


static int kcrash_help(struct seq_file *m, void *v)
{
    seq_printf(m, "%s\n",__help__);
    return 0;
}

static int kcrash_proc_open(struct inode *inode, struct file *file)
{
    return single_open(file, kcrash_help, NULL);
}


ssize_t kcrash_oom(struct file * fp, const char __user * buf, size_t len, loff_t * off)
{
	int i=0;

	for (i=0; i<10000; i++)
	{
		printk(KERN_ALERT "%d\n", i);
		kmalloc(4096, GFP_KERNEL);
	}
	return len;
}

static const struct file_operations help_fops = {
    .owner      = THIS_MODULE,
    .open       = kcrash_proc_open,
    .read       = seq_read,
    .llseek     = seq_lseek,
    .release    = single_release,
};

static const struct file_operations oom_fops = {
    .owner      = THIS_MODULE,
    .write		= kcrash_oom,
};


static int __init kcrash_init(void)
{
    int err = 0;

    struct proc_dir_entry * parent;
    parent = proc_mkdir("kcrash", NULL);

    if (!proc_create("help", 0755, parent, &help_fops))
    {
        printk("failed to create proc help!\n");
        return -EIO;
    }

    if (!proc_create("oom-trigger", 0755, parent, &oom_fops))
    {
        printk("failed to create proc help!\n");
        return -EIO;
    }


    printk("kcrash init!\n");
    return err;
}


static void __exit kcrash_exit(void)
{
    remove_proc_entry("kcrash/help", NULL);
    remove_proc_entry("kcrash", NULL);
    printk("kcrash exit!\n");
}


module_init(kcrash_init);
module_exit(kcrash_exit);

MODULE_AUTHOR("Hua Shao <nossiac@163.com>");
MODULE_DESCRIPTION("A module to crash the kernel!");
MODULE_LICENSE("GPL");
