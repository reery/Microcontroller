int red = 0;
int green = 0;
int blue = 0;

void setup() {
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Serial.begin(9600);
}

void loop() { 
  farbe_start(0,10,10);
  /*wechsel2(0,38,19,1000);
  wechsel2(0,4,78,1000);
  wechsel2(0,38,19,1000);
  wechsel2(0,10,10,1000);*/
  farbe_start(0,0,0);
  wechsel2(0,255,0,1000);
  wechsel2(0,255,50,1000);
  wechsel2(255,0,0,1000);
  wechsel2(255,50,0,1000);
  wechsel2(0,0,255,1000);
  wechsel2(255,255,255,1000);
  wechsel2(0,0,0,1000);

  //farbe(0,0,0);
  //delay(500);
  
  /*wechsel(0,10,10,0,38,19,1000);
  wechsel(0,38,19,0,4,78,1000);
  wechsel(0,4,78,0,38,19,1000);
  wechsel(0,38,19,0,10,10,1000);*/

}

void farbe_start(int red_new, int green_new, int blue_new) {
  red = red_new;
  green = green_new;
  blue = blue_new;
}

void farbe(int red, int green, int blue) {
  analogWrite(9, blue); //Blau
  analogWrite(10, green); //Grün
  analogWrite(11, red); //Rot
}

void wechsel2(int red_new, int green_new, int blue_new, long duration) {
  int timer = 0;
  float red_increment = 0;
  float green_increment = 0;
  float blue_increment = 0;
  if(red != 0 || (red_new - red) != 0) {
    red_increment = 1/((float(red_new)-float(red))/float(duration));
    if(red_new >= 251 && red < 250){
      red_increment = 4.0; 
    }
  }
  if(green != 0 || (green_new - green) != 0) {
    green_increment = 1/((float(green_new)-float(green))/float(duration));
    if(green_new >= 251 && green < 250){
      green_increment = 4.0;
    }
  }
  if(blue != 0 || (blue_new - blue) != 0) {
    blue_increment = 1/((float(blue_new)-float(blue))/float(duration));
    if(blue_new >= 251 && blue < 250){
      blue_increment = 4.0; 
    }
  }
  Serial.print("\nred_increment = ");
  Serial.print(red_increment);
  Serial.print("\ngreen_increment = ");
  Serial.print(green_increment);
  Serial.print("\nblue_increment = ");
  Serial.print(blue_increment);
  Serial.print("\nT: ");
  Serial.print(timer);
  Serial.print("     R: ");
  Serial.print(red);
  Serial.print("  G: ");
  Serial.print(green);
  Serial.print("  B: ");
  Serial.print(blue);
  Serial.print("\n");
  for(timer = 1; timer <= duration ;timer++) {
    /*Serial.print("T: ");
    Serial.print(timer);
    Serial.print("     R: ");
    Serial.print(red);
    Serial.print("  G: ");
    Serial.print(green);
    Serial.print("  B: ");
    Serial.print(blue);
    Serial.print("\n");*/
    if(timer != 1 && timer % int(red_increment) == 0) {
      if(red_increment == 0){
        red = 0;
      } else if (red_increment > 0 && !(red > 250) && !(red < 0)) {
          red++;
          Serial.print("\nT: ");
          Serial.print(timer);
          Serial.print("     R: ");
          Serial.print(red);
      } else if (red_increment < 0 && !(red > 250) && !(red < 0)) {
          red--;
          Serial.print("\nT: ");
          Serial.print(timer);
          Serial.print("     R: ");
          Serial.print(red);
      }  
    }

    if(timer != 1 && timer % int(green_increment) == 0){
      if(green_increment == 0){
        green = 0;
      } else if (green_increment > 0 && !(green > 250) && !(green < 0)) {
          green++;
          Serial.print("\nT: ");
          Serial.print(timer);
          Serial.print("     G: ");
          Serial.print(green);
      } else if (green_increment < 0 && !(green > 250) && !(green < 0)) {
          green--;
          Serial.print("\nT: ");
          Serial.print(timer);
          Serial.print("     G: ");
          Serial.print(green);
      }  
    }
    
    if(timer != 1 && timer % int(blue_increment) == 0){
      if(blue_increment == 0){
        blue = 0;
      } else if (blue_increment > 0 && !(blue > 250) && !(blue < 0)) {
          blue++;
          Serial.print("\nT: ");
          Serial.print(timer);
          Serial.print("     B: ");
          Serial.print(blue);
      } else if (blue_increment < 0 && !(blue > 250) && !(blue < 0)) {
          blue--;
          Serial.print("\nT: ");
          Serial.print(timer);
          Serial.print("     B: ");
          Serial.print(blue);
      }  
    }
    
    farbe(red, green, blue);
    delay(5);
  }
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
