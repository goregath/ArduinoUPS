/*
 * Display.cpp
 *
 *  Created on: Mar 16, 2020
 *      Author: goregath
 */

#include <Arduino.h>
#include "Display.h"

#ifdef HAS_DISPLAY
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <uptime.h>

#define I2C_ADDRESS   0x3C
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 32
#define OLED_RESET    4

#define FXP_NOP UFixedDecimalHelpers::divide<1>
#define FXP_MILLIS UFixedDecimalHelpers::divide<1000>

inline float lm335_conv(const uint16_t &v) {
	// FIXME calculate precise temp
	// uncalibrated celsius
	return v / 10.0f - 273.15f;
}

#include "MegatecQ1UPS.h"
#include "UFixedDecimal.h"

#define bat_x 120
#define bat_y 2
#define bat_width 8
#define bat_height 11
#define bat_level_offset 22
#define bat_levels 9
#define bat_image_block_bytes 11
#define bat_px 968
static const uint8_t battery_xbm[] PROGMEM = {
	0x00, 0x00, 0x00, 0x88, 0x50, 0x20, 0x50, 0x88, 0x00, 0x00, 0x00, 0x70, 0xf8, 0x88, 0x88, 0x88,
	0x88, 0x88, 0x88, 0x88, 0x88, 0xf8, 0x70, 0xf8, 0x88, 0xa8, 0xa8, 0xa8, 0xa8, 0x88, 0xa8, 0x88,
	0xf8, 0x70, 0xf8, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0xf8, 0xf8, 0x70, 0xf8, 0x88, 0x88,
	0x88, 0x88, 0x88, 0x88, 0xf8, 0xf8, 0xf8, 0x70, 0xf8, 0x88, 0x88, 0x88, 0x88, 0x88, 0xf8, 0xf8,
	0xf8, 0xf8, 0x70, 0xf8, 0x88, 0x88, 0x88, 0x88, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0x70, 0xf8, 0x88,
	0x88, 0x88, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0x70, 0xf8, 0x88, 0x88, 0xf8, 0xf8, 0xf8, 0xf8,
	0xf8, 0xf8, 0xf8, 0x70, 0xf8, 0x88, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0x70, 0xf8,
	0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8 };

bool displayReady = false;

// 2832KSWE I2C Display
// Driver SH1106G 128x32 (SSD1306)
Adafruit_SSD1306 dsp(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void drawBattery(int level) {
   if(level < 0) {
      dsp.drawBitmap(bat_x, bat_y, battery_xbm, bat_width, bat_height, SSD1306_WHITE);
   } else {
      if(level >= bat_levels) level = bat_levels - 1;
      char offset = (char)(level) * bat_image_block_bytes;
      dsp.drawBitmap(bat_x, bat_y, (battery_xbm + bat_level_offset) + offset, bat_width, bat_height, SSD1306_WHITE);
   }
}

#endif

Display::Display(Configuration &conf) {}
Display::~Display() {}

void Display::init() {
#ifdef HAS_DISPLAY
	displayReady = dsp.begin(SSD1306_SWITCHCAPVCC, I2C_ADDRESS);
	dsp.display();
	dsp.setTextSize(1); // scale x1 of 5x8 font
	dsp.setTextColor(SSD1306_WHITE);
	dsp.cp437(true);
#endif
}

void Display::update(MegatecQ1UPS &ups) {
#ifdef HAS_DISPLAY
	static int lvl = 0;
	static uint16_t sc, mn, hr;
	static UFixedDecimal<2,0> fsc(sc, FXP_NOP);
	static UFixedDecimal<2,0> fmn(mn, FXP_NOP);
	static UFixedDecimal<2,0> fhr(hr, FXP_NOP);
	static uint16_t samples = 1;
	static uint32_t vccAvg = 0;
	static uint16_t vcc = 0;
	static UFixedDecimal<1,2> vbat(vcc, FXP_MILLIS);
	UFixedDecimal<2,1> ctmp(ups.vTmp, lm335_conv);
	uptime::calculateUptime();
	sc = uptime::getSeconds();
	mn = uptime::getMinutes();
	hr = uptime::getHours();
	if (! displayReady) return;
	dsp.clearDisplay();
	dsp.setTextColor(SSD1306_WHITE);
	dsp.setCursor(0, 0);
	dsp.println(String(uptime::getDays()) + F(" days"));
	dsp.print(fhr);
	dsp.println(F(" hours"));
	dsp.print(fmn);
	dsp.println(F(" minutes"));
	dsp.print(fsc);
	dsp.println(F(" seconds"));
	if (samples > 20) {
		vcc = vccAvg / samples;
		vccAvg = ups.vcc;
		samples = 1;
	} else {
		vccAvg += ups.vcc;
		++samples;
	}
	dsp.setCursor(128 - 24, 32 - 16);
	dsp.println(vbat);
	dsp.setCursor(128 - 24, 32 - 8);
	dsp.println(ctmp);
	drawBattery(map(sc % 6, 0, 5, 0, bat_levels));
	char stat[] = "   ";
	stat[0] = '-';
	stat[1] = ups.beeper.isActive() ? 'B' : '-';
	stat[2] = ups.state == RUNNING ? 'R' : 'S';
	dsp.fillRoundRect(bat_x - 36, bat_y, 24, 10, 3, SSD1306_WHITE);
	dsp.setCursor(bat_x - 32, bat_y + 1);
	dsp.setTextColor(SSD1306_BLACK);
	dsp.print(stat);
//	dsp.drawChar(bat_x - 8, bat_y + bat_height - 8,
//			ups.state == RUNNING ? 'R' : 'S', SSD1306_BLACK, 0, 1);
//	if (ups.beeper.isActive()) {
//		dsp.drawChar(bat_x - 16, bat_y + bat_height - 8, 'B', SSD1306_BLACK, 0, 1);
//	}
	dsp.display();
#endif
}
