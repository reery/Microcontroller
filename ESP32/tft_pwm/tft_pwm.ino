#include <TFT_eSPI.h>
#include <SPI.h>
//#include <Arduino.h>
//#include <analogWrite.h>

#define RX2 16
#define TX2 17
#define TFT_BACKLIGHT_PIN 34

TFT_eSPI tft = TFT_eSPI();

void setup() {
  tft.init();
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(2, 0, 4);
  tft.print("TFT backlight test");
  Serial.begin(9600);

  ledcAttachPin(34, 0);
  ledcSetup(0, 1000, 8);

tft.fillScreen(TFT_BLACK);
}

void loop() {
  for(int i = 0; i <= 255; i++) {
    ledcWrite(0, i);
    tft.setCursor(2, 0, 4);
    tft.print("ledcWrite ");
    tft.print(i);
    delay(20);
  }
  delay(1000);
  tft.fillScreen(TFT_BLACK);

  for(int i = 255; i >= 0; i--) {
    ledcWrite(0, i);
    tft.setCursor(2, 0, 4);
    tft.print("ledcWrite ");
    tft.print(i);
    delay(20);

    if(i == 99) {
      tft.fillScreen(TFT_BLACK);
    }
    
  }

  delay(1000);

  tft.fillScreen(TFT_BLACK);

}
