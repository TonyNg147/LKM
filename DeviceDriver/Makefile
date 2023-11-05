obj-m:=DeviceDriver.o

KERNEL_MAKE = /lib/modules/`uname -r`/build

all :
	make -C $(KERNEL_MAKE) M=$(shell pwd) 

clean :
	make -C $(KERNEL_MAKE) M=$(shell pwd) clean
