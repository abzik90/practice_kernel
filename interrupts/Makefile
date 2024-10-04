obj-m := interrupt_practice.o

all:
	make -C /lib/modules/5.15.167/build M=$(PWD) modules
clean:
	make -C /lib/modules/5.15.167/build M=$(PWD) clean
