#include <Arduino.h>
#include <FastLED.h>

// CHSV test = CHSV(1,1,1);
class Animation {
public:
  Animation(unsigned int freq, unsigned int offset);
  static CRGB singleRGB(byte r, byte g, byte b);
  static CHSV singleHSV(byte h, byte s, byte v);
  CHSV cycleHSV();
  bool ready();
  unsigned long getSpeed();
  unsigned long getNext();
  CHSV getCurrent();

private:
  unsigned long prev = 0;
  unsigned int offset = 0;
  unsigned long freq = 0;
  byte brightness = 255;
  CHSV current = CHSV(0,0,0);
};

Animation::Animation(unsigned int _freq, unsigned int _offset) {
  this->freq = _freq;
  this->offset = _offset * 2.55;
};

CRGB Animation::singleRGB(byte r, byte g, byte b) { return CRGB(r, g, b); };
CHSV Animation::singleHSV(byte h, byte s, byte v) { return CHSV(h, s, v); };

CHSV Animation::cycleHSV() {
  if (ready()) {
    this->prev = millis();
    byte v = (millis() / freq + offset) % 255;
    current = CHSV(v, 255, 128);
  }

  return current;

}

bool Animation::ready() {
  return (millis() - this->prev) >= this->freq;
};

unsigned long Animation::getSpeed() { return this->freq; };
unsigned long Animation::getNext() { return this->prev; };
CHSV Animation::getCurrent() { return this->current; };

