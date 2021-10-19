#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET LED_BUILTIN
Adafruit_SSD1306 display(OLED_RESET);
int i = 0;

void setup()   {   
  Wire.setClock(800000L);           

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  // Clear the buffer.
  display.clearDisplay();

  // draw a single pixel
  display.drawPixel(10, 10, WHITE);
  display.setTextSize(1);
  display.setTextColor(WHITE);
}


void loop() {
  for(i = 0; i < 10000; i++) {
    display.clearDisplay();
    display.setCursor(10,10);
    display.print("Test ESP 8266 Ada");
    display.setCursor(15, 20);
    display.print(i);
    display.display();
  }
}

