#include <FastLED.h>

#define NUM_LEDS 35
#define STRIP    6
#define SENSOR_PIN  A0

int pinToLight   = 0,
    airPressure,
    interval     = 50,
    high            = 0,
    low             = 600;
long lastMillis  = 0;
boolean isOn     = false;

// Constructor functions
CRGB leds[NUM_LEDS];

void setup() {
  // Setup the LED strip
  FastLED.addLeds<NEOPIXEL, STRIP>(leds, NUM_LEDS);
}
 
void loop() {
  _autoTune();
  if (millis() - lastMillis > interval){
    blinkAndStep();
    lastMillis = millis();
  }
}

void _autoTune() {
  if (airPressure < low) {
    low = airPressure;
  }
  if (airPressure > high) {
    high = airPressure;
  }
  airPressure = map(airPressure, low, high, 1, 100);
  airPressure = constrain(airPressure, 1, 100);
}

void blinkAndStep() {
   if (pinToLight > NUM_LEDS){
    pinToLight=0;
  };
  leds[pinToLight].setRGB(96, 44, 113);
  FastLED.show();
  leds[pinToLight] = CRGB::Black;
  FastLED.show();
  pinToLight++;
}