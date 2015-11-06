#include <FastLED.h>

#define NUM_LEDS 35 // The number of LEDs in our strip
#define STRIP    6 // The pin to which the strip is connected
const int sensorPin   = A0; // The pin to which the sensor is connected

int ledToLight      = 0,   // The specific LED from within the array
    airPressure     = 0,   // The current state of the pressure in our system
    interval        = 500, // The speed at which we animate
    maxSpectrum     = 1000,
    minSpectrum     = 1,
    maxMessured     = 1,   // Initiate spectrum ends
    minMessured     = maxSpectrum, // 
    prevAirPressure = 0,
    isHugging       = false,
    superHugReactionDuration = 2000,
    superHugDuration = 2000;

long  lastMillis        = 0, // The last time we called blinkAndStep()
      superHugEndTime   = 0,
      hugStartTime = 1; 

// Constructor functions
CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(115200);
  pinMode(sensorPin,INPUT);
  FastLED.addLeds<NEOPIXEL, STRIP>(leds, NUM_LEDS); // Setup the LED strip
}
 
void loop() {

  prevAirPressure = airPressure;
  airPressure = analogRead(sensorPin);
  autoTune();

  if (prevAirPressure < maxSpectrum*0.65 && airPressure > maxSpectrum*0.65 && isHugging==false) {
    hugStartTime = millis();
    isHugging = true;
    
  };

  if(airPressure < maxSpectrum/2 && isHugging==true ){
    isHugging = false;
  };

  if (millis()-hugStartTime > superHugDuration && isHugging) {
    superHugEndTime = millis() + superHugReactionDuration;
    Serial.println("Just SUPER hugged!"); 
  }
  if (millis() - lastMillis > interval/airPressure){
    if(superHugEndTime - millis() > 0) {
      lightEmUp();
    } else {
      blinkAndStep();
    }
    lastMillis = millis();
  }  
}

// Determine the spectrum of air pressure
void autoTune() {
  if (airPressure < minMessured) {
    minMessured = airPressure;
  } else if (airPressure > maxMessured) {
    maxMessured = airPressure;
  }
  airPressure = map(airPressure, minMessured, maxMessured, minSpectrum, maxSpectrum);
  airPressure = constrain(airPressure, minSpectrum, maxSpectrum);
}
// Blink an LED, and set the position of the next LED to light
void blinkAndStep() {
  for(int i=0; i<NUM_LEDS; i++){
    leds[i] = CRGB::Black;
  };
  if(ledToLight > NUM_LEDS){ledToLight=0;};
  leds[ledToLight].setRGB(airPressure, 68, 255/airPressure);;
  FastLED.show();
  leds[ledToLight] = CRGB::Black;
  FastLED.show();
  ledToLight++;
}

void lightEmUp() {
  for(int i=0; i<NUM_LEDS; i++){
    leds[i] = CRGB::Purple;
  };
  FastLED.show();
}
