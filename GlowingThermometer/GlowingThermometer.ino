#include <Wire.h>
#include <SeeedOLED.h>
#include "Barometer.h"
#include <Seeed_ws2812.h>

#define DEBUGING while(true) {Serial.println("DEBUG...");}  
#define printf(str1, value, str2) Serial.print(str1), \
                                  Serial.print(value),\
                                  Serial.println(str2)
#define LEDN 5
#define SIG_PIN 12
#define high_temp_alarm  32
#define low_temp_alarm  25

uint8_t color[LEDN];
uint8_t t[5] = {28, 29, 30, 31, 32};//you can setup 5 temperature steps to track color
//Barometer

float temperature;
float pressure;
float altitude;
float atm;

unsigned char hightemp_alarm[] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xE0, 0xF8, 0xFC, 0xFE, 0x7E,
0xFE, 0xFC, 0xF0, 0xE0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xF0, 0xFC, 0xFE, 0xFF, 0x3F, 0x0F, 0x03, 0x01, 0x00,
0x01, 0x07, 0x1F, 0x3F, 0xFF, 0xFE, 0xF8, 0xE0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x80, 0xE0, 0xF8, 0xFE, 0xFF, 0x7F, 0x1F, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0F, 0x1F, 0x7F, 0xFF, 0xFC, 0xF0, 0xE0, 0x80, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xE0, 0xF0, 0xFC,
0xFF, 0x7F, 0x3F, 0x0F, 0xC3, 0xC0, 0x00, 0x80, 0xC0, 0x40, 0x00, 0xC0, 0xC0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0xFE, 0x01, 0x03, 0xFC, 0x00, 0x00, 0x01, 0x07, 0x0F, 0x3F, 0xFF, 0xFE, 0xFC,
0xF0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xF0, 0xF8, 0xFE, 0xFF, 0x3F, 0x1F, 0x07,
0x01, 0x00, 0x00, 0xFC, 0xFF, 0x01, 0x00, 0xFF, 0x87, 0x00, 0xFE, 0xFF, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x07,
0x1F, 0x7F, 0xFF, 0xFE, 0xF8, 0xE0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x80, 0xE0, 0xF8, 0xFC, 0xFF, 0x7F, 0x1F, 0x0F, 0x03, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x07, 0xFF, 0xF8, 0x03, 0x0F, 0xFC, 0xE1, 0x07, 0xFE, 0xF0, 0x00, 0x00,
0x80, 0xC0, 0xC0, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x01, 0x03, 0x0F, 0x3F, 0xFF, 0xFF, 0xFC, 0xF0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0,
0xF0, 0xFC, 0xFE, 0xFF, 0x3F, 0x0F, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x40, 0xE0, 0x38, 0x1F, 0xE3, 0x70, 0x1E, 0x47, 0x70, 0x3C, 0x0F, 0x03, 0x00, 0x04,
0x3F, 0x7F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x7F, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x07, 0x1F, 0x7F, 0xFF, 0xFE, 0xF8, 0xE0, 0xC0,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x7E, 0x7F, 0x7F,
0x7F, 0x7F, 0x7B, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78,
0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78,
0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78,
0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x7B, 0x7F, 0x7F, 0x7F,
0x7F, 0x7C, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
unsigned char lowtemp_alarm [] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xE0, 0xF8, 0xF8, 0xFC, 0xFC,
0xF8, 0xF0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xF0, 0xF8, 0xFE, 0xFF, 0x3F, 0x0F, 0x07, 0x01, 0x01,
0x07, 0x1F, 0x7F, 0xFF, 0xFC, 0xF8, 0xE0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x80, 0xE0, 0xF8, 0xFC, 0xFF, 0x7F, 0x1F, 0x07, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x01, 0x03, 0x0F, 0x3F, 0xFF, 0xFE, 0xFC, 0xF0, 0xC0, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xF0, 0xFC,
0xFE, 0xFF, 0x3F, 0x0F, 0x03, 0x01, 0x80, 0x80, 0x80, 0x40, 0x60, 0xE0, 0xC0, 0xC0, 0xFC, 0xFC,
0xC0, 0xE0, 0xE0, 0x60, 0x00, 0x80, 0x80, 0x80, 0x01, 0x07, 0x1F, 0x7F, 0xFF, 0xFC, 0xF8, 0xE0,
0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xE0, 0xF8, 0xFE, 0xFF, 0x7F, 0x1F, 0x07,
0x01, 0x00, 0xC6, 0xEE, 0xFE, 0xFC, 0x7F, 0x3F, 0x7F, 0xF0, 0xE0, 0xE0, 0xC1, 0xC1, 0xFF, 0xFF,
0xC1, 0xC1, 0xE0, 0xE0, 0x70, 0x7F, 0x7F, 0x7F, 0xFC, 0xEE, 0xCE, 0x44, 0x01, 0x03, 0x0F, 0x3F,
0xFF, 0xFE, 0xFC, 0xF0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xC0, 0xF0, 0xFC, 0xFE, 0x7F, 0x3F, 0x0F, 0x03, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x8C, 0xDF, 0xDC, 0xF8, 0xF8, 0xF0, 0xF8, 0x38, 0x1C, 0x1F, 0x0F, 0x0F, 0xFF, 0xFF,
0x0F, 0x0F, 0x1C, 0x1C, 0x38, 0xF8, 0xF0, 0xF8, 0xFC, 0xDC, 0xCF, 0x88, 0x00, 0x00, 0x00, 0x00,
0x00, 0x01, 0x07, 0x1F, 0x7F, 0xFF, 0xFE, 0xF8, 0xE0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
0xE0, 0xF8, 0xFE, 0xFF, 0x3F, 0x1F, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x07, 0x07, 0x07, 0x08, 0x18, 0x1C, 0x1C, 0x0E, 0xFF, 0xFF,
0x0E, 0x1C, 0x1C, 0x18, 0x00, 0x07, 0x07, 0x07, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x0F, 0x3F, 0xFF, 0xFF, 0xFC, 0xF0, 0xC0, 0x80, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x1F, 0x3F,
0x3F, 0x3F, 0x3F, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E,
0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E,
0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E,
0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3F, 0x3F, 0x3F, 0x3F, 0x1F, 0x1E,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


