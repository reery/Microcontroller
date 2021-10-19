short ledPin = 2;
short interval1 = 1;
boolean button = false;
unsigned long seconds = 0;
unsigned long secondsPrev = 0;
short switchState = 1;

void setup() {
  for(int x = 2; x <8; x++){
     pinMode(x, OUTPUT);
  }
  pinMode(9, INPUT);
}

void loop() {
  seconds = millis() / 1000;

  if(seconds - secondsPrev >= interval1 && button == false){
    secondsPrev = seconds;
    digitalWrite(ledPin, HIGH);
    ledPin++;

    if(ledPin == 8){
    ledPin = 7; 
    button = true;
    }
  }

  if(seconds - secondsPrev >= interval1 && button == true){
  secondsPrev = seconds;
  digitalWrite(ledPin, LOW);
  ledPin--;

  if(ledPin == 1){
    ledPin = 2;
    button = false;
    }
  }
  
  switchState = digitalRead(9);

  if(switchState == 0){
    button = false;
    
    for(ledPin = 2; ledPin <8; ledPin++){
      digitalWrite(ledPin, LOW);
    }
    ledPin = 2;
  }
  
}
