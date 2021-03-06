#include "Timer.h"
#include "animation.h"
#include <Arduino.h>
#include <FastLED.h>

#define FASTLED_ALLOW_INTERRUPTS 0

byte getPos(byte pos);
byte mapMinMax(byte v, byte oldMin, byte oldMax, byte newMin, byte newMax);
void drawNumber(byte number, byte pos, CHSV c);
void populateCoords();

#define ROWS 11
#define COLS 7
// Sets if the LED strips start at the bottom-left, or top-left.
#define STARTBOTTOM true
// Sets how many deadpixels there are at the top of the LEDSTRIP in the turn.
#define DEADPIXELS 2
#define LED_PIN 6
#define LEDCOUNT ROWS *COLS + COLS / 2 * DEADPIXELS
#define FREQUENCY 100
#define SPREAD 6

byte coords[COLS][ROWS];
CRGB leds[LEDCOUNT];
Timer timer;

Animation c[4] = {
    Animation(FREQUENCY, SPREAD * 0), Animation(FREQUENCY, SPREAD * 1),
    Animation(FREQUENCY, SPREAD * 2), Animation(FREQUENCY, SPREAD * 3)};

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, LEDCOUNT);
  populateCoords();

  timer.init();
}
void loop() {
  if (Serial.available() > 0) {
    timer.getSerial();
  };
  FastLED.clear();

  DateTime t = timer.getTime();
  byte h = t.hour();
  byte m = t.minute();

  drawNumber(floor(h / 10), 0, c[0].cycleHSV());
  drawNumber(h % 10, 1, c[1].cycleHSV());
  drawNumber(floor(m / 10), 2, c[2].cycleHSV());
  drawNumber(m % 10, 3, c[3].cycleHSV());

  FastLED.show();
}

byte getPos(byte pos) {
  return (DEADPIXELS > 0) ? pos + floor((pos + ROWS) / (ROWS * 2)) * DEADPIXELS
                          : pos;
}

byte mapMinMax(byte v, byte oMin, byte oMax, byte nMin, byte nMax) {
  return (nMax - nMin) * (v - oMin) / (oMax - oMin) + nMin;
}

byte numbers[10][3][5] = {
    {{1, 1, 1, 1, 1}, {1, 0, 0, 0, 1}, {1, 1, 1, 1, 1}},  // 0
    {{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {1, 1, 1, 1, 1}},  // 1
    {{1, 0, 1, 1, 1}, {1, 0, 1, 0, 1}, {1, 1, 1, 0, 1}},  // 2
    {{1, 0, 1, 0, 1}, {1, 0, 1, 0, 1}, {1, 1, 1, 1, 1}},  // 3
    {{1, 1, 1, 0, 0}, {0, 0, 1, 0, 0}, {1, 1, 1, 1, 1}},  // 4
    {{1, 1, 1, 0, 1}, {1, 0, 1, 0, 1}, {1, 0, 1, 1, 1}},  // 5
    {{1, 1, 1, 1, 1}, {1, 0, 1, 0, 1}, {1, 0, 1, 1, 1}},  // 6
    {{1, 0, 0, 0, 0}, {1, 0, 0, 0, 0}, {1, 1, 1, 1, 1}},  // 7
    {{1, 1, 1, 1, 1}, {1, 0, 1, 0, 1}, {1, 1, 1, 1, 1}},  // 8
    {{1, 1, 1, 0, 1}, {1, 0, 1, 0, 1}, {1, 1, 1, 1, 1}}}; // 9

byte numberCoords[4][2] = {{0, 0}, {4, 0}, {0, 6}, {4, 6}};

void drawNumber(byte digit, byte pos, CHSV c) {
  for (byte oY = 0; oY < 5; oY++) {
    for (byte oX = 0; oX < 3; oX++) {
      byte x = numberCoords[pos][0] + oX;
      byte y = numberCoords[pos][1] + oY;
      byte led = coords[x][y];
      if (numbers[digit][oX][oY]) {
        leds[led] = c;
      }
    }
  }
}

void populateCoords() {
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
