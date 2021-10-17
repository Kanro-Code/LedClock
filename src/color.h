#include <Arduino.h>
#include <FastLED.h>

CHSV test = CHSV(1,1,1);
class Color {
public:
  CHSV getColor(byte r, byte g, byte b);
}

CHSV Color::getColor(byte r, byte g, byte b) {
  return CHSV(r,g,b);
}