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

#include "./helloWorld.h"	


#define LENGTH 10

int helloWorld_major;
static dev_t helloWorld_dev = 0;
static struct cdev *helloWorld_cdev; 
static struct class *helloWorld_class;

static char recvi[LENGTH] = {0};
static char s[][10] = {"Good", "soso", "bad"};
static int counter = 0;
static int helloWorld_open(struct inode *inode, struct file *filp) ;
static ssize_t helloWorld_read(struct file *filp, char __user *buffer, size_t count,  loff_t *f_pos)  ;
static ssize_t  helloWorld_write(struct file *filp, const char __user *buffer, size_t count,  loff_t *f_pos) ;
static int helloWorld_release(struct inode *inode, struct file *filp) ;
long helloWorld_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);



static int helloWorld_open(struct inode *inode, struct file *filp) 
{
	printk(KERN_INFO "open success");
	return 0;
}

static ssize_t helloWorld_read(struct file *filp, char __user *buffer, size_t count,  loff_t *f_pos) 
{
    counter = (counter + 1) % 3;

	int arr[] = {10, 20, 30};
    ssize_t len = strlen(s[counter]);
    
    if (count < len + 1) {
        printk("buffer length to short\n");
        return -EFAULT;
    }

    count = len;
    if (copy_to_user((int __user*)buffer, arr, 12)) {
        return -EFAULT;
    }
    printk(KERN_INFO "count: %d\n", count);
    return count;
}

static ssize_t  helloWorld_write(struct file *filp, const char __user *buffer, size_t count,  loff_t *f_pos) 
{
	int result = 0;
	if (LENGTH < count) {
		printk(KERN_WARNING "buffer length can't contain %d (maximun: 10)", count);
		return -EFAULT;
	}
	if (result = copy_from_user(recvi, buffer, count)) {
		printk(KERN_WARNING "copy_from_user error %d", result);
		return result;
	}
	printk(KERN_INFO "Result: %s", recvi);
	return result;
}


/*
 * @inode, @filp: for FILE structure use
 * @cmd: choose cmd th io command
 * @args: optional, decided by cmd spec
 */
long helloWorld_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	int arr[3] = {0};
	int breakfast = 0, err = 0, ret = 0;
	int dinner = 0;
	/* cmd bits represents significant meaning:
	 * bits 31 ~ 24: driver major number
	 * bits 23 ~ 17: cmd number
	 * bits 16 ~ 1 : datatype size
	 */

	/* Check cmd is belong to this module: _IOC_TYPE(nr) :=  (((nr) >> _IOC_TYPESHIFT) & _IOC_TYPEMASK) */
	if (_IOC_TYPE(cmd) != IOC_MAGIC){
		printk(KERN_WARNING "This ioctl rule is not this module\n");
		return -ENOTTY;
	}
 
	/* Check cmd is exist: IOC_NR := (((nr) >> _IOC_NRSHIFT) & _IOC_NRMASK) */
	if (_IOC_NR(cmd) > IOC_MAXNR) {
		printk(KERN_WARNING, "This ioctl rule is not exist\n");
		return -ENOTTY;
	}

    switch (cmd) {
	    case SETARRAYTOKERNEL:
			if (copy_from_user(arr, (int __user*)arg, 12))
				return -EFAULT;
			break;
		case SETBREAKFIRST:
			ret = __get_user(breakfast, (int __user*)arg);
			break;
		case SETDINNER:
			ret = __get_user(dinner , (int __user*)arg);
			break;
		default:
			return -ENOTTY;

	}
	printk(KERN_INFO "num[0]: %d\nnum[1]: %d\n, num[2]: %d\n", arr[0], arr[1], arr[2]);
	printk(KERN_INFO "breakfase: %d\ndinner: %d\n", breakfast, dinner);

	return ret;
}

static int helloWorld_release(struct inode *inode, struct file *filp) 
{
	return 0;
}

struct file_operations helloWorld_fops = {
	.owner = THIS_MODULE,
	.open = helloWorld_open,
	.read = helloWorld_read,
	.write = helloWorld_write,
	.release = helloWorld_release,
	.unlocked_ioctl = helloWorld_ioctl,
};

struct helloWorld_object {
	struct cdev cdev;
};

static int __init  init_function(void)
{
	int result;
	
	/* helloWorld_dev := device number come from major and minor combine */
	result = alloc_chrdev_region(&helloWorld_dev, 0, 1, "helloWorld");
	if (result < 0) {
		printk(KERN_WARNING "helloWorld: can't not alloc get major %d", helloWorld_major);
		return result;
	}
	/* apply cdev structure to store method of file operations */
	helloWorld_cdev = cdev_alloc();
	if (!helloWorld_cdev) {
		printk(KERN_WARNING "alloc_cdev() fail\n");
		result = -1;
		goto failed_cdev;
	}
	helloWorld_cdev->ops = &helloWorld_fops;

	/* Add char device use helloWorld_cdev method and device number and how many devices can handle */
	result = cdev_add(helloWorld_cdev, helloWorld_dev, 1);
	if (result < 0) {
		printk(KERN_WARNING "cdev_add() fail\n");
		result = -2;
		goto failed_cdev;
	}
	
	/* Build /sys/ catagory and device create can use the class */
	helloWorld_class = class_create(THIS_MODULE, "helloWorld");	
	if (!helloWorld_class) {
		printk(KERN_WARNING "class_create fail\n");
		result = -3;
		goto failed_class_create;
	}

	if (!device_create(helloWorld_class, NULL, helloWorld_dev, NULL, "helloWorld")) {
		printk(KERN_WARNING "device_create fail\n");
		result = -4;
		goto failed_device_create;
	}

	printk(KERN_INFO "Hello World\n");
    return result;
	

failed_device_create:
	class_destroy(helloWorld_class);
failed_class_create:
	cdev_del(helloWorld_dev);
failed_cdev:
	unregister_chrdev_region(helloWorld_dev, 1);

	return result;
}

static void __exit exit_function(void)
{
	device_destroy(helloWorld_class, helloWorld_dev);
	class_destroy(helloWorld_class);
	cdev_del(helloWorld_cdev);
	unregister_chrdev_region(helloWorld_dev, 1);
	printk(KERN_INFO "Goodbye World\n");
}	

module_init(init_function);
module_exit(exit_function);

MODULE_AUTHOR("Peter ");
MODULE_LICENSE("GPL");
