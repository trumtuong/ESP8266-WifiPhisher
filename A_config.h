#ifndef config_h
#define config_h

// ===== DISPLAY LIBRARY ===== //
#include "SH1106Spi.h"
#include "SH1106Wire.h"
#include "SSD1306Spi.h"
#include "SSD1306Wire.h"
#include <SPI.h>
#include <Wire.h>

#define USE_DISPLAY true // default display setting

// TTGO module
#define BUTTON_UP 12
#define BUTTON_DOWN 13
#define BUTTON_A 14
#define BUTTON_B 15
#define GPIO0

#define WAKE_UP_PIN 14
//#define DEAUTHER_DISPLAY SSD1306Wire display = SSD1306Wire(0x3c, 5, 4) // for
// 0.96" OLED
#define DEAUTHER_DISPLAY  SH1106Wire display = SH1106Wire(0x3c, 5, 4); // for 1.3" OLED


#endif
