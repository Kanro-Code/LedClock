#include "RTClib.h"
#include <Wire.h>

RTC_DS3231 rtc;
// https://www.timeapi.io/

class Timer {
public:
  void init();
  DateTime &getTime();
  void refresh(bool force);
  bool dts(DateTime &time);
  void setTime(char *s);
  void getSerial();
  void print();

private:
  unsigned long lastRefresh = 0;
  DateTime now;
  const int ST_OFFSET = 3600;
  const int TZ_OFFSET_UTC = 3600;
  unsigned long prev = 0;
  unsigned long freq = 0;
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
                   "zone?timeZone=UTC");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  refresh(true);
}

DateTime &Timer::getTime() {
  refresh(false);
  return now;
}

void Timer::refresh(bool force) {
  if (force) {
    this->prev = millis() + this->freq;
    this->now = rtc.now() + TimeSpan(TZ_OFFSET_UTC);

    if (dts(this->now)) {
      this->now = this->now + TimeSpan(ST_OFFSET);
    }
  } else {
    if (millis() - this->prev >= this->freq) {
      refresh(true);
    }
  }
}

// https://www.timeapi.io/api/Time/current/zone?timeZone=UTC
void Timer::setTime(char *str) {
  DateTime time = str;
  rtc.adjust(time);
  refresh(true);
  Serial.println("Setting time:");
  print();
}

bool Timer::dts(DateTime &t) {
  if (t.month() < 3 && t.month() > 10) {
    return false;
  } else if (t.month() == 3) {
    // Lowerbound DST limit > 2021-03-28T01:00:00Z
    if (t.day() == 28 && t.hour() < 1) {
      return false;
    } else if (t.day() < 28) {
      return false;
    } else {
      return true;
    }
  } else if (t.month() == 10) {
    // Upperbound DST limit > 2021-10-31T01:00:00Z
    if (t.day() == 31 && t.hour() >= 1) {
      return false;
    } else {
      return true;
    }
  } else {
    return true;
  }
}

void Timer::getSerial() {
  String str = Serial.readStringUntil('\n');
  while (Serial.available()) {
    Serial.flush();
  }
  if (str.length() <= 29) {
    char c[29];
    str.toCharArray(c, 29);
    setTime(c);
  }
}

void Timer::print() {
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
  Serial.println();
}
