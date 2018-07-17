/*
 *  A module to study kernel's file operations.
 *
 *  (C) Copyright Hua Shao <nossiac@163.com>,
 *
 */



#include <linux/kernel.h>       /* For printk/panic/... */
#include <linux/module.h>       /* For module specific items */
#include <linux/moduleparam.h>  /* For new moduleparam's */
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/vmalloc.h>
#include <linux/errno.h>


/* This is a section from "Declaration of Independence (1776)"
 * We assume it is the content of a file.
*/
static char * __content__ = "We hold these truths to be self-evident, \
that all men are created equal, that they are endowed by their Creator \
with certain unalienable rights, that among these are life, liberty and \
the pursuit of happiness. That to secure these rights, governments are \
instituted among men, deriving their just powers from the consent of the \
governed. That whenever any form of government becomes destructive to \
these ends, it is the right of the people to alter or to abolish it, \
\and to institute new government, laying its foundation on such principles \
and organizing its powers in such form, as to them shall seem most likely \
to effect their safety and happiness. Prudence, indeed, will dictate that \
governments long established should not be changed for light and transient \
causes; and accordingly all experience hath shown that mankind are more \
disposed to suffer, while evils are sufferable, than to right themselves \
by abolishing the forms to which they are accustomed. But when a long train \
of abuses and usurpations, pursuing invariably the same object evinces a \
design to reduce them under absolute despotism, it is their right, it is \
their duty, to throw off such government, and to provide new guards for \
their future security. -- Such has been the patient sufferance of these \
colonies; and such is now the necessity which constrains them to alter their \
former systems of government. The history of the present King of Great \
Britain is a history of repeated injuries and usurpations, all having in \
direct object the establishment of an absolute tyranny over these states. \
To prove this, let facts be submitted to a candid world.\n\
";


static int fapi_help(struct seq_file *m, void *v)
							{
    seq_printf(m, "%s",
        "this is the help message\n");
    return 0;
}

static int fapi_proc_open(struct inode *inode, struct file * fp)
{
    printk("<api> %s\n", __FUNCTION__);

    printk("inode->i_mode:0x%x\n", inode->i_mode);
    printk("inode->i_uid.val:0x%x\n", inode->i_uid.val);
    printk("inode->i_gid.val:0x%x\n", inode->i_gid.val);
    printk("inode->i_flags:0x%x\n", inode->i_flags);
    printk("inode->i_opflags:0x%x\n", inode->i_opflags);
    printk("inode->i_size:0x%x\n", inode->i_size);
    printk("inode->i_bytes:0x%x\n", inode->i_bytes);
    printk("inode->i_version:0x%x\n", inode->i_version);

    printk("file->f_mode:0x%x\n", fp->f_mode);
    printk("file->f_flags:0x%x\n", fp->f_flags);
    printk("file->f_count:0x%x\n", fp->f_count);
    printk("file->f_version:0x%x\n", fp->f_version);

    return 0;
}


ssize_t fapi_proc_read(struct file * fp, char __user * buf, size_t len, loff_t * off)
{
	int total = strlen(__content__);
    int ret = total - *off;
    printk("<api> %s(%p, 0x%p, %d, 0x%p(0x%x)).\n", __FUNCTION__, fp, buf, len, off, *off);

	if (ret<0)
	    return -EFAULT;
	else if (ret==0)
		return 0;
	else
		ret = ret>len?len:ret;

	copy_to_user(buf, __content__, ret);
	*off += ret;

	return ret;
}

static int fapi_proc_write(struct file * fp, const char __user * buf, size_t len, loff_t * off)
{
	//char * __buf = NULL;
	char __buf[1024];
    printk("<api> %s(%p, 0x%p, %d, 0x%p(0x%x)).\n", __FUNCTION__, fp, buf, len, off, *off);

	//__buf == (char *)vmalloc(len);
	//if (!__buf) return -ENOMEM;

	copy_from_user(__buf, buf, len);
	printk("%s\n", __buf);
	printk("%s\n", buf);
    return len;
}


static int fapi_proc_mmap(struct file * fp, struct vm_area_struct * vmas)
{
    printk("<api> %s\n", __FUNCTION__);
    return 0;
}

static int fapi_proc_flush(struct file * fp, fl_owner_t id)
{
    printk("<api> %s\n", __FUNCTION__);
    return 0;
}

static int fapi_proc_fsync(struct inode * inode, struct file * fp)
{
    printk("<api> %s\n", __FUNCTION__);
    return 0;
}

static int fapi_proc_llseek(struct file * fp, loff_t off, int whence)
{
    printk("<api> %s\n", __FUNCTION__);
    return 0;
}

static int fapi_proc_release(struct inode *inode, struct file *file)
{
    printk("<api> %s\n", __FUNCTION__);
    return 0;
}

static const struct file_operations proc_fops = {
    .owner      = THIS_MODULE,
    .open       = fapi_proc_open,
    .read       = fapi_proc_read,
    .write		= fapi_proc_write,
    .mmap		= fapi_proc_mmap,
    .flush		= fapi_proc_flush,
    .fsync		= fapi_proc_fsync,
    .llseek     = fapi_proc_llseek,
    .release    = fapi_proc_release,
};

static int __init fapi_init(void)
{
    int err = 0;

    struct proc_dir_entry * parent;
    parent = proc_mkdir("fapi", NULL);

    if (!proc_create("help", 0755, parent, &proc_fops))
    {
        printk("failed to create proc help!\n");
        return -EIO;
    }

    printk("fapi init!\n");
    return err;
}


static void __exit fapi_exit(void)
{
    remove_proc_entry("fapi/help", NULL);
    remove_proc_entry("fapi", NULL);
    printk("fapi exit!\n");
}


module_init(fapi_init);
module_exit(fapi_exit);

MODULE_AUTHOR("Hua Shao <nossiac@163.com>");
MODULE_DESCRIPTION("A module to study the kernel's file operations!");
MODULE_LICENSE("GPL");
