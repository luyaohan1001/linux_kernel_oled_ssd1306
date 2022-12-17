## Linux Loadable Kernel Module Driver for SSD1306 OLED.

    Tested on Linux raspberrypi 5.10.103-v7l+ #1529 SMP Tue Mar 8 12:24:00 GMT 2022 armv7l GNU/Linux 
    (Raspberry Pi Buster.)

    Documents generated (by doxygen) at /docs/html/files.html

#### To compile 

    Install the kernel headers.
    $ sudo apt install raspberrypi-kernel-headers

    Compile
    $ sudo make

#### To run:

    1. First apply device tree overlay by

        $ sudo make dtoverlay

    2. Insert the kernel module

        $ sudo make insmod

#### To check for printk log:

        $ dmesg

#### To remove the kernel module:

        $ sudo dmesg

#### To generate docs by doxygen

        $ make doxygen

        $ cd /docs/html

#### Kanban - TODO

- [x] release-00: Minimal-viable kernel i2c bus module and simple configruation + fill-screen.

- [ ] release-01: Add font / image support to the screen datalink layer.

- [ ] release-02: Add user-space interface through sysfs.

- [ ] release-03: Develop the dinosaur game on this screen.

