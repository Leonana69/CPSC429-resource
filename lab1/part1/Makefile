obj-m += hello.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

test:
	@# clear message
	sudo dmesg -C
	@# insert module
	sudo insmod hello.ko
	@# remove module
	sudo rmmod hello.ko
	@# check message
	dmesg