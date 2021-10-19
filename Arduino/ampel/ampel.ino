int buttonPressed = 0;
boolean ampelRot = true;
boolean ampelGruen = false;
boolean ampelRotPerm = true;
boolean ampelGruenPerm = false;

void setup() {
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(2, INPUT);
}

void loop() {
  buttonPressed = digitalRead(2);

  if (buttonPressed == HIGH) {
    delay(200);
    ampelRot = ! (ampelRot);
    ampelGruen = ! (ampelGruen);
  }
  
  if (ampelRotPerm == true) {
    digitalWrite(5, HIGH);
  } 

  if (ampelGruenPerm == true) {
    digitalWrite(3, HIGH);
  }

  if (buttonPressed == HIGH && ampelGruen == true) {
    delay(1000);
    digitalWrite(5, LOW);
    delay(200);
    digitalWrite(4, HIGH);
    delay(800);
    digitalWrite(4, LOW);
    delay(200);
    digitalWrite(3, HIGH);
    ampelRotPerm = false;
    ampelGruenPerm = true;
  }

  if (buttonPressed == HIGH && ampelRot == true) {
    delay(1000);
    digitalWrite(3, LOW);
    delay(200);
    digitalWrite(4, HIGH);
    delay(800);
    digitalWrite(4, LOW);
    delay(200);
    digitalWrite(5, HIGH);
    ampelRotPerm = true;
    ampelGruenPerm = false;
  }
}