Barometer myBarometer;
WS2812 strip = WS2812(LEDN, SIG_PIN);  //WS2812(led_num, sig_pin)

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  SeeedOled.init();   //initialze SEEED OLED display
  initOledTitle();    //init OLED Title
  myBarometer.init(); //Barometer init      
  vibratorInit();     //init vibrator pins
  strip.begin();      // Init and begin strip drivers
  color_domain_init(low_temp_alarm, high_temp_alarm, LEDN);
}

void messagetoSerial()
{
  printf("Temperature is:", temperature, " C");
  printf("Pressure is:", pressure, " Pa");
  printf("atm is: ", altitude, " m");
  Serial.println("");
}

void messagetoOled()
{
  SeeedOled.setTextXY(2,6);
  SeeedOled.putNumber(temperature); //Print the String
  SeeedOled.putString(" deg C");
  SeeedOled.setTextXY(3,6);
  SeeedOled.putNumber(pressure);
  SeeedOled.putString(" Pa");
  SeeedOled.setTextXY(4,6);
  SeeedOled.putNumber(altitude); //Print the String
  SeeedOled.putString(" m");
}
void loop()
{    
  temperature = myBarometer.bmp085GetTemperature(myBarometer.bmp085ReadUT()); //Get the temperature, bmp085ReadUT MUST be called first  
  pressure = myBarometer.bmp085GetPressure(myBarometer.bmp085ReadUP());// Get the pressure
  altitude = myBarometer.calcAltitude(pressure);
  atm = pressure / 101325;
  
  colorFollow();  //led strip changing with temperature
  
  TempAlarm();    //monitor temperature and warning
  
  messagetoSerial();
  
  messagetoOled();
  
  delay(500);
}

