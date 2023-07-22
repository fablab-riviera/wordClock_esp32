#include <Arduino.h>
#include "RTClib.h"

#include "neoMatrix.h"

// Real Time Clock
RTC_DS1307 rtc;

// déclaration horloge internet
const char* ntpServer = "ch.pool.ntp.org";
int timeZone = 3600;

int getLastSundayOfMonth(int y, int m, int lastDayOfMonth) {
  // work only from month 3 to month 12 !!!
  int d = lastDayOfMonth;
  return lastDayOfMonth - ((d += y - 2, 23*m/9 + d + 4 + y/4- y/100 + y/400)%7);
}
// calculate Daylight Saving Time (DST)
int getDST(int year, int month, int day) {
  int dst = 0; 
  if ( month >= 3 && month <= 10 ) {
    switch (month) {
      case 3: if (getLastSundayOfMonth(year, month, 31) <= day) dst = 1; break;
      case 10: if (getLastSundayOfMonth(year, month, 31) > day) dst = 1; break;
      default: dst = 1; break;
    }
  }
  return dst;
}
int getTimeYear() {
  return rtc.now().year();
}
int getTimeMonth() {
  return rtc.now().month();
}
int getTimeDay() {
  return rtc.now().day();
}
int getTimeHour() {
  return rtc.now().hour();
}
int getTimeMinute() {
  return rtc.now().minute();
}
int getTimeSecond() {
  return rtc.now().second();
}
String getStringDate() {
  String s = "";
  s += getTimeYear();
  s += "-";
  s += getTimeMonth()<10?"0":"";
  s += getTimeMonth();
  s += "-";
  s += getTimeDay()<10?"0":"";
  s += getTimeDay();
  return s;
}
String getStringDateLocal() {
  String s = "";
  s += getTimeDay()<10?"0":"";
  s += getTimeDay();
  s += ".";
  s += getTimeMonth()<10?"0":"";
  s += getTimeMonth();
  s += ".";
  s += getTimeYear();
  return s;
}
String getStringTime() {
  String s = "";
  s += getTimeHour()<10?"0":"";
  s += getTimeHour();
  s += ":";
  s += getTimeMinute()<10?"0":"";
  s += getTimeMinute();
  return s;
}
String getStringDateTime() {
  String s = "";
  s += getStringDate();
  s += "T";
  s += getStringTime();
  return s;
}
void rtcBegin() {
  rtc.begin();
}
void setDateTimeManually(int year, int month, int day, int hour, int min) {
    // Set RTC time
    rtc.adjust(DateTime(year, month, day, hour, min, 0));
}
// Syncing time from NTP Server
void setInternetTime() {
  ledsSyncOn();
  // Set ntp time to local
  configTime(timeZone, 0, ntpServer);

  // Get local time
  struct tm timeInfo;
  if (getLocalTime(&timeInfo)) {
    // Set RTC time
    int year = timeInfo.tm_year + 1900;
    int month = timeInfo.tm_mon + 1;
    int day = timeInfo.tm_mday;
    int hour = timeInfo.tm_hour + getDST(year, month, day);
    int min = timeInfo.tm_min;
    int sec = timeInfo.tm_sec;
    rtc.adjust(DateTime(year, month, day, hour, min, sec));
  }
  ledsSyncOff();
}
