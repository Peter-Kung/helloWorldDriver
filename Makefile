TARGET := helloWorld
obj-m :=  $(TARGET).o


KERNELDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all:
	make -C $(KERNELDIR) M=$(PWD) modules 

clean: 
	rm -f *.mod.cmd  *.ko.cmd *.o  *.dwo  *.ko  *.mod *.mod.c  *.mod.dwo  *.mod.o  *.o  *.order  *.symvers  .$(TARGET).*
