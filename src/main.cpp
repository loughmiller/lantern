#include <Arduino.h>
#include <FastLED.h>
#include <Wire.h>
#include <Visualization.h>
#include <Streak.h>
#include <Sparkle.h>
#include <RainbowDrop.h>

// FAST LED
// #define NUM_LEDS 310
// #define ROWS 38
// #define COLUMNS 8
#define NUM_LEDS 150
#define ROWS 150
#define COLUMNS 1
#define DISPLAY_LED_PIN 22

CRGB leds[NUM_LEDS];
CRGB off;

void clear();
void setAll(CRGB color);

#define NUM_STREAKS 9
#define NUM_SPARKELS 3

CRGB pink = 0xFF0B20;
CRGB blue = 0x0BFFDD;
CRGB green = 0xB9FF0B;
CRGB soundBlue = 0x0BFFDD;

CRGB colors[3];
Streak * streaks[NUM_STREAKS];
Sparkle * sparkle;

RainbowDrop * rainbowDrop;

enum States { streaksState, rainbowState };
States currentVisualization;

void setup() {
  uint16_t i;
  delay(2000);

  Serial.begin(9600);
  Serial.println("setup started");

  off = 0x000000;
  setAll(off);
  FastLED.show();

  // DISPLAY STUFF
  colors[0] = green;
  colors[1] = pink;
  colors[2] = blue;

  FastLED.addLeds<NEOPIXEL, DISPLAY_LED_PIN>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );;

  setAll(0xFFFFFF);
  FastLED.show();
  delay(2000);

  for(i=0; i<NUM_STREAKS; i++) {
    streaks[i] = new Streak(COLUMNS, ROWS, leds, colors[i % 3]);
    streaks[i]->setLengthMinMax(13, 31);
    streaks[i]->setIntervalMinMax(5, 25);
  }

  sparkle = new Sparkle(COLUMNS, ROWS, leds, 0xFFFFFF, 207);

  rainbowDrop = new RainbowDrop(COLUMNS, ROWS, leds);
  rainbowDrop->setInterval(1);

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

  //if (currentVisualization == streaksState) {
    for(i=0; i<NUM_STREAKS; i++) {
      streaks[i]->display(currentTime);
    }
  //}

  //sparkle->display();
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
