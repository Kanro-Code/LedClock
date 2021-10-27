#include <Arduino.h>
#define OUTPUT_LOW 50
#define OUTPUT_HIGH 250
#define LDR_LOW 50
#define LDR_HIGH 450
#define GROUPS 10

class Ldr
{
public:
  Ldr(uint8_t pin, uint16_t delay);
  uint8_t getBuffered();
  uint8_t convReading(int v);

private:
  uint8_t pin, current;
  uint16_t delay;
  unsigned long prev = 0;
  float convFactor = (LDR_HIGH - LDR_LOW) / (OUTPUT_HIGH - OUTPUT_LOW);

};

Ldr::Ldr(uint8_t pin, uint16_t delay)
{
  this->pin = pin;
  this->delay = delay;
  average.clear();
};

uint8_t Ldr::getBuffered()
{
  if (millis() - prev >= delay)
  {
    prev = millis();
    int reading = analogRead(pin);
    uint8_t v = convReading(reading);
    Serial.print("Value:");
    Serial.print(v);
  }

  return current;
}

uint8_t Ldr::convReading(int v)
{
  if (v > LDR_HIGH)
  {
    return OUTPUT_HIGH;
  }
  else if (v < LDR_LOW)
  {
    return OUTPUT_LOW;
  }
  return (v - LDR_LOW) / convFactor + LDR_LOW;
}