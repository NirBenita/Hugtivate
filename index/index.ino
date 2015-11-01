#include <FastLED.h>

#define NUM_LEDS 35 // The number of LEDs in our strip
#define STRIP    6 // The pin to which the strip is connected
const int sensorPin   = A0; // The pin to which the sensor is connected

int ledToLight      = 0,   // The specific LED from within the array
    airPressure     = 0,   // The current state of the pressure in our system
    interval        = 500, // The speed at which we animate
    maxMessured     = 0,   // Initiate spectrum ends
    minMessured     = 600, // 
    prevAirPressure = 0,
    hugStartTime,
    superHugEndTime;

const int superHugReactionDuration = 2000,
          superHugDuration = 5000,
          maxSpectrum     = 255,
          minSpectrum     = 1;

long lastMillis     = 0; // The last time we called blinkAndStep()

// Constructor functions
CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(115200);
  pinMode(sensorPin,INPUT);
  FastLED.addLeds<NEOPIXEL, STRIP>(leds, NUM_LEDS); // Setup the LED strip
}
 
void loop() {
  prevAirPressure = airPressure;
  airPressure = analogRead(sensorPin); // First read
  autoTune();
  if (prevAirPressure < maxSpectrum/2 && airPressure > maxSpectrum/2) {
    hugStartTime = millis();
  }
  if (millis()-hugStartTime > superHugDuration) {
    lightEmUp();
  }
}

// Determine the spectrum of air pressure
void autoTune() {
  if (airPressure < minMessured) {
    minMessured = airPressure;
  }
  if (airPressure > maxMessured) {
    maxMessured = airPressure;
  }
  airPressure = map(airPressure, minMessured, maxMessured, minSpectrum, maxSpectrum);
  airPressure = constrain(airPressure, minSpectrum, maxSpectrum);
}
// Blink an LED, and set the position of the next LED to light
void blinkAndStep() {
   if (ledToLight > NUM_LEDS){
    ledToLight=0;
    };
  leds[ledToLight].setRGB(airPressure, 68, 255/airPressure);;
  FastLED.show();
  leds[ledToLight] = CRGB::Black;
  FastLED.show();
  ledToLight++;
}

void lightEmUp() {
  leds[0].setRGB(20,234,13);
  FastLED.show();
  superHugEndTime = millis()+ superHugReactionDuration;
}

        
