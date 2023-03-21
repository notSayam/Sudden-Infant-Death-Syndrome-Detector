/*

PROJECT CREATED FOR THE PURPOSE OF COMPLETING COURSE ECE-GY 6483 F22 REAL TIME EMBEDDED SYSTEMS

CREATORS: 

Sayam Dhingra     Abhav Vohra     Yami Naik      Ankita Gupta
N16451815         N17434068       N10518065       N15402218
sd5292            av3290          yn2224          ag9135

SENSOR: 

Adafruit Sensirion SHT40 Temperature & Humidity Sensor

DATA: 

// Websites: 
https://sensirion.com/products/catalog/SHT40/
https://www.adafruit.com/product/4885

// DataSheet: 
https://cdn-learn.adafruit.com/assets/assets/000/099/223/original/Sensirion_Humidity_Sensors_SHT4x_Datasheet.pdf?1612388531

DETAILS: 
This file contains screen setups used in the alarm systems.

*/

#ifndef __LCDSTUFF
#define __LCDSTUFF

#include "drivers/LCD_DISCO_F429ZI.h"

#define BACKGROUND 1
#define FOREGROUND 0
#define GRAPH_PADDING 5

char display_buf[3][40];
LCD_DISCO_F429ZI lcd;

/*
Basic screen setup for the first 10 seconds of initialization phase
*/

void setup_background_layer()
{
  lcd.SelectLayer(BACKGROUND);
  lcd.Clear(LCD_COLOR_BLACK);
  lcd.SetBackColor(LCD_COLOR_BLACK);
  lcd.SetTextColor(LCD_COLOR_GREEN);
  lcd.SetLayerVisible(BACKGROUND,ENABLE);
  lcd.SetTransparency(BACKGROUND,0x7Fu);
}

void setup_foreground_layer()
{
    lcd.SelectLayer(FOREGROUND);
    lcd.Clear(LCD_COLOR_BLACK);
    lcd.SetBackColor(LCD_COLOR_BLACK);
    lcd.SetTextColor(LCD_COLOR_LIGHTGREEN);
}

/*
NO BREATH DETECTED SCREEN SETTINGS
*/
void ALARM_SYSTEM_RED()
{
    lcd.SelectLayer(FOREGROUND);
    lcd.Clear(LCD_COLOR_RED);
    lcd.SetBackColor(LCD_COLOR_RED);
    lcd.SetTextColor(LCD_COLOR_BLACK);
}

/*
LOW BREATH WARNING SCREEN SETTINGS
*/
void ALARM_SYSTEM_YELLOW()
{
    lcd.SelectLayer(FOREGROUND);
    lcd.Clear(LCD_COLOR_YELLOW);
    lcd.SetBackColor(LCD_COLOR_YELLOW);
    lcd.SetTextColor(LCD_COLOR_BLACK);
}

/*
Breathing being detected properly screen settings
*/
void KALM()
{
    snprintf(display_buf[0],40,"Everything's good... (-.-)zzZZ");
    snprintf(display_buf[1],40,"(-.-)zzZZ");
    snprintf(display_buf[2],40,"(-.-)zzZZ");

    lcd.DisplayStringAt(0, LINE(0), (uint8_t *)display_buf[1], LEFT_MODE);
    lcd.SelectLayer(FOREGROUND);
    lcd.Clear(LCD_COLOR_LIGHTGREEN);
    lcd.SetBackColor(LCD_COLOR_LIGHTGREEN);
    lcd.SetTextColor(LCD_COLOR_BLACK);
}

#endif