## GNU/Linux Loadable Kernel Module Driver for SSD1306 OLED/PLED Driver.

    Source code hierarchy:

                    oled_sysfs
                        |
                     graphics
                        |
                     datalink
                        |
                  driver  oled.dts

    Tested on Linux raspberrypi 5.10.103-v7l+ #1529 SMP Tue Mar 8 12:24:00 GMT 2022 armv7l GNU/Linux.
    (Raspberry Pi Buster.)

    PDF documents generated (by doxygen) at /docs/latex/refman.pdf
    To read the compiled pdf, git clone this repository, and open the softlink oled_driver_manual.pdf.

#### Demo: Displaying text and the dinosaur graphics from chrome browser.
![](docs/misc/demo_graphics.jpg)

#### Demo: Displaying oled_sysfs (kobject-mapped directory) from terminal.
![](docs/misc/demo_oled_sysfs.png)

#### Documentation.

![Click for full documentation](docs/latex/refman.pdf)

#### To compile.

    Setup compile environment
    $ sudo make setup

    Compile
    $ sudo make

    Successful compile message example:
    pi@raspberrypi:~/Projects/raspberrypi-4b/drivers/oled $ make
        make -C /usr/src/linux-headers-5.10.103-v7l+ \
                ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- \
                M=/home/pi/Projects/raspberrypi-4b/drivers/oled modules
        make[1]: Entering directory '/usr/src/linux-headers-5.10.103-v7l+'
          CC [M]  /home/pi/Projects/raspberrypi-4b/drivers/oled/oled_sysfs.o
          LD [M]  /home/pi/Projects/raspberrypi-4b/drivers/oled/oled_driver.o
          MODPOST /home/pi/Projects/raspberrypi-4b/drivers/oled/Module.symvers
          CC [M]  /home/pi/Projects/raspberrypi-4b/drivers/oled/oled_driver.mod.o
          LD [M]  /home/pi/Projects/raspberrypi-4b/drivers/oled/oled_driver.ko
        make[1]: Leaving directory '/usr/src/linux-headers-5.10.103-v7l+'

#### To run:

    Insert the kernel module

        $ sudo make insmod

#### To remove the kernel module:

        $ sudo make rmmod


#### To check for printk log:

        $ dmesg

#### To generate docs by doxygen

        $ make doxygen

        $ cd /docs/html

#### Kanban - TODO

- [x] release-00: Minimal-viable kernel i2c bus module and simple configruation + fill-screen function.
    - Constructing Makefile, setup build-environment (linux kernel headers)
    - Understanding struct i2c_client , struct i2c_driver.
    - Implementing probe and remove callbacks when the kernel inserts/remove the driver.

- [x] release-01: Add font / image support to the screen datalink layer.
    - Reading and implementing various display utitlities (change line, clear screen, set coordinate...etc) according to SSD1306 datasheet by Solomon Systech.

- [x] release-02: Add oled_sysfs as an interface to user-space.
    - Understanding struct kobject, kobj_attrbute.
    - Implementing the creation of the oled device as a sysfs folder.
    - Implementing the creation of oled attributes such as display_text, brightness, etc. as files in that sysfs folder.
    - Implementing kthread in driver.c that flushes display_text stored through sysfs onto the screen.

- [ ] release-03: Develop the dinosaur game displaying on this screen.
    - Add multi-threading protection to critical sections.
    - Developing the Chrome dinosaur game, interacting with the kernel module through oled_sysfs.

- [ ] release-04: Unit testing.
    - TBD

#### An important lesson.

    When refactoring function oled_draw_bitmap in graphics, passing 2D array (bitmap images encoded in hex) caused NULL pointer-dereference and triggered kernel Oops.

![](docs/misc/2d_array_mistake_oops.png)

    It must be caution that a Double Pointer (i.e. **p) is NOT equvilent to a 2D-Array.

    When passing a 2d array, we pass it as a single pointer.

    For example, for bitmap[M][N], when can pass it to a function, we can pass as:

    draw_bitmap((char const *)bitmap, max_rows, max_cols).

    We can then access its element by:

    bitmap(M, N) = *(bitmap + M * max_cols + N), where M indicates the Mth row, N indicates the Nth column.

        bitmap ==> Start address

        M * max_cols ==> skipped whole rows

        N ==>  column offset