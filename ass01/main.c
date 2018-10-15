#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("wisani khosa <wkhosa@wtc.co.za>");
MODULE_DESCRIPTION("Simple hello module");

static	int hello_entry(void)
{
	pr_info("Hello world !\n");
	return 0;
}

static	void hello_exit(void)
{
	pr_info("Cleaning up module.\n");
}

module_init(hello_entry);
module_exit(hello_exit);
