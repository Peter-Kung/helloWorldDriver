TARGET := blockDriver
obj-m += $(TARGET).o
CPPCHECK := 1
CC := gcc
CFLAG := -g -o
KERNELDIR := /lib/modules/$(shell uname -r)/build
PWD    	  := $(shell pwd)

all: check
	make -C $(KERNELDIR) M=$(PWD) modules
	$(CC) client.c $(CFLAG) ./client
	$(CC) server.c $(CFLAG) ./server

clean: uninstall
	rm -f out *.mod.cmd  *.ko.cmd *.o  *.dwo  *.ko  *.mod *.mod.c  *.mod.dwo  *.mod.o  *.o  *.order  *.symvers  .$(TARGET).*  .modules.order.cmd  .Module.symvers.cmd ./client ./server

check:
ifeq ($(CPPCHECK), 1)
	cppcheck ./
endif

install:
ifeq ($(shell lsmod | grep $(TARGET)), )
	sudo insmod $(TARGET).ko
endif

uninstall:
ifneq ($(shell lsmod | grep $(TARGET)), )
	sudo rmmod $(TARGET)
endif

test: all install
	sudo ./client &
	sudo ./server
