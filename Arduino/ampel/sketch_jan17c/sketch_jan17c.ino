uint8_t red = 0;
uint8_t green = 0;
uint8_t blue = 0;
bool red_no_change = 0;
bool green_no_change = 0;
bool blue_no_change = 0;
float red_counter = 0.0;
float green_counter = 0.0;
float blue_counter = 0.0;
unsigned long timer = 0;
float red_steps = 0.0;
float green_steps = 0.0;
float blue_steps = 0.0;

void setup() {
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
}

void loop() {
  colorChange(200,200,200,1000);
  colorChange(200,200,0,1000);
  colorChange(200,0,0,1000);
  colorChange(0,200,0,1000);
  colorChange(0,200,200,1000);
  colorChange(0,0,200,1000);
  delay(1000);
}

void colorChange(uint8_t red_new, uint8_t green_new, uint8_t blue_new, unsigned long duration) {
  red_steps = 0.0;
  green_steps = 0.0;
  blue_steps = 0.0;

  if(red_new - red == 0) {
    red_steps = 0;
    red_no_change = 1;
  } else {
    red_no_change = 0;
    red_steps = (float(red_new) - float(red)) / float(duration);
  }

  if(green_new - green == 0) {
    green_steps = 0;
    green_no_change = 1;
  } else {
    green_no_change = 0;
    green_steps = (float(green_new) - float(green)) / float(duration);
  }

  if(blue_new - blue == 0) {
    blue_steps = 0;
    blue_no_change = 1;
  } else {
    blue_no_change = 0;
    blue_steps = (float(blue_new) - float(blue)) / float(duration);
  }

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

    rgb_color(red,green,blue);
    delay(1);
  }

  red = red_new;
  green = green_new;
  blue = blue_new;

}

void rgb_color(uint8_t red, uint8_t green, uint8_t blue) {
  analogWrite(9, blue);
  analogWrite(10, green);
  analogWrite(11, red);
}
