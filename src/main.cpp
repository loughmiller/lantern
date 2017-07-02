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

#define NUM_STREAKS 8

CRGB pink = 0xFF0B20;
CRGB blue = 0x0BFFDD;
CRGB green = 0xB9FF0B;

Streak * greenS[NUM_STREAKS];
Streak * blueS[NUM_STREAKS];
Streak * pinkS[NUM_STREAKS];

Sparkle * s1;
Sparkle * s2;

int active = 0;

void setup() {
  Serial.begin(9600);

  // DISPLAY STUFF
  FastLED.setBrightness(32);
  off = 0x000000;
  FastLED.addLeds<NEOPIXEL, DISPLAY_LED_PIN>(leds, NUM_LEDS).setCorrection( Typical8mmPixel );;

  clear();
  FastLED.show();
  delay(2000);

  for(unsigned int i=0; i<NUM_STREAKS; i++) {
    greenS[i] = new Streak(COLUMNS, ROWS, leds, green);
    blueS[i] = new Streak(COLUMNS, ROWS, leds, blue);
    pinkS[i] = new Streak(COLUMNS, ROWS, leds, pink);
  }

  s1 = new Sparkle(1, NUM_LEDS, leds, blue, 201);
}

void loop() {
  CRGB color;

  unsigned long currentTime = millis();

  clear();

  s1->display();

  for(unsigned int i=0; i<NUM_STREAKS; i++) {
    pinkS[i]->display(currentTime);
    blueS[i]->display(currentTime);
    greenS[i]->display(currentTime);
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
