#ifndef TIMESYNC_H
#define TIMESYNC_H
#include <Arduino.h>
void rtcBegin();
int getTimeYear();
int getTimeMonth();
int getTimeDay();
int getTimeHour();
int getTimeMinute();
int getTimeSecond();
String getStringDate();
String getStringDateLocal();
String getStringTime();
void setInternetTime();
void setDateTimeManually(int y, int mo, int d, int h, int mi);

#endif
