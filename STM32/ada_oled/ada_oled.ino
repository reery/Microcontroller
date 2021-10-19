#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif


void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.ssd1306_command(SSD1306_SETCONTRAST);
  display.ssd1306_command(0);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  pinMode(8, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
 
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0,0);
    display.println("Temp: ");
    display.setCursor(60,0);
}
