obj-m := kbdh.o
kbdh-y := kbd_handler.o src/keyp.o src/fs.o
EXTRA_CFLAGS += -I$(M)/include -Werror -Wall -O2 -c -D__KERNEL__ \
				-Wno-missing-braces -Wno-error=unused-function \
					-DMODULE

all:
	$(MAKE) -C /lib/modules/$$(uname -r)/build M=$(CURDIR) modules

clean:
	$(MAKE) -C /lib/modules/$$(uname -r)/build M=$(CURDIR) clean