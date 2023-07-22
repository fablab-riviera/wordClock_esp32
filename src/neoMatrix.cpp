#include "neoMatrix.h"
#include "flashEeprom.h"

// déclarations neoMatrix
#define DIM_MATRICE 14 // dimension matrice leds
#define PIN 32 // leds NeoPixels
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(DIM_MATRICE, DIM_MATRICE, PIN,
  NEO_MATRIX_TOP + NEO_MATRIX_RIGHT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB + NEO_KHZ800);

// Définitions des couleurs pour les leds
const uint16_t colorRed = matrix.Color(255,0,0);
const uint16_t colorGreen = matrix.Color(0,255,0);
const uint16_t colorBlue = matrix.Color(0,0,255);
const uint16_t colorMagenta = matrix.Color(255, 0, 255);
const uint16_t colorYellow = matrix.Color(255, 255, 0);
const uint16_t colorCyan = matrix.Color(0, 255, 255);
const uint16_t colorWhite = matrix.Color(255,255,255);
const uint16_t colorWhiteMuted = matrix.Color(191,191,191);
const uint16_t off = matrix.Color(0,0,0);
uint16_t color = colorWhite;

// manage color
// ------------
String int2hex(uint16_t n) {
  int color32[3];
  int color255[3];
  color32[0] = (n & 0b1111100000000000) >> 11; // red
  color32[1] = (n & 0b0000011111100000) >> 5; // green
  color32[2] = n & 0b0000000000011111; // blue

  for (int i=0; i<3; i++) color255[i] = color32[i]*8;

  String sHex = "#";
  for (int i=0; i<3; i++) {
    int x1 = (color255[i] & 0b11110000) >> 4;
    int x0 = (color255[i] & 0b00001111);
    int j = i*2;
    sHex += char(x1<10? 0x30+(x1) : 0x57+x1);
    sHex += char(x0<10? 0x30+(x0) : 0x57+x0);
  }
  return sHex;
}
int hex2int(char *cHex, int offset) {
  // convertit un nombre hex sur 2 positions en valeur décimale
  // le nombre hex sur 2 positions est pris dans une chaîne de caractères donnée en paramètre
  // par exemple si cHex = "#00ff00" et que la valeur d'offset = 2, c'est le nombre hex ff qui sera convertit
  // 
  char hex[3];
  hex[0] = cHex[1+offset];
  hex[1] = cHex[2+offset];

  int digit, cnt=0;
  int dec = 0;
  for (int i = 1; i >= 0; i--) {
    switch (hex[i]) {
      case 'A': case 'a': digit = 10; break;
      case 'B': case 'b': digit = 11; break;
      case 'C': case 'c': digit = 12; break;
      case 'D': case 'd': digit = 13; break;
      case 'E': case 'e': digit = 14; break;
      case 'F': case 'f': digit = 15; break;
      default: digit = hex[i] - 0x30;
    }
    dec = dec + (digit)*pow((double)16, (double)cnt);
    cnt++;
  }
  return dec;
}
void setColor(uint16_t c) {
  writeColor(c);
  color = c;
}
void setColorHex(String sHex) {
  char cHex[8];
  strcpy(cHex, sHex.c_str());
  int r = hex2int(cHex, 0);
  int g = hex2int(cHex, 2);
  int b = hex2int(cHex, 4);
  setColor(matrix.Color(r, g, b));
}
uint16_t getColor() {
  uint16_t color = readColor();
  if (color == 0) {// set valeur par défaut
    color = colorWhite; 
    setColor(color);
  }
  return color;
}
String getColorHex() {
  return int2hex(getColor());
}
// manage brightness
// -----------------
void setBrightnessNormal(uint8_t brightness) {
  writeBrightnessNormal(brightness);
}
void setBrightnessReduced(uint8_t brightness) {
  writeBrightnessReduced(brightness);
}
void setMatrixBrightness(uint8_t brightness) {
  matrix.setBrightness(brightness);
}
uint8_t getBrightnessNormal() {
  uint8_t brightness = readBrightnessNormal();
  if (brightness == 0) setBrightnessNormal(50); // set valeur par défaut
  return brightness;
}
uint8_t getBrightnessReduced() {
  uint8_t brightness = readBrightnessReduced();
  if (brightness == 0) setBrightnessReduced(10); // set valeur par défaut
  return brightness;
}
// manage matrix leds
// ------------------
uint16_t myRemapFn(uint16_t x, uint16_t y) {
  /*  les lignes de la matrice de leds sont reliées droite-droite puis gauche-gauche et ainsi de suite.
      C'est à dire que toute les lignes paires sont inversées. La déclaration de la fonction myRemapFn, 
      avec le code ci-dessous fait cette inversion automatiquement. 
  */
  if ((y+1)%2 == 0) return y*DIM_MATRICE+(DIM_MATRICE-x)-1;
  else return y*DIM_MATRICE+x;
}

void setSegmLeds(int x, int y, int len, uint16_t color) {
  for (int x1=x-1; x1<x+len-1; x1++) matrix.drawPixel(x1, y-1, color);
}
void setSegmLedsOn(int x, int y, int len) {
  setSegmLeds(x, y, len, getColor()); // default
}
void setSegmLedsOn(int x, int y, int len, uint16_t color) {
  setSegmLeds(x, y, len, color);
}
void setSegmLedsOff(int x, int y, int len) {
  setSegmLeds(x, y, len, off);
}
void ledsInit() {
  matrix.clear();
}
void ledsDisplay() {
  matrix.show();
}

// ---------------
// affichage leds on wifi connect and sync
void wifiLedsOn(int noWifi) {
  ledsInit();
  setSegmLedsOn(1, 12, 4, colorGreen);	// wifi
  if (noWifi > 0) { // no wifi =0 -> wifi variable, >0 wifi fixe
    int x = 6, y=13; // x,y wifi 1
    switch (noWifi) {
      case 2: x=7; break;
      case 3: x=5; y=14; break;
      case 4: x=6; y=14; break;  
    }
    setSegmLedsOn(x, y, 1, colorGreen); // 1, 2, 3 ou 4
  }
  ledsDisplay();
}
void wifiLedsOff() {
  ledsInit();
  ledsDisplay();
}
void ledsSyncOn() {
  ledsInit();
  setSegmLedsOn(5, 12, 4, colorGreen); //sync
  ledsDisplay();
}
void ledsSyncOff() {
  ledsInit();
  ledsDisplay();
}

void matrixBegin() {
  eepromBegin();
  matrix.begin();
  matrix.setRemapFunction(myRemapFn);  
  matrix.setTextWrap(false);
  setMatrixBrightness(getBrightnessNormal());
}
int matrixWidth() {
  return matrix.width();
}
void textDisplay(String text) {
  int xL = text.length() * -6; // 6 est la largeur d'un caractère
  for (int x=matrix.width(); x > xL; x--) {
    matrix.setTextColor(colorGreen);
    matrix.fillScreen(0);
    matrix.setCursor(x, 3);
    matrix.print(text);
    matrix.show();
    delay(100);
  }
}
