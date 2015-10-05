#include <FastLED.h>
#define NUM_LEDS    34
#define DATA_PIN    6
#define SENSOR_PIN  A0

int airPressure,
    barometricToRGB,
    frequency       = 0,
    high            = 0,
    low             = 600,
    interval        = 50;

long previousMillis = 0;

CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(115200);
  pinMode(SENSOR_PIN,INPUT);  
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
}

void loop() {
  airPressure = analogRead(SENSOR_PIN);
  autoTune();

  unsigned long currentMillis = millis();
  Serial.print("frequency ");
  Serial.println(frequency);
  Serial.print("Time to beat ");
  Serial.println(frequency*interval);
  Serial.print("Last Called ");
  Serial.println(currentMillis - previousMillis);

  if(currentMillis - previousMillis > (frequency/interval)) {
    previousMillis = currentMillis;
    
    for (int i=0; i<NUM_LEDS; i++) {
          leds[i].setRGB( barometricToRGB, 68, 255/barometricToRGB);
          FastLED.show();
        }
  }
}

void autoTune() {
  if (airPressure < low) {
    low = airPressure;
  }
  if (airPressure > high) {
    high = airPressure;
  }
  barometricToRGB = map(airPressure, low, high, 1, 255);
  barometricToRGB = constrain(barometricToRGB, 1, 255);
  frequency = map(airPressure, low, high, 1, 100);
  frequency = constrain(frequency, 1, 100);

}



