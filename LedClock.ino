#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>

#define LED_COUNT 83
#define COL_COUNT 7

#define LED_PIN 6

CRGB leds[LED_COUNT];

byte brightness = 250;
byte saturation = 255;
byte startColor = 0;



void setup() {
  Serial.begin(57600);
  Serial.println("7 Segment Clock v2 - Display Test...");
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, LED_COUNT);
}
byte k = 0;
void loop() {
  FastLED.clear();
  byte t = convPos(k);
  leds[t] = CRGB::Red;
  k++;
  if (k > 77) {
    k = 0;
  }
  FastLED.show();
  delay(100);
}

byte temp[4] = {11, 33, 55, 77};

byte convPos(byte pos) {
  byte r = pos;
  for (byte i = 0; i < 4; i++) {
    if (pos >= temp[i]) {
      r = r + 2;
    }
  }
  Serial.println("Orignal: " + pos +  " Convert: " + r);
  return r;
}
