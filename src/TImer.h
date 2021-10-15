#include "RTClib.h"
#include <Wire.h>
#include <EEPROM.h>

RTC_DS3231 rtc;

class Timer
{
public:
  void init();
  DateTime &getTime();
  void refresh(bool force);
  bool isSummerTime();
  bool isSummerTime(DateTime time);
  void setTime(char *s);
  void setTime(DateTime time);
  void getSerial();
  void print();

private:
  unsigned long lastRefresh = 0;
  byte summerTimeHour;
  const byte SUMMERTIME_LOC = 6;
  void checkSummerTime();
  DateTime now;
};

void Timer::init()
{
  if (!rtc.begin())
  {
    Serial.println("Couldn't find RTC.");
    Serial.flush();
    abort();
  }

  if (rtc.lostPower())
  {
    Serial.print("RTC reset, set time.");
    Serial.println("https://www.timeapi.io/api/Time/current/"
                   "zone?timeZone=Europe/Amsterdam");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  now = rtc.now();
}

DateTime &Timer::getTime()
{
  refresh(false);
  return now;
}

void Timer::refresh(bool force)
{
  if (force)
  {
    now = rtc.now();
    lastRefresh = millis() + 1000;
    checkSummerTime();
  }
  else
  {
    if (millis() > lastRefresh)
    {
      refresh(true);
    }
  }
}

// https://www.timeapi.io/api/Time/current/zone?timeZone=Europe/Amsterdam -> dateTime
// 2021-10-07T17:18:11.1731013
void Timer::setTime(char *str)
{
  DateTime time = str;
  setTime(time);
}

void Timer::setTime(DateTime time) {
  Serial.println("Setting time:");
  rtc.adjust(time);
  refresh(true);
  EEPROM.write(SUMMERTIME_LOC, isSummerTime());
}

// If in summertime, clocks are advanced one hour, so that darkness falls later.
// This clock is only made for CEST, other timezones might have different triggers.
bool Timer::isSummerTime()
{
  getTime();
  return isSummerTime(now);

}

bool Timer::isSummerTime(DateTime time) {
  if ((time.month() == 3 && time.day() >= 28 && time.hour() >= 1) ||
      time.month() > 3)
  {
    if ((time.month() == 10 && time.day() <= 31 && time.hour() < 1) ||
        time.month() < 10)
    {
      return true;
    }
  }
  return false;
}

void Timer::checkSummerTime()
{
  if (summerTimeHour == now.hour())
    return;

  summerTimeHour = now.hour();
  bool c = EEPROM.read(SUMMERTIME_LOC);
  if (c != isSummerTime())
  {
    TimeSpan change = TimeSpan(0, 1, 0, 0);
    DateTime newNow = (c == true) ? now - change : now + change;
    EEPROM.write(SUMMERTIME_LOC, isSummerTime(newNow));
    Serial.println(EEPROM.read(SUMMERTIME_LOC));
    setTime(newNow);
    Serial.println(EEPROM.read(SUMMERTIME_LOC));
  }
}

void Timer::getSerial()
{
  String str = Serial.readStringUntil('\n');

  if (str.length() == 27)
  {
    char c[27];
    str.toCharArray(c, 27);
    setTime(c);
  }
}

void Timer::print()
{
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
  Serial.print(isSummerTime());
  Serial.println();
}