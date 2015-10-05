#include <FastLED.h>

#define NUM_LEDS 35
#define STRIP    6

const int sensorPin   = A0;
int pinToLight   = 0,
    airPressure,
    pressureToRGB,
    interval     = 300,
    high            = 0,
    low             = 600;
long lastMillis  = 0;
boolean isOn     = false;

// Constructor functions
CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(115200);
  pinMode(sensorPin,INPUT);
  // Setup the LED strip
  FastLED.addLeds<NEOPIXEL, STRIP>(leds, NUM_LEDS);
}
 
void loop() {
  airPressure = analogRead(sensorPin);
  Serial.print("Air Pressure:");
  Serial.println(airPressure);
  autoTune();
  Serial.print("After auto Tune:");
  Serial.println(airPressure);
  if (millis() - lastMillis > interval/airPressure){
    blinkAndStep();
    lastMillis = millis();
  }
}

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

void blinkAndStep() {
   if (pinToLight > NUM_LEDS){
    pinToLight=0;
  };
  leds[pinToLight].setRGB(pressureToRGB, 68, 255/pressureToRGB);;
  FastLED.show();
  leds[pinToLight] = CRGB::Black;
  FastLED.show();
  pinToLight++;
}

