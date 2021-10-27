#include <Arduino.h>
#define LOW 50
#define HIGH 250
#define GROUPS 10
#define LDR_LOW 50
#define LDR_HIGH 450

class Ldr
{
public:
  Ldr(uint8_t pin, uint16_t delay);
  uint8_t getBuffered();
  uint8_t convReading(int v);

private:
  uint8_t pin;
  uint8_t goal = LOW;
  uint16_t delay;
  unsigned long last = 0;
  float correction = (LDR_HIGH - LDR_LOW) / (HIGH - LOW);
};

Ldr::Ldr(uint8_t pin, uint16_t delay)
{
  this->pin = pin;
  this->delay = delay;
};

uint8_t Ldr::getBuffered()
{
  int reading = analogRead(pin);
  uint8_t value = convReading(reading);


  return value;
}

uint8_t Ldr::convReading(int v)
{
  if (v > LDR_HIGH)
  {
    return HIGH;
  }
  else if (v < LDR_LOW)
  {
    return LOW;
  }
  v = (v - LDR_LOW) / correction + LDR_LOW;
  return v;
}