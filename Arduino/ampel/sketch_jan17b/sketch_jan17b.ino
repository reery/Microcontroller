int red = 0;
int green = 0;
int blue = 0;
bool red_no_change = 0;
bool green_no_change = 0;
bool blue_no_change = 0;
float red_counter = 0.0;
float green_counter = 0.0;
float blue_counter = 0.0;

void setup() {
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
//  Serial.begin(9600);
}

void loop() {
  colorChange(200,200,200,1000);
  colorChange(200,200,0,1000);
  colorChange(200,0,0,1000);
  colorChange(0,200,0,1000);
  colorChange(0,200,200,1000);
  colorChange(0,0,200,1000);
//  colorChange(0,0,0,1000);
  delay(1000);
}


// Assumption green = 0, green_new = 255, duration = 1000
// Changes RGB LED color within the given time duration in ms.
void colorChange(uint8_t red_new,uint8_t green_new, uint8_t blue_new, long duration) {
  // Resetting incremental color steps
  float red_steps = 0.0;
  float green_steps = 0.0;
  float blue_steps = 0.0;

  // Calculate incremental color steps
  if(red_new - red == 0) { // Division by zero check
    red_steps = 0; // If there is no change from previous color, then the incremental step is zero
    red_no_change = 1; // Set flag to true that the color has not been changed
  } else {
    red_no_change = 0; // If there is a color change, then set flag back to false
    red_steps = (float(red_new) - float(red)) / float(duration); // (new color - existing color) divided by given duration, gives the incremental steps
  }

  if(green_new - green == 0) {
    green_steps = 0;
    green_no_change = 1;
  } else {
    green_no_change = 0;
    green_steps = (float(green_new) - float(green)) / float(duration); // green_steps = -0,255
  }

  if(blue_new - blue == 0) {
    blue_steps = 0;
    blue_no_change = 1;
  } else {
    blue_no_change = 0;
    blue_steps = (float(blue_new) - float(blue)) / float(duration);
  }

  // Timer needed for the color change loop
  unsigned long timer = 0;

  // Iterates through the given duration and adds the calculated steps to the int number
  for(timer = 1; timer <= duration; timer++) {

    if(red_no_change == 1) { // When there is no color change (flag is true), then just do nothing with this color
    } else { // If there is a change, then...
      red_counter += red_steps; // Add the float incremental to the float color counter, works with negative numbers as well. Needed because adding float to float
      red = int(red_counter); // Set the color according to the color counter, the float counter will get converted to int 
    }
    
    if(green_no_change == 1) {
    } else { 
      green_counter += green_steps;
      green = int(green_counter);
    } 

    if(blue_no_change == 1) {
    } else {
      blue_counter += blue_steps;
      blue = int(blue_counter);
    } 
    
//    Serial.print("counter: ");
//    Serial.print(counter);
//    Serial.print("    T: ");
//    Serial.print(timer);
//    Serial.print("    R: ");
//    Serial.print(red_counter);
//    Serial.print("    G: ");
//    Serial.print(green_counter);
//    Serial.print("    B: ");
//    Serial.print(blue_counter);
//    Serial.print("\n");

    rgb_color(red,green,blue); // Set the calculated color
    delay(1);
  }

  // Set final color as definite to avoid float rounding problems
  red = red_new;
  green = green_new;
  blue = blue_new;
  
}

void rgb_color(uint8_t red, uint8_t green, uint8_t blue) {
  analogWrite(9, blue); //Blau
  analogWrite(10, green); //Grün
  analogWrite(11, red); //Rot
}
