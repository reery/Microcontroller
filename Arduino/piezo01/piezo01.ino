const int sensorPin = A0;
int sensorLow = 1023;
int sensorHigh = 0;
int sensorValue;

void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  
  while(millis() <5000) {
    sensorValue = analogRead(sensorPin);
    if(sensorValue < sensorLow) {
      sensorLow = sensorValue;
    }
    if(sensorValue > sensorHigh) {
      sensorHigh = sensorValue;
    }
  }
  Serial.begin(9600);
  Serial.print("SensorLow: ");
  Serial.print(sensorLow);
  Serial.print(" SensorHigh: ");
  Serial.println(sensorHigh);
  digitalWrite(13, LOW);
}

void loop() {
  sensorValue = analogRead(sensorPin);
  int pitch = map(sensorValue, sensorLow, sensorHigh, 440, 1200);
  if(sensorValue <= sensorHigh - 50) {
      tone(8,pitch,20);
      delay(10);
  }
  delay(10);
}
