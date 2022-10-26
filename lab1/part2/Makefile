.PHONY: all test
obj-m += mymem.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

test:
	gcc test.c -o test

load:
	sudo insmod mymem.ko
	sudo chmod 777 /dev/mymem

unload:
	sudo rmmod mymem.ko

load_test:
	@# clear message
	sudo dmesg -C
	@# insert module
	sudo insmod mymem.ko
	@# remove module
	sudo rmmod mymem.ko
	@# check message
	dmesg