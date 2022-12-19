/**
 * @file graphics.h
 * @brief ssd1306 OLED graphics display APIs header.
 */
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "datalink.h"

#define OLED_CANVAS_WIDTH_PIXELS 128
#define OLED_CANVAS_HEIGHT_PIXELS 64
#define BITS_PER_BYTE 8

#define OLED_COLUMN_LENGTH OLED_CANVAS_WIDTH_PIXELS
#define OLED_COLUMN_MIN 0
#define OLED_COLUMN_MAX OLED_CANVAS_WIDTH_PIXELS - 1

#define OLED_PAGE_LENGTH (OLED_CANVAS_HEIGHT_PIXELS / BITS_PER_BYTE)
#define OLED_PAGE_MIN 0
#define OLED_PAGE_MAX (OLED_CANVAS_HEIGHT_PIXELS / BITS_PER_BYTE) - 1

typedef struct {
  uint8_t line;
  uint8_t cursor_position;
  uint8_t font_char_width;
} oled_graphics_params_t;

void oled_putc(unsigned char c);
void oled_printf(const char *format, ...);
void oled_new_line(void);
void oled_set_cursor(uint8_t lineNo, uint8_t cursorPos);
void oled_fill_all(uint8_t data);
void oled_draw_dino_map(void);

#endif /* GRAPHICS_H */