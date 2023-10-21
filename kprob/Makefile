CONFIG_MODULE_SIG=n

TARGET := kprobe
obj-m := $(TARGET).o

KERNELDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all: 
	make -C $(KERNELDIR) M=$(PWD) modules
	make install

clean: uninstall
	rm -f out *.mod.cmd  *.ko.cmd *.o  *.dwo  *.ko  *.mod *.mod.c  *.mod.dwo  *.mod.o  *.o  *.order  *.symvers  .$(TARGET).*  .modules.order.cmd  .Module.symvers.cmd

install: 
ifeq ($(shell lsmod | grep $(TARGET)), )
	sudo insmod $(TARGET).ko
endif

uninstall:
ifneq ($(shell lsmod | grep $(TARGET)), )
	sudo rmmod $(TARGET)
endif

