TARGET := helloWorld
TESTOBJ := client.o
TESTSRC := client.c
obj-m :=  $(TARGET).o

CC := gcc
CFLAG := -O

KERNELDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all: test
	make -C $(KERNELDIR) M=$(PWD) modules
    	
install: 
	sudo insmod $(TARGET).ko

uninstall:
	sudo rmmod $(TARGET)

test: $(TESTOBJ)
$(TESTOBJ):$(TESTSRC)
	$(CC) -o $@ $^


clean: 
	rm -f *.mod.cmd  *.ko.cmd *.o  *.dwo  *.ko  *.mod *.mod.c  *.mod.dwo  *.mod.o  *.o  *.order  *.symvers  .$(TARGET).*  .modules.order.cmd  .Module.symvers.cmd
