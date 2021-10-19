const int sensorPin = A0;
const float basetemp = 20.0;
// without const basetemp 4,358 bytes flash and 234 bytes ram

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  for(int pinNumber = 3; pinNumber<6; pinNumber++){
    pinMode(pinNumber, OUTPUT);
    digitalWrite(pinNumber, LOW);
  }
}

void loop() {
 digitalWrite(LED_BUILTIN, LOW);
 
 int sensorValue = analogRead(sensorPin);
 Serial.print("Sensor Value: ");
 Serial.print(sensorValue); 

 float sensorVolts = sensorValue/1024.0 * 5;
 Serial.print(", Volts: ");
 Serial.print(sensorVolts);

 float tempValue1 = (sensorVolts - 0.5) * 100;
 delay(50);
 float tempValue2 = (sensorVolts - 0.5) * 100;
 delay(50);
 float temp = (tempValue1 + tempValue2) / 2;
 Serial.print(", Temp: ");
 Serial.print(temp);
 Serial.println("°C");

 if (temp < basetemp) {
  delay(1200);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  delay(500);
  digitalWrite(3, HIGH);
  delay(300);
 } else if(temp >= basetemp && temp < basetemp +2) {
  delay(2000);
  digitalWrite(3, HIGH);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
 } else if(temp >= basetemp +2 && temp < basetemp +4) {
  delay(2000);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, LOW);
 } else if(temp >= basetemp +4 && temp < basetemp +6) {
  delay(2000);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
 } else if(temp >= basetemp +6) {
  delay(1200);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, LOW);
  delay(500);
  digitalWrite(5, HIGH);
  delay(300);
 }
}
