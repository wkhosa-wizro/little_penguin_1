#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/module.h>

#define LOGIN_ID "wkhosa"
#define BUFF_LEN 8

static	char	kern_buff[BUFF_LEN];
static	const	char	student_id[] = "wkhosa\n";

static	ssize_t ft_write(struct file *filpt, const char __user *buff,
		size_t len, loff_t *f_pos)
{
	ssize_t	bytes_read;

	if (len != (strlen(LOGIN_ID) + 1)) {
		pr_err("wrong length of input len %ld\n", len);
		return -EINVAL;
	}
	if (strncmp(LOGIN_ID, buff, strlen(LOGIN_ID)) != 0)  {
		pr_err("incorrect login user name\n");
		return -EINVAL;
	}
	bytes_read = simple_write_to_buffer(kern_buff, sizeof(kern_buff),
			f_pos, buff, len);
	kern_buff[len] = '\0';
	return bytes_read;
}

static	ssize_t ft_read(struct file *filpt, char __user *buff,
		size_t len, loff_t *f_pos)
{
	return simple_read_from_buffer(buff, strlen(student_id), f_pos,
			student_id, strlen(student_id));
}

static	const struct file_operations ft_fops = {
	.owner	= THIS_MODULE,
	.write	= ft_write,
	.read	= ft_read,
};

static	struct miscdevice ft_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "fortytwo",
	.fops = &ft_fops,
};

static	int __init misc_init(void)
{
	int	error;

	error = misc_register(&ft_device);
	if (error) {
		pr_err("failed to register device :\n");
		return error;
	}
	return 0;
}

static	void __exit misc_exit(void)
{
	misc_deregister(&ft_device);
}

module_init(misc_init);
module_exit(misc_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("simple misc device driver");
MODULE_AUTHOR("wisani khosa <wkhosa@wtc.co.za>");
