obj-m := kbdh.o
kbdh-y := kbd_handler.o ./src/keyp.o ./src/fs.o
EXTRA_CFLAGS += -Werror -Wall -Wno-missing-braces -O2 -Wno-error=unused-function

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

install:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules_install

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
