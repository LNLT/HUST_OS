obj-m := adddriver.o    #这里是上面所创建的c文件名.o
    PWD := $(shell pwd)
    KERNELDIR := /usr/src/linux-4.14.225    #你要安装mod的内核版本 
all:
	make -C $(KERNELDIR) M=$(PWD) modules
 
 
.PHONY: clean
clean:
	rm -rf *.o *~ core *.ko *.mod.c modules.order Module.symvers
