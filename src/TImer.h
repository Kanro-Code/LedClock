#include "RTClib.h"
RTC_DS3231 rtc;

class Timer
{
public:
  static void init();
  static void getTime();
  static void setTime(int t);
  static void getSerial();

private:
  int refreshed;
  int t;
};

void Timer::getTime()
{
  DateTime now = rtc.now();
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);

  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
};

void Timer::init()
{
  if (!rtc.begin())
  {
    Serial.println("Something is wrong with the RTC module.");
    Serial.flush();
    abort();
  }
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
  }
}

void Timer::setTime(int t)
{
}

void Timer::getSerial()
{
  String s = Serial.readString();
  Serial.println("Received:" + s);
}