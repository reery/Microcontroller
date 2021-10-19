#include <Servo.h>

int angle;
int minute = 60;
Servo myServo;

void setup() {
  myServo.attach(3);

  Serial.begin(9600);
  myServo.write(0);
}

void loop() {
  for (int t = 0; t<29; t++){
    angle = map(t, 0, 29, 0, 179);
    myServo.write(angle);
    Serial.print("Time: ");
    Serial.print(t);
    Serial.print(" Angle: ");
    Serial.println(angle);
    delay(1000);
  }
  for (int t = 29; t>0; t--){
    angle = map(t, 0, 29, 0, 179);
    myServo.write(angle);
    Serial.print("Time: ");
    Serial.print(t);
    Serial.print(" Angle: ");
    Serial.println(angle);
    delay(1000);
  }
}
