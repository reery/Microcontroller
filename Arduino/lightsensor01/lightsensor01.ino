const int redPin = 11;
const int bluePin = 10;
const int greenPin = 9;

const int redSensor = A0;
const int blueSensor = A1;
const int greenSensor = A2;

int redValue = 0;
int blueValue = 0;
int greenValue = 0;

int redSensorValue = 0;
int blueSensorValue = 0;
int greenSensorValue = 0;

void setup() {
  Serial.begin(9600);
  pinMode(redPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(greenPin, OUTPUT);
}

void loop() {
  redSensorValue = analogRead(redSensor);
  delay(5);
  blueSensorValue = analogRead(blueSensor);
  delay(5);
  greenSensorValue = analogRead(greenSensor);
  delay(5);
  Serial.print("RED: ");
  Serial.print(redSensorValue);
  Serial.print("  \tBLUE: ");
  Serial.print(blueSensorValue);
  Serial.print("   \tGREEN: ");
  Serial.println(greenSensorValue);

  redValue = redSensorValue;
  blueValue = blueSensorValue;
  greenValue = greenSensorValue;

  analogWrite(redPin, redValue);
  analogWrite(bluePin, blueValue);
  analogWrite(greenPin, greenValue);
}
