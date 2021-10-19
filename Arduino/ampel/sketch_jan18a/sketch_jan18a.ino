int red = 0;
int green = 0;
int blue = 0;
bool red_no_change = 0;
bool green_no_change = 0;
bool blue_no_change = 0;
float red_counter = 0.0;
float green_counter = 0.0;
float blue_counter = 0.0;
float red_steps = 0.0;
float green_steps = 0.0;
float blue_steps = 0.0;
unsigned long timer = 0;

void setup() {
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
}

void loop() {
  colorChange(40,0,0,1000);
  delay(1000);
  colorChange(20,20,0,1000);
  delay(1000);
  colorChange(13,13,13,1000);
  delay(1000);
  colorChange(0,20,20,1000);
  delay(1000);
  colorChange(0,0,40,1000);
  delay(1000);
  colorChange(0,0,0,1000);
  delay(1000);
}

void colorChange(uint8_t red_new,uint8_t green_new, uint8_t blue_new, unsigned long duration) {

  // red_steps in function call could be removed?
  red_steps = calculateSteps(red_new, red, red_no_change, red_steps, duration);
  green_steps = calculateSteps(green_new, green, green_no_change, green_steps, duration);
  blue_steps = calculateSteps(blue_new, blue, blue_no_change, blue_steps, duration);

  for(timer = 1; timer <= duration; timer++) {

    if(red_no_change == 1) {
    } else {
      red_counter += red_steps;
      red = int(red_counter);
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

    setColor(red,green,blue);
    delay(1);
  }

  red = red_new;
  green = green_new;
  blue = blue_new;

}

float calculateSteps(uint8_t color_new, uint8_t color, bool color_no_change, float color_steps, unsigned long duration) {

    if(color_new - color == 0) {
      color_steps = 0;
      color_no_change = 1;
    } else {
      color_no_change = 0;
      color_steps = (float(color_new) - float(color)) / float(duration);
    }

    return color_steps;

}

void setColor(uint8_t red, uint8_t green, uint8_t blue) {
  analogWrite(9, blue);
  analogWrite(10, green);
  analogWrite(11, red);
}
