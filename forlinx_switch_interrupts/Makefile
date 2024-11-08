obj-m += io_interrupts.o

# Path to the kernel source for your ARM64 target
KERNEL_SOURCE := /home/nurbol/Desktop/forlinx-kernel/linux-imx-bk

# Cross-compilation settings
ARCH := arm64
CROSS_COMPILE := aarch64-linux-gnu-

all:
	$(MAKE) -C $(KERNEL_SOURCE) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules

clean:
	$(MAKE) -C $(KERNEL_SOURCE) M=$(PWD) clean