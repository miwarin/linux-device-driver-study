CFILES = myDeviceDriver.c
obj-m := MyDeviceModule.o
MyDeviceModule-objs := $(CFILES:.c=.o)

ccflags-y += -std=gnu99 -Wall -Wno-declaration-after-statement

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean


insmod:
	insmod MyDeviceModule.ko
	mknod --mode=666 /dev/mydevice0 c `grep MyDevice /proc/devices | awk '{print $$1;}'` 0
	mknod --mode=666 /dev/mydevice1 c `grep MyDevice /proc/devices | awk '{print $$1;}'` 1
	mknod --mode=666 /dev/mydevice2 c `grep MyDevice /proc/devices | awk '{print $$1;}'` 2


rmmod:
	rmmod MyDeviceModule
	rm /dev/mydevice*


