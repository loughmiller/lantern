#include <Arduino.h>
#include <FastLED.h>
#include <Wire.h>
#include <Visualization.h>
#include <Streak.h>
#include <Sparkle.h>
#include <SoundReaction.h>
#include <RainbowDrop.h>
#include <TeensyAudioFFT.h>

// FAST LED
#define NUM_LEDS 150
#define ROWS 15
#define COLUMNS 10
#define DISPLAY_LED_PIN 5

#define AUDIO_INPUT_PIN A14

CRGB leds[NUM_LEDS];
CRGB off;

void clear();
void setAll(CRGB color);

#define NUM_STREAKS 5
#define NUM_SPARKELS 3

CRGB pink = 0xFF0B20;
CRGB blue = 0x0BFFDD;
CRGB green = 0xB9FF0B;
CRGB soundBlue = 0x0BFFDD;

CRGB colors[3];
Streak * streaks[NUM_STREAKS];
Sparkle * sparkle;
SoundReaction * soundReactionA;
SoundReaction * soundReactionB;

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

  // AUDIO setup
  TeensyAudioFFTSetup(AUDIO_INPUT_PIN);
  samplingBegin();

  // DISPLAY STUFF
  colors[0] = green;
  colors[1] = pink;
  colors[2] = blue;

  FastLED.addLeds<NEOPIXEL, DISPLAY_LED_PIN>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );;

  setAll(0xFFFFFF);
  FastLED.show();
  delay(2000);

  for(i=0; i<NUM_STREAKS; i++) {
    streaks[i] = new Streak(COLUMNS, ROWS, leds, colors[i % 2]);
    streaks[i]->setLengthMinMax(4, 8);
    streaks[i]->setIntervalMinMax(40, 160);
  }

  sparkle = new Sparkle(COLUMNS, ROWS, leds, 0xFFFFFF, 207);

  //soundBlue.fadeLightBy(192);
  soundReactionA = new SoundReaction(0, 15, leds, soundBlue, 0xBBBBBB);
  soundReactionB = new SoundReaction(75, 90, leds, soundBlue, 0xBBBBBB);

  rainbowDrop = new RainbowDrop(COLUMNS, ROWS, leds);

  currentVisualization = rainbowState;

  Serial.println("setup complete");
}

void loop() {
  unsigned int i;
  CRGB color;

  setAll(0x303030); // this only clears the array, not the LEDs, it's fine at the top

  unsigned long currentTime = millis();
  float intensity = readRelativeIntensity(currentTime, 2, 4);

  if (currentVisualization == streaksState) {
    soundReactionA->display(intensity);
    soundReactionB->display(intensity);

    for(i=0; i<NUM_STREAKS; i++) {
      streaks[i]->display(currentTime);
    }
  }

  if (currentVisualization == rainbowState) {
    rainbowDrop->display(currentTime, intensity);
    // sparkle->display();
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
