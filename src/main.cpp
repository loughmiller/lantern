#include <Arduino.h>
#include <FastLED.h>
#include <Wire.h>
#include <MSGEQ7.h>
#include <Visualization.h>
#include <Streak.h>
#include <Ladder.h>
#include <Sparkle.h>
#include <Pulse.h>
#include <Frequency.h>

// FAST LED
#define NUM_LEDS 150
#define ROWS 15
#define COLUMNS 10
#define DISPLAY_LED_PIN 5

CRGB leds[NUM_LEDS];
CRGB off;

void clear();
void setAll(CRGB color);

#define NUM_STREAKS 5
#define NUM_SPARKELS 3

CRGB pink = 0xFF0B20;
CRGB blue = 0x0BFFDD;
CRGB green = 0xB9FF0B;

CRGB colors[3];
Streak * streaks[NUM_STREAKS];
Sparkle * sparkles[3];
Frequency * frequency;

uint8_t frequencies[7];

void setup() {
  uint16_t i;
  delay(2000);

  Serial.begin(9600);
  Serial.println("setup started");

  // DISPLAY STUFF
  colors[0] = green;
  colors[1] = pink;
  colors[2] = blue;

  // FastLED.setBrightness(64);
  off = 0x000000;
  FastLED.addLeds<NEOPIXEL, DISPLAY_LED_PIN>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );;

//  FastLED.setBrightness(64);
  setAll(0xFFFFFF);
  FastLED.show();
  delay(2000);

  for(i=0; i<NUM_STREAKS; i++) {
    streaks[i] = new Streak(COLUMNS, ROWS, leds, colors[i % 2]);
    streaks[i]->setLengthMinMax(4, 8);
    streaks[i]->setIntervalMinMax(40, 160);
  }

  for(i=0; i<NUM_SPARKELS; i++) {
    sparkles[i] = new Sparkle(COLUMNS, ROWS, leds, colors[i % 3], 407);
  }

  // frequency = new Frequency(COLUMNS, ROWS, leds, blue);

  Serial.println("setup complete");
}

void loop() {
  unsigned int i;
  CRGB color;

  setAll(0x030303); // this only clears the array, not the LEDs, it's fine at the top

  unsigned long currentTime = millis();

  for(i=0; i<NUM_STREAKS; i++) {
    streaks[i]->display(currentTime);
  }

  for(i=0; i<NUM_SPARKELS; i++) {
    sparkles[i]->display();
  }

  // setAll(green);
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
