#ifndef FLASHEEPROM_H
#define FLASHEEPROM_H

void eepromBegin();
uint8_t readBrightnessNormal();
uint8_t readBrightnessReduced();
void writeBrightnessNormal(uint8_t brightness);
void writeBrightnessReduced(uint8_t brightness);
uint16_t readColor();
void writeColor(uint16_t color);
String readWifiSsid();
void writeWifiSsid(String ssid);
String readWifiPass();
void writeWifiPass(String pass);

#endif
