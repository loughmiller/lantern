#include <Arduino.h>
#include <FastLED.h>
#include <Wire.h>
#include <Visualization.h>
#include <Streak.h>
#include <Ladder.h>
#include <Sparkle.h>
#include <Pulse.h>
#include <Frequency.h>

#define NUM_LEDS 150
#define ROWS 15
#define COLUMNS 10
#define DISPLAY_LED_PIN 3

CRGB leds[NUM_LEDS];
CRGB off;

void clear();
void setAll();

#define NUM_STREAKS 24

CRGB pink = 0xFF0B20;
CRGB blue = 0x0BFFDD;
CRGB green = 0xB9FF0B;

CRGB colors[3];
Streak * streaks[NUM_STREAKS];

Sparkle * sparkles[3];

int active = 0;

void setup() {
  unsigned int i;

  Serial.begin(9600);

  // DISPLAY STUFF
  colors[0] = green;
  colors[1] = blue;
  colors[2] = pink;

  FastLED.setBrightness(64);
  off = 0x000000;
  FastLED.addLeds<NEOPIXEL, DISPLAY_LED_PIN>(leds, NUM_LEDS).setCorrection( Typical8mmPixel );;

  clear();
  FastLED.show();
  delay(2000);

  for(i=0; i<NUM_STREAKS; i++) {
    streaks[i] = new Streak(COLUMNS, ROWS, leds, colors[i % 3]);
    streaks[i]->setLengthMinMax(4, 8);
    streaks[i]->setIntervalMinMax(40, 160);
  }

  for(i=0; i<3; i++) {
    sparkles[i] = new Sparkle(COLUMNS, ROWS, leds, colors[i % 3], 607);
  }
}

void loop() {
  unsigned int i;
  CRGB color;

  unsigned long currentTime = millis();

  clear();

  for(i=0; i<NUM_STREAKS; i++) {
    streaks[i]->display(currentTime);
  }

  for(i=0; i<3; i++) {
    sparkles[i]->display();
  }

  FastLED.show();
}

void setAll(CRGB color) {
  for (int i=0; i<NUM_LEDS; i++) {
    leds[i] = color;
  }
}

void clear() {
  setAll(off);
}
