#include <FastLED.h>

#define NUM_LEDS 34 // The number of LEDs in our strip
#define STRIP    3 // The pin to which the strip is connected
const int sensorPin   = A3; // The pin to which the sensor is connected

int ledToLight      = 0,   // The specific LED from within the array
    airPressure     = 0,   // The current state of the pressure in our system
    airPressureRGB,
    interval        = 20, // The speed at which we animate
    maxSpectrum     = 1000,
    minSpectrum     = 1,
    maxMessured     = 1,   // Initiate spectrum ends
    minMessured     = maxSpectrum, //
    prevAirPressure = 0,
    isHugging       = false,
    superHugReactionDuration = 5000,
    superHugDuration = 5000;

long  curTime = 0,
      lastMillis        = 0, // The last time we called blinkAndStep()
      superHugEndTime   = 0,
      hugStartTime = 1;

// Constructor functions
CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(115200);
  pinMode(sensorPin,INPUT);
  LEDS.addLeds<NEOPIXEL, STRIP>(leds, NUM_LEDS); // Setup the LED strip
  LEDS.setBrightness(50);
}


// Determine the spectrum of air pressure
void autoTune() {
  if (airPressure < minMessured) {
    minMessured = airPressure;
  } else if (airPressure > maxMessured) {
    maxMessured = airPressure;
  }
  airPressureRGB = map(airPressure, minMessured, maxMessured, 1, 255);
  airPressureRGB = map(airPressure, minMessured, maxMessured, 1, 255);
  airPressure = map(airPressure, minMessured, maxMessured, minSpectrum, maxSpectrum);
  airPressure = constrain(airPressure, minSpectrum, maxSpectrum);
}

// Blink an LED, and set the position of the next LED to light
void blinkAndStep() {
  if(ledToLight > NUM_LEDS){ledToLight=0;};
  LEDS.setBrightness(constrain(20+airPressureRGB, 1, 100));
  leds[ledToLight] = CHSV(constrain(20+airPressureRGB,1, 255), 255, 255);
  Serial.println(airPressureRGB);
  LEDS.show();
  ledToLight++;
}

// Light up the entire strip
void lightEmUp() {
  for(int i=0; i<NUM_LEDS; i++){
    leds[i].setRGB(7*i, 68, 255/i);
  };
  LEDS.show();
}

void loop() {
  curTime = millis();
  prevAirPressure = airPressure;
  airPressure = analogRead(sensorPin);
  autoTune();
  if (prevAirPressure < maxSpectrum*0.65 && airPressure > maxSpectrum*0.65 && isHugging==false) {
    hugStartTime = curTime;
    isHugging = true;
  };

  if(airPressure < maxSpectrum/2 && isHugging==true ){
    isHugging = false;
  };

  if (curTime-hugStartTime > superHugDuration && isHugging) {
    superHugEndTime = curTime + superHugReactionDuration;
  }

  if (curTime - lastMillis > interval){
    blinkAndStep();
    lastMillis = curTime;
  }
}
