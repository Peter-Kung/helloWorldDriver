#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

static int __init init_function(void) 
{
    printk(KERN_INFO "Hello world\n");
    pr_info("Hello World!\n");
    return 0;
}

static void __exit exit_function(void)
{
    pr_info("Goodbye\n");
    return;
}

module_init(init_function);
module_exit(exit_function);

MODULE_AUTHOR("peter");
MODULE_LICENSE("GPL");
