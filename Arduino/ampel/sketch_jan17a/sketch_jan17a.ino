int red = 0;
int green = 0;
int blue = 0;
bool red_no_change = 0;
bool green_no_change = 0;
bool blue_no_change = 0;

void setup() {
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Serial.begin(9600);
}

void loop() {
  colorChange(255, 1000);
  delay(2000);
}


// Assumption green = 0, green_new = 255, duration = 1000
void colorChange(uint8_t green_new, long duration) {
  // Calculate steps
  float green_steps = 0.0;
  
  
  if(green_new - green == 0) {
    green_steps = 0;
    green_no_change = 1;
  } else {
    green_steps = (float(green_new) - float(green)) / float(duration); // green_steps = 0,255 | Avoid division by zero!
  }
  
  float green_counter = 0.0;
  unsigned long timer = 0;
  
  for(timer = 1; timer <= duration; timer++) {

    if(green_no_change == 1) {
    } else {
      green_counter += green_steps;
      green = green_counter;
    }


    Serial.print("T: ");
    Serial.print(timer);
    Serial.print("    G: ");
    Serial.print(green);
    Serial.print("    green_counter: ");
    Serial.print(green_counter);
//    Serial.print("    green_counter(long): ");
//    Serial.print(long(green_counter));
    Serial.print("\n");
    
//    if(timer == long(green_counter)) {
//      green == green_counter;
//      green_counter += green_steps;
//    } else {
//      green_counter += green_steps;
//    }

    farbe_green(green);
    delay(10);
  }

  
}

void farbe_green(uint8_t green) {
  analogWrite(10, green);
}

void farbe(uint8_t red, uint8_t green, uint8_t blue) {
  analogWrite(9, blue); //Blau
  analogWrite(10, green); //Grün
  analogWrite(11, red); //Rot
}
