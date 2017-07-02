#include <Arduino.h>
#include <FastLED.h>
#include <Wire.h>
#include <MSGEQ7.h>
#include <Visualization.h>
#include <Streak.h>
// #include <Ladder.h>
#include <Sparkle.h>
// #include <Pulse.h>
#include <Frequency.h>

// MSGEQ7
#define pinAnalog A0  // BLUE HARNESS
#define pinReset 30   // BLUE R
#define pinStrobe 31  // GREEN S
#define MSGEQ7_INTERVAL ReadsPerSecond(100)
#define MSGEQ7_SMOOTH 191 // Range: 0-255

CMSGEQ7<MSGEQ7_SMOOTH, pinReset, pinStrobe, pinAnalog> MSGEQ7;

// FAST LED
#define NUM_LEDS 150
#define ROWS 15
#define COLUMNS 10
#define DISPLAY_LED_PIN 4

CRGB leds[NUM_LEDS];
CRGB off;

void clear();
void setAll(CRGB color);

#define NUM_STREAKS 24

CRGB pink = 0xFF0B20;
CRGB blue = 0x0BFFDD;
CRGB green = 0xB9FF0B;

CRGB colors[3];
Streak * streaks[NUM_STREAKS];
Sparkle * sparkles[3];
Frequency * frequency;

int frequencies[7];

int active = 0;

void setup() {
  unsigned int i;

  Serial.begin(9600);
  MSGEQ7.begin();

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

  frequency = new Frequency(COLUMNS, ROWS, leds, blue);

  Serial.println("setup complete");
}

void loop() {
  unsigned int i;
  CRGB color;
  clear();  // this only clears the array, not the LEDs, it's fine at the top

  bool newReading = MSGEQ7.read(MSGEQ7_INTERVAL);
  if (newReading) {
    // Read bass frequency
    frequencies[0] = MSGEQ7.get(MSGEQ7_0);
    frequencies[0] = mapNoise(frequencies[0]);
    frequencies[1] = MSGEQ7.get(MSGEQ7_1);
    frequencies[1] = mapNoise(frequencies[1]);
    frequencies[2] = MSGEQ7.get(MSGEQ7_2);
    frequencies[2] = mapNoise(frequencies[2]);
    frequencies[3] = MSGEQ7.get(MSGEQ7_3);
    frequencies[3] = mapNoise(frequencies[3]);
    frequencies[4] = MSGEQ7.get(MSGEQ7_4);
    frequencies[4] = mapNoise(frequencies[4]);
    frequencies[5] = MSGEQ7.get(MSGEQ7_5);
    frequencies[5] = mapNoise(frequencies[5]);
    frequencies[6] = MSGEQ7.get(MSGEQ7_6);
    frequencies[6] = mapNoise(frequencies[6]);
  }

  unsigned long currentTime = millis();

  frequency->display(currentTime, frequencies);


  // for(i=0; i<NUM_STREAKS; i++) {
  //   streaks[i]->display(currentTime);
  // }
  //
  for(i=0; i<3; i++) {
    sparkles[i]->display();
  }

  // setAll(blue);

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
