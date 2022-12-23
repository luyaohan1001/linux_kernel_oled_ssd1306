/**
 * @file driver.c
 * @brief This file implements the necessary i2c_client probe and remove
 * callbacks on the SSD1306 I2C bus device driver. On top of driver.c, display
 * configurations and initialization are implemented in datalink.c. On top of
 * datalink, OLED printing / graphics are implemented in graphics.c
 * @author Luyao Han (luyaohan1001@gmail.com)
 * @date 12-21-2022
 */

#include "datalink.h"
#include "graphics.h"
#include "oled_sysfs.h"

#include <linux/delay.h>
#include <linux/i2c.h>
#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/sysfs.h>

/* Loadable kernel module license registration. */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Luyao Han");
MODULE_DESCRIPTION("Linux kernel module driver for ssd1306 oled display");

/* Function signatures. */
static int driver_on_probe(struct i2c_client *client,
                           const struct i2c_device_id *id);
static int driver_on_remove(struct i2c_client *client);
static int oled_display_text_thread(void *parameters);

/**
 * @brief Identifies the device (i.e. SSD1306 OLED contoller) connected to the
 * i2c bus.
 */
struct i2c_client *i2c_client;

/**
 * @brief Points to the oled_display_text_thread created.
 */
struct task_struct *handle_display_text_thread;

/**
 * @brief Link the symbol to its spawn in graphics.c
 */
extern oled_graphics_params_t oled_graphics_params;

/**
 * @brief Specifies the ".compatible" strings.
 * of_device_id array should store the same value as corresponding
 * node's "compatible" field in the device tree. In this case the oled.dts in
 * the same directory has the "compatible" field. When the .compatible field
 * here matches the device tree, the I2C device will be probed.
 */
static struct of_device_id driver_id[] = {
    {.compatible = "ssd1306, oled_device"}, {/*sentinel*/}};

/**
 * @brief This macro describes which devices each specific driver can support.
 * At compilation time, the build process extracts this information out of the
 * driver and builds a table.
 */
MODULE_DEVICE_TABLE(of, driver_id);

/**
 * @brief This array is pointed by the id_table field of struct i2c_driver.
 *          The id_table is used for non-DT based probing of I2C-devices.
 */

static struct i2c_device_id driver_device_id[] = {{"oled_device", 0},
                                                  {/*sentinel*/}};

MODULE_DEVICE_TABLE(i2c, driver_device_id);

/* Instantiate i2c driver. */
static struct i2c_driver i2c_driver = {
    /* Callbacks for the driver entry / exit. */
    .probe = driver_on_probe,
    .remove = driver_on_remove,
    .id_table = driver_device_id,
    .driver =
        {
            .name = "oled_device",
            .of_match_table = driver_id,
        },
};

/**
 * @brief Callback function pointer called on probing (driver-device binding) of
 * the device driver. This function implements the following prototype defined
 * struct i2c_driver in linux/i2c.h: int (*probe)(struct i2c_client *client,
 * const struct i2c_device_id *id);
 * @param client Pointer to the i2c_client instance.
 * @param device_id The device id to be probed.
 * @return Error status.
 */
static int driver_on_probe(struct i2c_client *client,
                           const struct i2c_device_id *device_id) {
  /* Store function return status. */
  int status_code = 0;

  pr_info("Entered driver_on_probe function\n");

  if (client->addr != 0x3c) {
    pr_info("Wrong I2C address.\n");
    status_code = -1;
    goto RETURN;
  } else {
    pr_info("SSD1306 OLED device driver has been successfully probed "
            "(inserted).\n");
  }

  /* Binding instance to the probed i2c client. */
  i2c_client = client;

  /* Entry to the OLED display logic. */
  ssd1306_controller_init();

  /* Invoke sysfs initialization from oled_sysfs.c. */
  oled_sysfs_init();

  /* Create thread for oled_display_text_task function and run it. */
  handle_display_text_thread =
      kthread_run(oled_display_text_thread, NULL, "display_text_thread");

RETURN:
  return status_code;
}

/**
 * @brief Callback function pointe called on the removal of the device driver.
 *        This function implements the following prototype defined struct
 * i2c_driver in linux/i2c.h: void (*remove)(struct i2c_client *client);
 * @param client Pointer to the i2c_client instance.
 * @return None.
 */
static int driver_on_remove(struct i2c_client *client) {
  int status_code = 0;
  /* Deinitialize oled_sysfs. */
  oled_sysfs_deinit();
  pr_info("oled_sysfs kobjects have been denintialized.\n");

  /* Stop all kernel threads. */
  status_code = kthread_stop(handle_display_text_thread);

  pr_info("oled driver kernel module has been removed.\n");
  // return status_code;
  return 0;
}

/* Helper macro for registering a modular I2C driver. */
module_i2c_driver(i2c_driver);

/**
 * @brief Thread implementing for deploying oled_graphics_params.display_text to
 * oled screen.
 * @param None.
 * @return None.
 */
static int oled_display_text_thread(void *parameters) {
  oled_cursor_coordinate_t cursor_coordinate;

  /* Clear the screen. */
  oled_fill_all(0x00);

  /* Draw a Chrome dinosaur on the screen. */
  cursor_coordinate.line = 2;
  cursor_coordinate.position = 40;
  oled_draw_dino_map(cursor_coordinate);

  while (true) {
    /* Print the display_text in graphics structure to the oled screen. */
    cursor_coordinate.line = 3;
    cursor_coordinate.position = 0;
    oled_set_cursor(cursor_coordinate);
    oled_printf(oled_graphics_params.display_text);

    msleep(100);

    /* When other threads calls kthread_stop on this thread. */
    if (kthread_should_stop() == true) {
      /* Exit this current thread.*/
      do_exit(0);
    }
  }
  return 0;
}