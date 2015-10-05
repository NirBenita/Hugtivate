#include <FastLED.h>

#define NUM_LEDS 35 // The number of LEDs in our strip
#define STRIP    6 // The pin to which the strip is connected
const int sensorPin   = A0; // The pin to which the sensor is connected

int ledToLight   = 0, // The specific LED from within the array
    airPressure, // The current state of the pressure in our system
    pressureToRGB, // A representation of airPressure between 1-255
    interval     = 300, // The speed at which we animate
    high            = 0, // Maximum air pressure
    low             = 600; // Static air pressure
long lastMillis  = 0; // The last time we called blinkAndStep()

// Constructor functions
CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(115200);
  pinMode(sensorPin,INPUT);
  FastLED.addLeds<NEOPIXEL, STRIP>(leds, NUM_LEDS); // Setup the LED strip
}
 
void loop() {
  airPressure = analogRead(sensorPin); // First read
  autoTune();
  if (millis() - lastMillis > interval/airPressure){
    blinkAndStep();
    lastMillis = millis();
  }
}

// Determine the spectrum of air pressure
void autoTune() {
  if (airPressure < low) {
    low = airPressure;
  }
  if (airPressure > high) {
    high = airPressure;
  }
  pressureToRGB = map(airPressure, low, high, 1, 255);
  pressureToRGB = constrain(pressureToRGB, 1, 255);
  airPressure = map(airPressure, low, high, 1, 200);
  airPressure = constrain(airPressure, 1, 200);
}
// Blink an LED, and set the position of the next LED to light
void blinkAndStep() {
   if (ledToLight > NUM_LEDS){
    ledToLight=0;
  };
  leds[ledToLight].setRGB(pressureToRGB, 68, 255/pressureToRGB);;
  FastLED.show();
  leds[ledToLight] = CRGB::Black;
  FastLED.show();
  ledToLight++;
}