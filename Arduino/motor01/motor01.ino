const int motorPin = 7;
const int switchPin = 2;
int switchState = 0;

void setup() {
  pinMode(7, OUTPUT);
  pinMode(2, INPUT);
 // Serial.begin(9600);
}

void loop() {
  switchState = digitalRead(switchPin);
  if(switchState == 1){
/*    Serial.print("Switch State: ");
    Serial.println(switchState); */
    digitalWrite(motorPin, HIGH);
  } else {
    digitalWrite(motorPin, LOW);
   /* Serial.print("Switch State: ");
    Serial.println(switchState);*/
  }
}
