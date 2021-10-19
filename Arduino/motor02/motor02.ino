const int motorPin = 6;
const int potPin = A0;
int switchState = 0;
int motorState = 0;

void setup() {
  pinMode(motorPin, OUTPUT);
  pinMode(potPin, INPUT);
}

void loop() {
  switchState = analogRead(potPin);

  motorState = map(switchState, 0, 1023, 0, 254);

  analogWrite(motorPin, motorState);

}
