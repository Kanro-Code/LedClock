#include "RTClib.h"
#include <Wire.h>
#include <EEPROM.h>

RTC_DS3231 rtc;

class Timer {
public:
  void init();
  DateTime &getTime();
  boolean summerTime(DateTime &time);
  void setTime(char *s);
  void getSerial();

private:
  unsigned long refresh = 0;
  DateTime now;
};

void Timer::init() {
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC.");
    Serial.flush();
    abort();
  }

  if (rtc.lostPower()) {
    Serial.print("RTC reset, set time.");
    Serial.println("https://www.timeapi.io/api/Time/current/"
                   "zone?timeZone=Europe/Amsterdam");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  now = rtc.now();
}

DateTime &Timer::getTime() {
  if (millis() > refresh) {
    refresh = millis() + 1000;
    now = rtc.now();
  }

  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.print(" Summertime:");
  Serial.print(summerTime(now));
  Serial.println();

  return now;
}

// https://www.timeapi.io/api/Time/current/zone?timeZone=Europe/Amsterdam -> dateTime
// 2021-10-07T17:18:11.1731013
void Timer::setTime(char *str) {
  DateTime time = str;
  Serial.println("Setting time:");
rtc.adjust(time);
  Serial.println(EEPROM.read(0));
  EEPROM.write(0, summerTime(time));
  Serial.println(EEPROM.read(0));

  
}

// If in summertime, clocks are advanced one hour, so that darkness falls later.
// This clock is only made for CEST, other timezones might have different triggers.
boolean Timer::summerTime(DateTime &time) {
  if ((time.month() == 3 && time.day() >= 28 && time.hour() >= 1) ||
      time.month() > 3) {
    if ((time.month() == 10 && time.day() <= 31 && time.hour() < 1) ||
        time.month() < 10) {
      return true;
    }
  }
  return false;
}

void Timer::getSerial() {
  String str = Serial.readStringUntil('\n');

  if (str.length() == 27) {
    char c[27];
    str.toCharArray(c, 27);
    setTime(c);
  }
}