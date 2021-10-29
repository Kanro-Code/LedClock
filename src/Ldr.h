#include <Arduino.h>
#define OUTPUT_LOW 50
#define OUTPUT_HIGH 250
#define LDR_LOW 50
#define LDR_HIGH 450

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
};

uint8_t Ldr::getBuffered()
{
  if (millis() - prev >= delay)
  {
    prev = millis();
    int reading = analogRead(pin);
    uint8_t v = convReading(reading);
    int8_t diff = (v - current);
    if (diff > 10 && current <= HIGH) {
      current++;
    } else if (diff < -10 && current >= LOW) {
      current--;
    }
    Serial.print("v:");
    Serial.print(v);
    // current = current + ((v > current) ? 1 : -1);
    Serial.print(" C:");
    Serial.println(current);
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