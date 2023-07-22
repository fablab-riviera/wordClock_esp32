#ifndef NEOMATRIX_H
#define NEOMATRIX_H
#include <Adafruit_NeoMatrix.h>

void wifiLedsOn(int noWifi);
void wifiLedsOff();
void ledsSyncOn();
void ledsSyncOff();
void textDisplay(String text);
int matrixWidth();
void matrixBegin();
void setSegmLedsOn(int x, int y, int len);
void setSegmLedsOn(int x, int y, int len, uint16_t color);
void ledsInit();
void ledsDisplay();
uint8_t getBrightnessNormal();
uint8_t getBrightnessReduced();
void setBrightnessNormal(uint8_t brighthness);
void setBrightnessReduced(uint8_t brighthness);
void setMatrixBrightness(uint8_t brightness);
uint16_t getColor();
String getColorHex();
void setColor(uint16_t color);
void setColorHex(String sHex);
int hex2int(char* cHex, int offset);
extern const uint16_t colorBlack;
extern const uint16_t colorBlue;
extern const uint16_t colorRed;
extern const uint16_t colorGreen;
extern const uint16_t colorCyan;
extern const uint16_t colorMagenta;
extern const uint16_t colorYellow;
extern const uint16_t colorWhite;
extern const uint16_t colorWhiteMuted;

#endif
