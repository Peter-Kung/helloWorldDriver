#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kdev_t.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched/signal.h>
#include <linux/sched.h>
#include <linux/string.h>
#include <linux/slab.h>


static DECLARE_WAIT_QUEUE_HEAD(wq);
static int flag = 0;
static int block_dev = 0;
static int block_nr_devs = 5;
static struct class *block_class;
static struct cdev *block_cdev;

MODULE_AUTHOR("Peter");
MODULE_LICENSE("GPL");

static int block_open(struct inode *inode, struct file *filp) 
{
	printk("Open success\n");
	return 0;
}

static ssize_t block_read(struct file *filp, char __user *buffer, size_t count, loff_t  *f_ops)
{
	printk(KERN_DEBUG "process %i (%s) going to sleep\n", current->pid, current->comm);
	wait_event_interruptible(wq, flag != 0);
	flag = 0;
	printk(KERN_DEBUG "awoken %i (%s)\n", current->pid, current->comm);
	return 0;
}

static ssize_t block_write(struct file *filp, const char __user *buffer, size_t count, loff_t *f_ops)
{
	printk(KERN_DEBUG "process %i (%s) awakening the readers... \n", current->pid, current->comm);
	flag = 1;
	printk("Wake up now\n");
	wake_up_interruptible(&wq);
	printk("write done\n");
	return count;
}

static int block_release(struct inode *inode, struct file *filp) 
{
	return 0;
}

struct file_operations block_fops = {
	.owner = THIS_MODULE,
	.open = block_open,
	.release = block_release,
	.write = block_write,
	.read = block_read,
};


static int __init init_function(void)
{
	int result = 0;
	dev_t dev = 0;
	result = alloc_chrdev_region(&block_dev, 0, block_nr_devs, "blockDriver");

	if (result < 0)	 {
		printk(KERN_ALERT "block_driver: can't not allocate get major, errno: %d", result);
		return 0;
	}

	block_cdev = cdev_alloc();
	if (!block_cdev) {
		printk(KERN_ALERT "block_driver: cdev_alloc failed, errnoi\n");
		result = -1;
		goto  fail_cdev;
	}
	block_cdev->ops = &block_fops;
	result = cdev_add(block_cdev, block_dev, block_nr_devs);
	if (result < 0) {
		printk(KERN_ALERT "block_driver: can't register into device\n");
		result = -2;
		goto fail_cdev;
	}
    block_class = class_create(THIS_MODULE, "blockDriver");
	if (!block_class) {
		printk(KERN_ALERT "block_driver: can't create class\n");
		result = -3;
		goto fail_class_create;
	}	
	
	if (!device_create(block_class, NULL, block_dev, NULL, "blockDriver")) {
		printk(KERN_ALERT "block_driver: can't create device\n");
		result = -4;
		goto fail_device_create;
	}
	
	return result;

fail_device_create:
	class_destroy(block_class);
fail_class_create:
	cdev_del(block_cdev);
fail_cdev:
	unregister_chrdev_region(block_dev, block_nr_devs);
	return result;
}

static void __exit exit_function(void)
{
	device_destroy(block_class, block_dev);
	class_destroy(block_class);
	cdev_del(block_cdev);
	unregister_chrdev_region(block_dev, block_nr_devs);
}
module_init(init_function);
module_exit(exit_function);
