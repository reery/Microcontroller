#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 4

Adafruit_SSD1306 display(OLED_RESET);

uint8_t ballX = 2;
uint8_t ballY = 2;
uint8_t wall = 0;
uint8_t dir = 0;

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.ssd1306_command(SSD1306_SETCONTRAST);
  display.ssd1306_command(0);
  //display.clearDisplay();
  pinMode(8, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  display.setTextSize(2);
  display.setTextColor(WHITE);
display.clearDisplay();
  display.setCursor(20,20);
  display.println("YOLO");
}

void loop() {
  /*display.clearDisplay();
  display.setCursor(20,20);
  display.println("YOLO");
  display.drawPixel(ballX, ballY, WHITE);

  if (wall == 0) {
    dir = 0;
    ballX++;
    ballY++;
    }

    if (wall == 2) {
      dir = 1;
      ballX--;
      ballY++;
    }

    if (wall == 3 && dir == 0) {
      ballX++;
      ballY++;
    }

    if (wall == 3 && dir == 1) {
      ballX--;
      ballY++;
    }

    if (wall == 1 && dir == 0) {
      ballX++;
      ballY--;
    }

    if (wall == 1 && dir == 1) {
      ballX--;
      ballY--;
    }
    
    if (ballY == 64) {
      wall = 1;
    }

    if (ballY == 0) {
      wall = 3;
    }

    if (ballX == 128) {
      wall = 2;
    }

    if (ballX == 0) {
      wall = 0;
    }*/

}
