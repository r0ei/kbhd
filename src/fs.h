#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

#define MAX_PROC_SIZE   200

ssize_t
kbd_fs_read(struct file *file, char __user *buf, size_t len, loff_t *offset);

void append_to_keys(char *value);

extern const struct proc_ops proc_fops_keys;
extern char keys[MAX_PROC_SIZE];