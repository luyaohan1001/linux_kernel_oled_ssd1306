/**
 * @file oled_sysfs.c
 * @brief Headers to expose user-control through sysfs filesystem.
 * @author Luyao Han (luyaohan1001@gmail.com)
 * @date 12-21-2022
 */

#ifndef OLED_SYSFS_H
#define OLED_SYSFS_H

int oled_sysfs_init(void);
void oled_sysfs_deinit(void);

#endif /* OLED_SYSFS_H */