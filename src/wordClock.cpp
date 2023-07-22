#include <Adafruit_GFX.h>

#include "timeSync.h"
#include "neoMatrix.h"
#include "wifiConnect.h"

// variable de gestion de l'horloge
int day_last = -1;
int min_last = -1;
int brightness_last = -1;
uint16_t color_last = 0;

// wordClock, fonctions utiliraires
// --------------------------------
int getMinutesOf5() {
  return getTimeMinute() % 5;
}  
int getHeureDansMoinsDe5Minutes() {
  return getTimeHour() + ((((getTimeMinute()-1)/5)+1)*5)/60;
}
int getMinutes(){
  int minutes = getTimeMinute();
  if (minutes > 0) {
    return ((((minutes-1)/5)+1)*5)%60;
  } else return 0;
}
int getDansMinutes(){
    return 5-getMinutesOf5();
}
bool isText_sera() {
  return getMinutesOf5() != 0;
}
uint8_t getBrightness() {
  // baisser la luminosité entre 22h et 07h
  if (getTimeHour() >= 22 || getTimeHour() < 7) return getBrightnessReduced();
  else return getBrightnessNormal();
}

// wordClock, allume les différentes zones de leds
// -----------------------------------------------
void setText_il() {
  setSegmLedsOn(1, 2, 2); // il
  if (isText_sera()) setSegmLedsOn(4, 2, 4); // sera
  else setSegmLedsOn(9, 2, 3); // est
}
void setText_heure() {
  uint16_t color = getColor();
  int heure = getHeureDansMoinsDe5Minutes();
  if (heure % 12 != 0) { // pas d'indication heure si midi ou minuit
    if (heure == 1 || heure == 13) setSegmLedsOn(9, 7, 5, color); // heure
    else setSegmLedsOn(9, 7, 6, color); // heures
  }
  switch (heure%12) {
    case 1: setSegmLedsOn(10, 6, 3, color); break; // une
    case 2: setSegmLedsOn(1, 5, 4, color); break; // deux
    case 3: setSegmLedsOn(4, 4, 5, color); break; // trois
    case 4: setSegmLedsOn(9, 5, 6, color); break; // quatre
    case 5: setSegmLedsOn(5, 5, 4, color); break; // cinq
    case 6: setSegmLedsOn(12, 4, 3, color); break; // six
    case 7: setSegmLedsOn(8, 4, 4, color); break; // sept
    case 8: setSegmLedsOn(1, 4, 4, color); break; // huit
    case 9: setSegmLedsOn(11, 6, 4, color); break; // neuf
    case 10: setSegmLedsOn(1, 6, 3, color); break; // dix
    case 11: setSegmLedsOn(1, 7, 4, color); break; // onze
  }
}
void setText_midi() {
  const uint16_t color = getColor();
  switch (getHeureDansMoinsDe5Minutes()) {
    case 0: case 24: setSegmLedsOn(4, 6, 6, color); break; // minuit
    case 12: setSegmLedsOn(5, 7, 4, color); break; // midi
  }
}
void setText_minute() {
  const uint16_t color = getColor();
  switch (getMinutes()) {
    case 5: setSegmLedsOn(1, 11, 4, color); break; // cinq
    case 10: setSegmLedsOn(6, 10, 3, color); break; // dix
    case 15: setSegmLedsOn(9, 10, 6, color); break; // quinze
    case 20: setSegmLedsOn(1, 10, 5, color); break; // vingt
    case 25: setSegmLedsOn(1, 10, 5, color); setSegmLedsOn(10, 11, 5, color); break; // vingt-cinq
    case 30: setSegmLedsOn(1, 9, 6, color); break; // trente
    case 35: setSegmLedsOn(1, 9, 6, color); setSegmLedsOn(10, 11, 5, color); break; // trente-cinq
    case 40: setSegmLedsOn(7, 9, 8, color); break; // quarante
    case 45: setSegmLedsOn(7, 9, 8, color); setSegmLedsOn(10, 11, 5, color); break; // quarante-cinq
    case 50: setSegmLedsOn(1, 11, 9, color); break; // cinquante
    case 55: setSegmLedsOn(1, 11, 14, color); break; // cinquante-cinq
  }
}
void setText_dans() {
  const uint16_t color = getColor();
  int min = getDansMinutes();
  if (min%5 != 0) setSegmLedsOn(1, 13, 4); // dans
  switch (min) {
    case 1: setSegmLedsOn(6, 13, 1, color); setSegmLedsOn(8, 14, 6); break; // 1 minute
    case 2: setSegmLedsOn(7, 13, 1, color); setSegmLedsOn(8, 14, 7); break; // 2 minutes
    case 3: setSegmLedsOn(5, 14, 1, color); setSegmLedsOn(8, 14, 7); break; // 3 minutes
    case 4: setSegmLedsOn(6, 14, 1, color); setSegmLedsOn(8, 14, 7); break; // 4 minutes
  }
}
// wordClock, assemble la matrix de leds
// -------------------------------------
void clockSet() {
  ledsInit();
  setText_il();
  setText_heure();
  setText_midi();
  setText_minute();
  setText_dans();
}
// wordClock, fonctions d'affichage
// --------------------------------
void wordClockDisplay() {
  // si le mode wifi n'est pas en mode AP (accès-point), l'horloge est synchonisée avec l'heure internet une fois par jour, à 3 heures le matin
  if (!isWifiModeAP()) {
    if (getTimeDay() != day_last && getTimeHour() > 3) {
      if (!isWifiConnected()) wifiOn(); // si la connexion wifi  a été perdue
      if (isWifiConnected()) { // si la connexion wifi est présente
        setInternetTime();
        day_last = getTimeDay();
      }
    }
  }
  // si la minute, la couleur ou la luminosité ont changé: afficher l'heure
  if (getTimeMinute() != min_last || getColor() != color_last || getBrightness() != brightness_last) {
    min_last = getTimeMinute();
    color_last = getColor();
    brightness_last = getBrightness();
    setMatrixBrightness(getBrightness());
    clockSet();  
    ledsDisplay();
  }
}
// wordClock, initialisation
// -------------------------
void wordClockBegin() {
  matrixBegin();
  rtcBegin();
  // activer le wifi
  wifiOn();
  // synchronise l'heure internet si le wifi n'est pas en mode AP
  if (!isWifiModeAP()) setInternetTime();
}