void initOledTitle(void){
  SeeedOled.clearDisplay();          //clear the screen and set start position to top left corner
  SeeedOled.setNormalDisplay();      //Set display to normal mode (i.e non-inverse mode)
  SeeedOled.setPageMode();           //Set addressing mode to Page Mode
  SeeedOled.setTextXY(0,2);          //Set the cursor to Xth Page, Yth Column  
  SeeedOled.putString("Thermometer"); //Print the String
  SeeedOled.setTextXY(2,0);          //Set the cursor to Xth Page, Yth Column  
  SeeedOled.putString("Temp:");       //Print the String
  SeeedOled.setTextXY(3,0);
  SeeedOled.putString("Pres:"); 
  SeeedOled.setTextXY(4,0);          //Set the cursor to Xth Page, Yth Column  
  SeeedOled.putString("Alti:");  //Print the String
}

// blow 3 functions is about vibrator
void vibratorInit(void) {
  pinMode(10, OUTPUT);      //10, 11 are pins os vibrator 
  pinMode(11, OUTPUT);
}
void vibratorOn(void) {
  digitalWrite(10, HIGH);  //turn on vibrator
  digitalWrite(11, HIGH);
}
void vibratorOff(void) {
  digitalWrite(10, LOW);  //turn on vibrator
  digitalWrite(11, LOW);
}

void colorFollow(){
  int i, j;
  for(i=0;i<LEDN;i++){ 
    if(temperature < t[i]) {
      strip.WS2812Clear();     
      strip.setLedNum(i+1);
      for(j=0; j< strip.getLedNum(); j++) {
        strip.colorWheel(color[i], j);
      }
      strip.WS2812Send();
      strip.WS2812Send();
      strip.WS2812Send();
      strip.WS2812Send();
      i=LEDN;      
    }   
  } 
}

void TempAlarm(){
  int k=3;
  if ( temperature >= high_temp_alarm ) {
    while(k--){
      SeeedOled.setNormalDisplay();
      SeeedOled.setTextXY(0,0); 
      SeeedOled.drawBitmap(hightemp_alarm,1024);
      vibratorOn();
      delay(1000);
      SeeedOled.setInverseDisplay();    
      vibratorOff();
      delay(1000);
    }
    SeeedOled.clearDisplay();
    SeeedOled.setInverseDisplay();
    initOledTitle();    //init OLED Title
    vibratorOff();
  }
  else if(temperature < low_temp_alarm){
    while(k--){
      SeeedOled.setNormalDisplay();
      SeeedOled.setTextXY(0,0); 
      SeeedOled.drawBitmap(lowtemp_alarm,1024);
      vibratorOn();
      delay(1000);
      SeeedOled.setInverseDisplay();    
      vibratorOff();
      delay(1000);
    }
    SeeedOled.clearDisplay();
    SeeedOled.setInverseDisplay();
    initOledTitle();    //init OLED Title
    vibratorOff();
  }
}
uint16_t color_domain_init(uint8_t low, uint8_t high, uint8_t n) {
  uint8_t up_step = (high-low)/n;   //20 C ~ 30 C: (30-20)/5=2, every 2 C up, one led  light up  
  if(up_step == 0 ){
    up_step = 1; 
  }
  int color_step, t_step;    
  for( color_step=0; color_step<n; color_step++){
    color[color_step] = color_step*(170/(n-1));    
    printf("color:", color[color_step],"");    
  }  
  printf("up_step:", up_step,"");      
  return up_step;
}

void colorChangeWithTemperature(uint32_t temperature, uint8_t low, uint8_t high, uint8_t n){
  uint8_t up_step = (high-low)/n;
  int color_step, t_step;
  for(t_step=1, color_step = 0; t_step <= n; t_step++, color_step++){    
    if( temperature < low + (t_step * up_step) ){      
      printf("t_step:", t_step,"");
      strip.WS2812Clear();
      strip.setLedNum(t_step); 
      for(int i=0; i< t_step; i++) {
        strip.colorWheel(color[color_step], i);
      }      
      strip.WS2812Send();
      strip.WS2812Send();
      strip.WS2812Send();
      strip.WS2812Send();
      t_step = n+1;    //finish checking temperature high limit      
    }
  }
}


