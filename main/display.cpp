#include "display.h"

#include <Arduino.h>
#include <TFT_eSPI.h>

namespace {
    TFT_eSPI tft;
}

void display_init()
{
    // CYD backlight
    pinMode(21, OUTPUT);
    digitalWrite(21, HIGH);

    Serial.begin(115200);
    Serial.println("display_init");

    tft.init();
    tft.setRotation(3);// originally 1 

    tft.fillScreen(TFT_BLACK);

    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
}

void display_clear()
{
    tft.fillScreen(TFT_BLACK);
}

void display_fill(uint16_t color)
{
    tft.fillScreen(color);
}

void display_set_cursor(int x, int y)
{
    tft.setCursor(x, y);
}

void display_set_text_size(uint8_t size)
{
    tft.setTextSize(size);
}

void display_set_text_color(uint16_t fg, uint16_t bg)
{
    tft.setTextColor(fg, bg);
}

void display_print(int x, int y, const char* text)
{
    tft.setCursor(x, y);
    //tft.print(text);
    tft.drawString(text, x, y);
}

void display_println(const char* text)
{
    
    tft.println(text);
}

void display_rect(int x, int y, int w, int h, uint16_t color)
{
    tft.drawRect(x, y, w, h, color);
}

void display_fill_rect(int x, int y, int w, int h, uint16_t color)
{
    tft.fillRect(x, y, w, h, color);
}

void display_line(int x0, int y0, int x1, int y1, uint16_t color)
{
    tft.drawLine(x0, y0, x1, y1, color);
}