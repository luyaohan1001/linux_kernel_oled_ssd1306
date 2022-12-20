# Makefile for SSD1306 OLED Kernel module driver.

# Kernel module compiled expected name oled_driver.ko.
obj-m := oled_driver.o

# The target has two objects
oled_driver-objs := driver.o datalink.o graphics.o oled_sysfs.o

# Run make install-headers to install kernel headers. (This is only tested on Raspbian Buster)
KERNEL_DIR ?= /usr/src/linux-headers-$(shell uname -r)

# (C)hange to kernel directory, M is a variable pointing to output directory not a flag of make.
all:
	make -C $(KERNEL_DIR) \
		ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- \
		M=$(PWD) modules

clean:
	make -C $(KERNEL_DIR) \
		ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- \
		M=$(PWD) clean
	rm -rf *.dtbo

.PHONY: clean compile_dtbo dtoverlay insmod rmmod doxygen setup format

# Setup compile environment.
setup:
	apt install raspberrypi-kernel-headers
	apt install doxygen
	sudo apt-get install -y doxygen-latex

# Compile device tree overlay source.
compile_dtbo:
	dtc oled.dts -o oled.dtbo

# Add the device tree overlay.
dtoverlay: compile_dtbo
	sudo dtoverlay oled.dtbo

# Insert this kernel module.
insmod: dtoverlay
	insmod oled_driver.ko

# Remove this kernel module.
rmmod:
	rmmod oled_driver

# Generate documents with doxygen.
doxygen:
	doxygen doxygen.config
	cd docs/latex && make pdf

# Format the source codes.
format:
	clang-format -i ./*.[ch]


