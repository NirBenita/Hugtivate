const int ledPin      = 9;
const int sensorPin   = A0;
boolean wasHugged     = false;

int airPressure, high = 0, low = 600;

void setup() {
  Serial.begin(115200);
  pinMode(sensorPin,INPUT);
  pinMode(ledPin,OUTPUT);
}

void loop() {
  airPressure = analogRead(sensorPin);
  // Serial.println(airPressure);
  autoTune();
  analogWrite(ledPin, airPressure);
  // delay(50);
}

void autoTune() {
  if (airPressure < low) {
    low = airPressure;
  }
  
  if (airPressure > high) {
    high = airPressure;
  }

  airPressure = map(airPressure, low+170, high-30, 0, 255);
  airPressure = constrain(airPressure, 0, 255);
}
