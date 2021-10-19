#include <TFT_eSPI.h>
#include <SPI.h>

#define RX2 16
#define TX2 17

TFT_eSPI tft = TFT_eSPI();
unsigned long getDataTimer = 0;
unsigned long getDataTimer1 = 0;
byte read_co2[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
byte read_abc[9] = {0xFF, 0x01, 0x7D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
byte abc_on[9] = {0xFF, 0x01, 0x79, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x79};
byte abc_off[9] = {0xFF, 0x01, 0x79, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
byte sensor[9];
byte abc_response[9];
uint8_t bytesToRead = 0;
uint8_t bytePos = 0;
int ppm = 0;

void setup() {
  tft.init();
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(2, 0, 4);
  Serial2.begin(9600, SERIAL_8N1, RX2, TX2);
  Serial.begin(9600);

  Serial2.write(abc_off, 9);
  bytesToRead = Serial2.available();
  tft.print("Available: ");
  tft.print(bytesToRead);

}

void loop() {
  
  if (millis() - getDataTimer >= 2000) {
    Serial2.write(read_abc, 9);
    bytesToRead = Serial2.available();
    tft.fillScreen(TFT_BLACK);
tft.setCursor(2, 0, 4);
tft.print("Available: ");
  tft.print(bytesToRead);
  getDataTimer = millis();
  }
  


}
