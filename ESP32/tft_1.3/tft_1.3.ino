#include <TFT_eSPI.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include <Wire.h>

#define RX_PIN 16
#define TX_PIN 17

TFT_eSPI tft = TFT_eSPI();

void setup() {
  tft.init();
  tft.fillScreen(TFT_GREEN);
  tft.setCursor(2, 0, 2);
    tft.print("This is cool");
}

void loop() {

}
