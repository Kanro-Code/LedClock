#include <Arduino.h>
#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>

byte getPos(byte pos);
byte mapMinMax(byte v, byte oldMin, byte oldMax, byte newMin, byte newMax);

#define LED_COUNT 83
#define ROWS 11
#define COLS 7
#define BOTTOMLEFT true
#define CUT_CORNER true
#define LED_PIN 6

byte coords[COLS][ROWS];
CRGB leds[LED_COUNT];

void setup() {
  Serial.begin(57600);
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, LED_COUNT);

  byte led = 0;
  for (byte x = 0; x < COLS; x++) {
    for (byte y = 0; y < ROWS; y++) {
      coords[x][y] = getPos(led++);
    }
  }

  for (byte x = (BOTTOMLEFT == true) ? 0 : 1; x < COLS; x = x + 2) {
    for (byte y = 0; y < ROWS / 2; y++) {
      byte eq = mapMinMax(y, 0, ROWS - 1, ROWS - 1, 0);
      byte swap = coords[x][eq];

      coords[x][eq] = coords[x][y];
      coords[x][y] = swap;
    }
  }

  for (int x = 0; x < COLS; x++) {
    for (int y = 0; y < ROWS; y++) {
      Serial.println(coords[x][y]);
    }
  }
}

byte k = 0;
void loop() {
  for (byte x = 0; x < COLS; x++) {
    for (byte y = 0; y < ROWS; y++) {
      FastLED.clear();
      leds[coords[x][y]] = CRGB::Red;
      FastLED.show();
      delay(10);
    }
  }
}

byte getPos(byte pos) {
  if (CUT_CORNER == true) {
    return pos + floor((pos + ROWS) / (ROWS * 2)) * 2;
  } else {
    return pos;
  }
}

byte mapMinMax(byte v, byte oMin, byte oMax, byte nMin, byte nMax) {
  return (nMax - nMin) * (v - oMin) / (oMax - oMin) + nMin;
}
