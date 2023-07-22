#ifndef WIFICONNECT_H
#define WIFICONNECT_H

void wifiOn();
void wifiOff();
void setWifi(String ssid, String pass);
bool isWifiModeAP();
bool isWifiConnected();

#endif