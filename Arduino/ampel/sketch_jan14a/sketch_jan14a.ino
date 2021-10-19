void setup() {
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
}

void loop() { 
  wechsel(0,10,10,0,38,19,1000);
  wechsel(0,38,19,0,4,78,1000);
  wechsel(0,4,78,0,38,19,1000);
  wechsel(0,38,19,0,10,10,1000);

}

void farbe(int red, int green, int blue) {
  analogWrite(9, blue); //Blau
  analogWrite(10, green); //Grün
  analogWrite(11, red); //Rot
}

void wechsel(int red_start, int green_start, int blue_start, int red_end, int green_end, int blue_end, int duration) {
  int timer = 0;
  float red_increment = 1/((float(red_end)-float(red_start))/float(duration));
  int red = red_start;
  float green_increment = 1/((float(green_end)-float(green_start))/float(duration));
  int green = green_start;
  float blue_increment = 1/((float(blue_end)-float(blue_start))/float(duration));
  int blue = blue_start;
  
  for(timer = 0; timer < duration ;timer++) {
    if(timer % int(red_increment) == 0){
      if(red_increment >= 0){
        red++;
      } else {
        red--;
      }
    }

    if(timer % int(green_increment) == 0){
      if(green_increment >= 0){
        green++;
      } else {
        green--;
      }
    }
    
    if(timer % int(blue_increment) == 0){
      if(blue_increment >= 0){
        blue++;
      } else {
        blue--;
      }
    }
    
    farbe(red, green, blue);
    delay(1);
  }
  
  
}
/*
farbe(0,10,10);
  delay(1000);
  farbe(0,25,10);
  delay(1000);
  farbe(0,38,19);
  delay(1000);
  farbe(0,4,78);
  delay(1000); */
