/**
 * @file oled_sysfs.c
 * @brief Init/deinit callbacks implementation to expose user-control through
 * sysfs filesystem.
 * @author Luyao Han (luyaohan1001@gmail.com)
 * @date 12-21-2022
 */

#include "oled_sysfs.h"

#include <linux/kobject.h>

/* Function signatures. */
static ssize_t kobj_attr_display_text_show(struct kobject *kobj,
                                           struct kobj_attribute *attr,
                                           char *buffer);
static ssize_t kobj_attr_display_text_store(struct kobject *kobj,
                                            struct kobj_attribute *attr,
                                            const char *buffer, size_t count);

/**
 * @brief The pointer storing a oled kernel object to be created later.
 * @note The kobject, once created, will show up as a directory under
 * /sys/kernel/.
 */
struct kobject *oled_kobj;

/**
 * @brief "display_text" attribute, storing the current text the oled
 * displaying.
 * @note  "display_text" will show up as a file under /sys/kernel/oled_sysfs.
 */
static struct kobj_attribute kobj_attr_display_text = {
    .attr = {.name = "display_text", .mode = 0666},
    .show = kobj_attr_display_text_show,
    .store = kobj_attr_display_text_store};

/**
 * @brief Callback function prototype for when the user read display_text, i.e.
 * cat /sys/kernel/oled_sysfs/display_text. The prototype implements the
 * following function pointer in struct kobj_attribute in linux/kobject.h:
 *        ssize_t (*show)(struct kobject *kobj, struct kobj_attribute *attr,
 * char *buf);
 * @param kobj Kobject to which tied sysfs file is read (show).
 * @param attr Attr.com/ibute to which the tied sysfs file is read (show).
 * @param buffer Text display to the screen when the file is read.
 * @return Error status.
 */
static ssize_t kobj_attr_display_text_show(struct kobject *kobj,
                                           struct kobj_attribute *attr,
                                           char *buffer) {
  ssize_t status_code = 0;

  /* TODO: Implement book-keeping of attribute values in datalink and use
   * sprintf to print to buffer. */
  // status_code = sprintf(buffer, "display_text kobj_attribute:\n%s/%s\n",
  // kobj->name, attr->attr.name);
  pr_info("/sys/kernel/%s/%s is successfully read through function "
          "kobj_attr_display_text_show. \n",
          kobj->name, attr->attr.name);
  return status_code;
}

/**
 * @brief Callback function prototype for when the user write to the
 * display_text, i.e. echo "hello, world" > /sys/kernel/oled_sysfs/display_text.
 *        The prototype implements the following function pointer in struct
 * kobj_attribute in linux/object.h.
 * @param kobj Kobject to which tied sysfs file is written (store).
 * @param attr Attribute to which the tied sysfs file is written (store).
 * @param buffer Text display to the screen when the file is written.
 * @return Number of characters written.
 * @note   Returning status code is wrong, and could cause the system looping in store function.
 */
static ssize_t kobj_attr_display_text_store(struct kobject *kobj,
                                            struct kobj_attribute *attr,
                                            const char *buffer, size_t count) {
  pr_info("'%s' has been written to /sys/kernel/%s/%s through "
          "kobj_attr_display_text_store. \n",
          buffer, kobj->name, attr->attr.name);
  return count;
}

/**
 * @brief Creates kobject and its attributes under sysfs.
 * @param None.
 * @return status_code.
 */
int oled_sysfs_init(void) {
  int status_code = 0;
  const char dir_name[] = "oled_sysfs";
  struct kobject *parent = kernel_kobj;

  /* Register kobject for sysfs, to expose control from user space. */
  pr_info("Now creating sysfs directory path for oled device.\n");

  /* Creating an kobject dynamically with register it with sysfs. */
  /* The directory /sys/kernel/oled_sysfs will be created. */
  oled_kobj = kobject_create_and_add(dir_name, parent);

  /* Check create status. */
  if (NULL == oled_kobj) {
    pr_err("Error creating kobject for oled_sysfs, exiting...\n");
    status_code = -1;
    goto RETURN;
  }

  /* Create file under oled_sysfs directory. */;
  status_code = sysfs_create_file(oled_kobj, &kobj_attr_display_text.attr);

  if (status_code != 0) {
    pr_err("Error creating sysfs attribute file display_text, exiting...\n");

    /* Dynamically frees oled_kobj allocated by kobject_create_and_add. */
    kobject_put(oled_kobj);
    status_code = -1;
    goto RETURN;
  }
RETURN:
  if (0 == status_code) {
    pr_info("oled_sysfs kobject has been successfully created.\n");
    pr_info("    -->$ cd /sys/kernel/oled_sysfs to start playing with it.\n");
  }
  return status_code;
}

/**
 * @brief Cleans up the constructs created in oled_sysfs_init.
 *        Deletes the kernel object allocated and the sysfs folder created for
 * oled_kobj.
 * @param  None.
 * @return None.
 */
void oled_sysfs_deinit(void) {
  /* Print to kernel logs. */
  pr_info("Deleting oled_sysfs kobject. \n");

  /* Remove display_text file from sysfs. */
  sysfs_remove_file(oled_kobj, &kobj_attr_display_text.attr);

  /* Removes kobject from sysfs, which also deletes the oled_sysfs directory in
   * /sys/kernel/. */
  kobject_put(oled_kobj);
}