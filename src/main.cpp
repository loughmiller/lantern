#include <Arduino.h>
#include <FastLED.h>
#include <Wire.h>
#include <Visualization.h>
#include <Streak.h>
#include <Sparkle.h>
#include <RainbowDrop.h>

// FAST LED
#define NUM_LEDS 310
#define ROWS 38
#define COLUMNS 8
#define DISPLAY_LED_PIN 22

CRGB leds[NUM_LEDS];
CRGB off;

void clear();
void setAll(CRGB color);

Streak * streak;
Sparkle * sparkle;
RainbowDrop * rainbowDrop;

enum States { streaksState, rainbowState };
States currentVisualization;

void setup() {
  uint16_t i;
  delay(1000);

  Serial.begin(9600);
  Serial.println("setup started");

  off = 0x000000;
  setAll(off);
  FastLED.show();

  FastLED.addLeds<NEOPIXEL, DISPLAY_LED_PIN>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );;

  setAll(0x303030);
  FastLED.show();
  delay(1000);

  streak = new Streak(COLUMNS, ROWS, 245, 244, leds);
  streak->setLengthMinMax(8, 17);
  streak->setIntervalMinMax(2, 10);

  sparkle = new Sparkle(NUM_LEDS, 0, 0, leds, 507);

  rainbowDrop = new RainbowDrop(COLUMNS, ROWS, 225, leds);
  rainbowDrop->setInterval(10);

  currentVisualization = streaksState;

  Serial.println("setup complete");
}

void loop() {
  unsigned int i;
  CRGB color;

  // setAll(0x303030); // this only clears the array, not the LEDs, it's fine at the top

  unsigned long currentTime = millis();

  //if (currentVisualization == rainbowState) {
    rainbowDrop->display(currentTime, 0);
  //}

  // if (currentVisualization == streaksState) {
  //   for(i=0; i<NUM_STREAKS; i++) {
  //     streaks[i]->display(currentTime);
  //   }
  // }

  streak->display(currentTime);
  sparkle->display();
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
