/**
 * @file oled_sysfs.c
 * @brief Headers to expose user-control through sysfs filesystem.
 * @author Luyao Han (luyaohan1001@gmail.com)
 * @date 12-21-2022
 */

#ifndef OLED_SYSFS_H
#define OLED_SYSFS_H

/**
 * @brief Creates kobject and its attributes under sysfs.
 * @param None.
 * @return status_code.
 */
int oled_sysfs_init(void);

/**
 * @brief Cleans up the constructs created in oled_sysfs_init.
 *        Deletes the kernel object allocated and the sysfs folder created for
 * oled_kobj.
 * @param  None.
 * @return None.
 */
void oled_sysfs_deinit(void);

#endif /* OLED_SYSFS_H */