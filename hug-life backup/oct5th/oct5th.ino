#include <SimpleTimer.h>
#include <FastLED.h>

#define NUM_LEDS    35
#define DATA_PIN    6

boolean isOn = false;
int glowPin  = 0;

// Constructor functions
SimpleTimer timer;
CRGB leds[NUM_LEDS];

void animateStrip() {
   if (glowPin > NUM_LEDS){
    glowPin=0;
  };
  leds[glowPin].setRGB(96, 44, 113);
  FastLED.show();
  leds[glowPin] = CRGB::Black;
  FastLED.show();
  glowPin++;
}

void setup() {
  Serial.begin(9600);
  // Setup the LED strip
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  // timed actions setup
  timer.setInterval(500, animateStrip);
}
 
void loop() {
  // this is where the "polling" occurs
  timer.run();
}


