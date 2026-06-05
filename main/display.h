#pragma once

#include <stdint.h>

// Initialization
void display_init();

// Screen operations
void display_clear();
void display_fill(uint16_t color);

// Text
void display_print(int x, int y, const char* text);
void display_println(const char* text);
void display_set_cursor(int x, int y);
void display_set_text_size(uint8_t size);
void display_set_text_color(uint16_t fg, uint16_t bg = 0x0000);

// Drawing
void display_rect(int x, int y, int w, int h, uint16_t color);
void display_fill_rect(int x, int y, int w, int h, uint16_t color);
void display_line(int x0, int y0, int x1, int y1, uint16_t color);

// Common colors (RGB565)
constexpr uint16_t COLOR_BLACK = 0x0000;
constexpr uint16_t COLOR_WHITE = 0xFFFF;
constexpr uint16_t COLOR_RED   = 0xF800;
constexpr uint16_t COLOR_GREEN = 0x07E0;
constexpr uint16_t COLOR_BLUE  = 0x001F;
constexpr uint16_t COLOR_YELLOW = 0xFFE0;
constexpr uint16_t COLOR_CYAN   = 0x07FF;
constexpr uint16_t COLOR_MAGENTA = 0xF81F;
