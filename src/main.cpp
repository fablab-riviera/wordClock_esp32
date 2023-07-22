#include <Arduino.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#include "wordClock.h"
#include "webServerWC.h"

// ---------------
// programme principal
// ---------------
void setup() {
  // initialisation de la wordClock
  wordClockBegin();
  // activer le serveur web
  serverWebBegin();
}
void loop() {
  serverWebHandleClient();
  wordClockDisplay();
  delay(1000);  
}