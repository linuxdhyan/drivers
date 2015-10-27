#include <linux/init.h>

/* module_license () macro defined in this    */
#include <linux/module.h>

/*to use the printk function use kernel .h ,
/ printk()feeds kernel messages to the console, d
 mesg, and the syslog daemon */
#include <linux/kernel.h>          

/* macro  used to tell the kernel that this module
bears a free license .without  it will show error   */
MODULE_LICENSE("Dual BSD/GPL");

static int hello_init(void)
{
     printk(KERN_ALERT "Hello, world\n");
     
/*A non 0 return means init_module failed; module can't be loaded. */
    return 0;
}
static void hello_exit(void)
{
/* it is print fn used in kernel ,because kernel  
* it runs by itself, without the help of the C library
The string KERN_ALERT is the priority of the message. */
printk(KERN_ALERT "Goodbye, cruel world\n");
}
/*module_init and module_exit lines use special kernel macros ,
* which shows which function to be called when module inserted and removed .*/
 
module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
