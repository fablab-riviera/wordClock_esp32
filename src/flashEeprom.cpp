#include <EEPROM.h>

// define the number of bytes you want to access
#define EEPROM_SIZE 108
#define ADDRESS_BRIGHTNESSNORMAL 0  // 1 byte
#define ADDRESS_BRIGHTNESSREDUCED 1 // 1 byte
#define ADDRESS_COLOR 2             // 2 bytes
#define ADDRESS_WIFIVARSSID 4       // 52 bytes
#define ADDRESS_WIFIVARPASS 56      // 52 bytes
void eepromBegin() {
    EEPROM.begin(EEPROM_SIZE);
}
// utilitaire
String readStringEEPROM(int addr) {
    int len = EEPROM.read(addr);
    char data[len+1];
    for (int i=0; i<len; i++) data[i] = EEPROM.read(addr+1+i);
    data[len] = '\0';
    return String(data);
}
void writeStringEEPROM(int addr, String str) {
    byte len = str.length();
    EEPROM.write(addr, len);
    for (int i=0; i<len; i++) EEPROM.write(addr+1+i, str[i]);
    EEPROM.commit();
}
// brightness
uint8_t readBrightnessNormal() {
    uint8_t brightnessNormal;
    EEPROM.get(ADDRESS_BRIGHTNESSNORMAL, brightnessNormal);
    return brightnessNormal;
}
uint8_t readBrightnessReduced() {
    uint8_t brightnessReduced;
    EEPROM.get(ADDRESS_BRIGHTNESSREDUCED, brightnessReduced);
    return brightnessReduced;
}
void writeBrightnessNormal(uint8_t brightnessNormal) {
    // seulement des mises à jour pour limiter le nombre d'écritures
    if (readBrightnessNormal() != brightnessNormal) {
        EEPROM.put(ADDRESS_BRIGHTNESSNORMAL, brightnessNormal);
        EEPROM.commit();
    }
}
void writeBrightnessReduced(uint8_t brightnessReduced) {
    if (readBrightnessReduced() != brightnessReduced) {
        EEPROM.put(ADDRESS_BRIGHTNESSREDUCED, brightnessReduced);
        EEPROM.commit();
    }
}
// color
uint16_t readColor() {
    uint16_t c;
    EEPROM.get(ADDRESS_COLOR, c);
    return c;
}
void writeColor(uint16_t color) {
    if (readColor() != color) {
        EEPROM.put(ADDRESS_COLOR, color);
        EEPROM.commit();
    }
}
// wifi variable
String readWifiSsid() {
    return readStringEEPROM(ADDRESS_WIFIVARSSID);
}
void writeWifiSsid(String ssid) {
    writeStringEEPROM(ADDRESS_WIFIVARSSID, ssid);
}
String readWifiPass() {
    return readStringEEPROM(ADDRESS_WIFIVARPASS);
}
void writeWifiPass(String pass) {
    writeStringEEPROM(ADDRESS_WIFIVARPASS, pass);
}
