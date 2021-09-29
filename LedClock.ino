#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>

#define LED_COUNT 99
#define LED_PIN 6

CRGB leds[LED_COUNT];

byte brightness = 250;
byte saturation = 255;
byte startColor = 0;

byte segGroups[14][3] = {
  // right (seen from front) digit
  {   6,   7,   8 },    // top, a
  {   9,  10,  11 },    // top right, b
  {  13,  14,  15 },    // bottom right, c
  {  16,  17,  18 },    // bottom, d
  {  19,  20,  21 },    // bottom left, e
  {   3,   4,   5 },    // top left, f
  {   0,   1,   2 },    // center, g
  // left (seen from front) digit
  {  38,  39,  40 },    // top, a
  {  41,  42,  43 },    // top right, b
  {  25,  26,  27 },    // bottom right, c
  {  28,  29,  30 },    // bottom, d
  {  31,  32,  33 },    // bottom left, e
  {  35,  36,  37 },    // top left, f
  {  44,  45,  46 }    // center, g};
};

byte digits[10][7] = {
  { 1, 1, 1, 1, 1, 1, 0 },  // 0
  { 0, 1, 1, 0, 0, 0, 0 },  // 1
  { 1, 1, 0, 1, 1, 0, 1 },  // 2
  { 1, 1, 1, 1, 0, 0, 1 },  // 3
  { 0, 1, 1, 0, 0, 1, 1 },  // 4
  { 1, 0, 1, 1, 0, 1, 1 },  // 5
  { 1, 0, 1, 1, 1, 1, 1 },  // 6
  { 1, 1, 1, 0, 0, 0, 0 },  // 7
  { 1, 1, 1, 1, 1, 1, 1 },  // 8
  { 1, 1, 1, 1, 0, 1, 1 },  // 9
};

void setup() {
  Serial.begin(57600);
  Serial.println("7 Segment Clock v2 - Display Test...");  
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, LED_COUNT);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
  FastLED.setDither(0);
  FastLED.setBrightness(brightness);
  FastLED.clear();
  FastLED.show();
}

void loop() {
  FastLED.clear();
  FastLED.setBrightness(brightness);
  for (byte disp = 0; disp < 4; disp++) {
    for (byte i = 0; i < 10; i++) {
      showDigit(i, startColor + i, disp);
      if (i % 3 == 0) {
        showDots(startColor + 32);
      }
      FastLED.show();
      delay(150);
      FastLED.clear();
    }
  }
  for (byte i = 0; i <= LED_COUNT; i++) {
    leds[i].setHSV(startColor + i * 2, saturation, brightness);
    FastLED.show();
    delay(30);
  }
  startColor++;
}

void showSegment(byte segment, byte color, byte segDisplay) {
  for (byte i = 0; i < 3; i++) {
   if (segDisplay == 0) leds[(segGroups[segment][i])].setHSV(color, saturation, brightness);
   if (segDisplay == 1) leds[(segGroups[segment + 7][i])].setHSV(color, saturation, brightness);
   if (segDisplay == 2) leds[(segGroups[segment][i] + 52)].setHSV(color, saturation, brightness);
   if (segDisplay == 3) leds[(segGroups[segment + 7][i] + 52)].setHSV(color, saturation, brightness);
  }
}

void showDigit(byte digit, byte color, byte segDisplay) {
  for (byte i = 0; i < 7; i++) {
    if (digits[digit][i] != 0) {
      showSegment(i, color, segDisplay);
    }
  }
}

void showDots(byte color) {
  leds[47].setHSV(color, saturation, brightness);
  leds[48].setHSV(color, saturation, brightness);
  leds[50].setHSV(color, saturation, brightness);
  leds[51].setHSV(color, saturation, brightness);
}
