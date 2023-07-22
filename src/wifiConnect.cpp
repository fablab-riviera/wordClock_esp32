#include <WiFi.h>

#include "neoMatrix.h"
#include "flashEeprom.h"

// sauvegarde configuration wifi
void setWifi(String ssid, String pass) {
  writeWifiSsid(ssid);
  writeWifiPass(pass);
}
// déclaration AP wifi
const char* ssidAP = "wordClock-AP";
const char* passAP = NULL;
bool isWifiModeAP() {
  return (WiFi.getMode() == WIFI_AP || WiFi.getMode() == WIFI_AP_STA);
}

bool isWifiConnected() {
  return (WiFi.status() == WL_CONNECTED);
}

// connect wifi
void wifiConnect(String ssid, String pass) {
  char* c_ssid = new char[ssid.length()+1];
  char* c_pass = new char[pass.length()+1];
  strcpy(c_ssid, ssid.c_str());
  strcpy(c_pass, pass.c_str());
  WiFi.begin(c_ssid, c_pass);
  for (int i=0; i<20; i++) {
    if (!isWifiConnected()) delay(1000);
    else break;
  }
}
// ---------------
// wifi and sync time
void wifiOn() {
  do {
    // try to connect to wifi
    if (!isWifiConnected()) {
      wifiLedsOn(0); // affiche le no du wifi 
      wifiConnect(readWifiSsid(), readWifiPass());
    }
    // contrôle si connection wifi réussie, si non, on enclenche le mode point d'accès Wifi
    if (isWifiConnected()) textDisplay(WiFi.localIP().toString()); // affichage l'adresse IP sur la wordClock
    else {
      // pas de connexion, configuration de l'AP Wifi
      // affichage l'adresse IP sur la wordClock
      WiFi.mode(WIFI_AP);
      if (WiFi.softAP(ssidAP, passAP)) {
        textDisplay(WiFi.softAPIP().toString()); // affiche l'adresse ip
      }
    }
  } while (!isWifiConnected() && !isWifiModeAP());
}
void wifiOff() {
  WiFi.disconnect();
  wifiLedsOff();
}
IPAddress getLocalIP() {
    return WiFi.localIP();
}
