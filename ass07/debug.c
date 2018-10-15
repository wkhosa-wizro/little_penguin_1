/*
 * simple debugfs
 */

#include <linux/debugfs.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/jiffies.h>

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("Assignment 07 about debugfs");
MODULE_AUTHOR("wisani khosa <wkhosa@wtc.co.za>");

#define LOGIN_ID "wkhosa"
#define BUFF_LEN 8

static	char	kern_buff_foo[PAGE_SIZE];
static	char	kern_buff_id[BUFF_LEN];
static	const	char student_id[] = "wkhosa\n";
int	filevalue;
struct	dentry *dirret;

static	ssize_t ft_write_id(struct file *filpt, const char __user *buff,
		size_t len, loff_t *f_ops)
{
	ssize_t	bytes_read;

	if (len != (strlen(LOGIN_ID) + 1)) {
		pr_err("wrong length of input len %ldn", len);
		return -EINVAL;
	}
	if (strncmp(LOGIN_ID, buff, strlen(LOGIN_ID)) != 0) {
		pr_err("incorrect login\n");
		return -EINVAL;
	}
	bytes_read = simple_write_to_buffer(kern_buff_id, sizeof(kern_buff_id),
			f_ops, buff, len);
	kern_buff_id[len] = '\0';
	return bytes_read;
}

static	ssize_t ft_read_id(struct file *filpt, char __user *buff,
		size_t len, loff_t *f_pos)
{

	return simple_read_from_buffer(buff, strlen(student_id), f_pos,
			student_id, strlen(student_id));
}

static	const struct file_operations ft_fops_id = {
	.owner = THIS_MODULE,
	.read = ft_read_id,
	.write = ft_write_id,
};

static	ssize_t ft_read_jiffies(struct file *filpt, char __user *buff,
		size_t len, loff_t *f_pos)
{
	char		temp[sizeof(long) + 1];
	unsigned	long jiffies_time;
	size_t		bytes_stored;

	jiffies_time = get_jiffies_64();
	bytes_stored = snprintf(temp, sizeof(temp), "%lu", jiffies_time);
	return simple_read_from_buffer(buff, len, f_pos,
			temp, bytes_stored);
}

static	const struct file_operations ft_fops_jiffies = {
	.owner = THIS_MODULE,
	.read = ft_read_jiffies,
};

static	ssize_t ft_write_foo(struct file *filpt, const char __user *buff,
		size_t len, loff_t *f_ops)
{
	int	bytes_read;

	bytes_read = simple_write_to_buffer(kern_buff_foo,
			sizeof(kern_buff_foo), f_ops, buff, len);
	kern_buff_foo[len] = '\0';
	return bytes_read;
}

static	ssize_t ft_read_foo(struct file *filpt, char __user *buff,
		size_t len, loff_t *f_pos)
{

	return simple_read_from_buffer(buff, strlen(kern_buff_foo), f_pos,
			kern_buff_foo, strlen(kern_buff_foo));
}

static	const struct file_operations ft_fops_foo = {
	.owner = THIS_MODULE,
	.read = ft_read_foo,
	.write = ft_write_foo,
};
static	int __init ft_debug_init(void)
{

	dirret = debugfs_create_dir("fortytwo", NULL);
	debugfs_create_file("id", 0666, dirret, &filevalue, &ft_fops_id);
	debugfs_create_file("jiffies", 0444, dirret, &filevalue,
			&ft_fops_jiffies);
	debugfs_create_file("foo", 0644, dirret, &filevalue, &ft_fops_foo);
	pr_info("module initialising hello\n");
	return 0;
}

static	void __exit ft_debug_exit(void)
{
	debugfs_remove_recursive(dirret);
	pr_info("module unloading goodbye\n");
}

module_init(ft_debug_init);
module_exit(ft_debug_exit);
