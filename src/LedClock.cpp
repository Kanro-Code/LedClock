#include <Arduino.h>
#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>

byte convPos(byte pos);

#define LED_COUNT 83
#define ROWS 11
#define COLS 7

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
  delay(250);
}

byte convPos(byte pos) {
  byte t = pos + floor((pos+ROWS)/(ROWS*2))*2;
  return t;
}
