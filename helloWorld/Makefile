TARGET := helloWorld
TESTOBJ := client.o
TESTSRC := client.c
obj-m :=  $(TARGET).o

CPPCHECK := 1
CC := gcc
CFLAG := -O

KERNELDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all: check  
	make -C $(KERNELDIR) M=$(PWD) modules
	    	
clean: uninstall 
	rm -f out *.mod.cmd  *.ko.cmd *.o  *.dwo  *.ko  *.mod *.mod.c  *.mod.dwo  *.mod.o  *.o  *.order  *.symvers  .$(TARGET).*  .modules.order.cmd  .Module.symvers.cmd

check:
ifeq ($(CPPCHECK), 1)
	cppcheck ./
endif

install: 
ifeq ($(shell lsmod | grep hello), )
	sudo insmod $(TARGET).ko
endif

uninstall:
ifneq ($(shell lsmod | grep hello), )
	sudo rmmod $(TARGET)
endif

test: all $(TESTOBJ) install use
$(TESTOBJ):$(TESTSRC)
	$(CC) -o $@ $^

use: 
	sudo ./$(TESTOBJ) > out
