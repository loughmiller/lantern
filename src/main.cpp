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
#define NUM_LEDS 960
#define ROWS 120
#define COLUMNS 8
#define DISPLAY_LED_PIN 22

#define AUDIO_INPUT_PIN A14

CRGB leds[NUM_LEDS];
CRGB off;

Visualization * all;
Sparkle * sparkle;
Streak * streak;
RainbowDrop * rainbowDrop;

void setup() {
  delay(1000);

  Serial.begin(9600);
  Serial.println("setup started");

  all = new Visualization(COLUMNS, ROWS, 0, 0, leds);
  all->setAllCRGB(0x000000);
  FastLED.show();

  // AUDIO setup
  TeensyAudioFFTSetup(AUDIO_INPUT_PIN);
  samplingBegin();

  FastLED.addLeds<NEOPIXEL, DISPLAY_LED_PIN>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );;

  all->setAllCRGB(0x303030);
  FastLED.show();
  delay(1000);

  sparkle = new Sparkle(NUM_LEDS, 0, 0, leds, 567);
  rainbowDrop = new RainbowDrop(COLUMNS, ROWS, 225, leds);
  streak = new Streak(COLUMNS, ROWS, 0, 255, leds);
  streak->setLengthMinMax(7, 12);
  streak->setIntervalMinMax(30, 70);
  streak->setFade(false);
  streak->setRandomHue(true);

  Serial.println("setup complete");
}

void loop() {
  unsigned long currentTime = millis();
  float intensity = readRelativeIntensity(currentTime, 2, 4);

  rainbowDrop->display(currentTime, intensity);
  streak->display(currentTime);
  sparkle->display();

  FastLED.show();
}
