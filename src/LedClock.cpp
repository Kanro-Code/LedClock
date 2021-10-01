#include <Arduino.h>
#include <FastLED.h>
#define FASTLED_ALLOW_INTERRUPTS 0

byte getPos(byte pos);
byte mapMinMax(byte v, byte oldMin, byte oldMax, byte newMin, byte newMax);

#define ROWS 11
#define COLS 7
// Sets if the LED strips start at the bottom-left, or top-left.
#define STARTBOTTOM true
// Sets how many deadpixels there are at the top of the LEDSTRIP in the turn.
#define DEADPIXELS 2
#define LED_PIN 6
#define LEDCOUNT ROWS *COLS + COLS / 2 * DEADPIXELS

byte coords[COLS][ROWS];
CRGB leds[LEDCOUNT];

void setup() {
  Serial.begin(57600);
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, LEDCOUNT);

  byte led = 0;
  for (byte x = 0; x < COLS; x++) {
    for (byte y = 0; y < ROWS; y++) {
      coords[x][y] = getPos(led++);
    }
  }

  for (byte x = (STARTBOTTOM == true) ? 0 : 1; x < COLS; x = x + 2) {
    for (byte y = 0; y < ROWS / 2; y++) {
      byte eq = mapMinMax(y, 0, ROWS - 1, ROWS - 1, 0);
      byte swap = coords[x][eq];

      coords[x][eq] = coords[x][y];
      coords[x][y] = swap;
    }
  }
}

byte k = 0;
void loop() {
  for (byte x = 0; x < COLS; x++) {
    for (byte y = 0; y < ROWS; y++) {
      FastLED.clear();
      leds[coords[x][y]] = CHSV(random(0,255),random(0,255),random(0,255));
      FastLED.show();
      delay(10);
    }
  }
}

byte getPos(byte pos) {
  return (DEADPIXELS > 0) ? pos + floor((pos + ROWS) / (ROWS * 2)) * DEADPIXELS
                          : pos;
}

byte mapMinMax(byte v, byte oMin, byte oMax, byte nMin, byte nMax) {
  return (nMax - nMin) * (v - oMin) / (oMax - oMin) + nMin;
}
