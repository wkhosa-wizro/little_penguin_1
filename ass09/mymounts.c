#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/fs_struct.h>
#include <linux/mount.h>
#include <linux/proc_fs.h>
#include <linux/nsproxy.h>
#include <linux/list.h>
#include <linux/namei.h>

//struct	namespace	*n_space = current->namespace;
struct	mount		*mnt;
struct	proc_dir_entry	*proc_entry;

static	ssize_t	ft_read(struct file *filept, char __user *buff,
		size_t len, loff_t *f_pos)
{

	struct	dentry	*curdir;

	list_for_each_entry(curdir,
			&current->fs->root.mnt->mnt_root->d_subdirs,
			d_child) {
		if (curdir->d_flags & DCACHE_MOUNTED) {
			pr_info("%s \t /%s\n",
					curdir->d_name.name,
					curdir->d_iname);
		}
	}
	return 0;
}

static	const struct file_operations ft_ops = {
	.owner = THIS_MODULE,
	.read = ft_read,
};

static	int __init mymount_init(void)
{
	proc_entry = proc_create("mymounts", 0444, NULL, &ft_ops);
	if (!proc_entry) {
		pr_err("Failed to create dir in /proc\n");
		return -ENOMEM;
	}
	return 0;
}

static	void __exit mymount_exit(void)
{
	proc_remove(proc_entry);
}

module_init(mymount_init);
module_exit(mymount_exit);

MODULE_AUTHOR("wkhosa <wkhosa@wtc.co.za>");
MODULE_DESCRIPTION("assg 09");
MODULE_LICENSE("GPL v2");
