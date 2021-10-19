#include <Servo.h>

Servo myServo;
int const potPin = A0;
int potVal;
int angle;

void setup() {
  myServo.attach(3);

  Serial.begin(9600);
}

void loop() {
  potVal = analogRead(potPin);
  angle = map(potVal, 0, 1023, 0, 179);
    myServo.write(angle);
  Serial.print("Servo Angle: ");
  Serial.print(myServo.read());
  Serial.print(" Pot Value: ");
  Serial.println(potVal);
  delay(15);
}
