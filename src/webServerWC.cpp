#include <WebServer.h>

#include "webServerWC.h"
#include "webPages.h"
#include "neoMatrix.h"
#include "timeSync.h"
#include "wifiConnect.h"

// définitions webserver
WebServer server(80);

void handle_OnConnect() {
  String sBrightnessNormal = String(getBrightnessNormal());
  String sBrightnessReduced = String(getBrightnessReduced());
  server.send(200, "text/html", pageSetup(getStringDate(), getStringTime(), sBrightnessNormal, sBrightnessReduced, getColorHex(), isWifiModeAP())); 
}

void handle_dateTime() {
  String t = server.arg("dateTime");
  char t2[17];
  strcpy(t2, t.c_str());
  int y = (t2[0]-'0')*1000+(t2[1]-'0')*100+(t2[2]-'0')*10+(t2[3]-'0');
  int mo = (t2[5]-'0')*10+(t2[6]-'0');
  int d = (t2[8]-'0')*10+(t2[9]-'0');
  int h = (t2[11]-'0')*10+(t2[12]-'0');
  int mi = (t2[14]-'0')*10+(t2[15]-'0');
  setDateTimeManually(y, mo, d, h, mi);
  handle_OnConnect();
}

void handle_brightnessNormal() {
  uint8_t b = server.arg("brightness").toInt();
  if (getBrightnessNormal() != b) setBrightnessNormal(b);
  handle_OnConnect();
}
void handle_brightnessReduced() {
  uint8_t b = server.arg("brightnessReduced").toInt();
  if (getBrightnessReduced() != b) setBrightnessReduced(b);
  handle_OnConnect();
}
void handle_color() {
  String c = server.arg("color");
  setColorHex(c);
  handle_OnConnect();
}
void handle_wifiAP() {
  String ssid = server.arg("wifiAPssid");
  String pass = server.arg("wifiAPpass");
  setWifi(ssid, pass);
  handle_OnConnect();
}
void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

void serverWebBegin() {
  server.on("/", handle_OnConnect);
  server.on("/dateTime", handle_dateTime);
  server.on("/brightness", handle_brightnessNormal);
  server.on("/brightnessReduced", handle_brightnessReduced);
  server.on("/color", handle_color);
  server.on("/wifiAP", handle_wifiAP);
  server.onNotFound(handle_NotFound);
  server.begin();
}

void serverWebHandleClient() {
  server.handleClient();
}