#include <TFT_eSPI.h>
#include <SPI.h>
#include <MFRC522.h>

TFT_eSPI tft = TFT_eSPI();

// SDA, RST
//MFRC522 rfid = MFRC522(34, 26);

void setup() {
  // SCK, MISO, MOSI
  //SPI.begin(33, 12, 13);
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  //rfid.PCD_Init();
}

void loop() {
  /*MFRC522::MIFARE_Key key;
    //some variables we need
  byte block;
  byte len;
  MFRC522::StatusCode status; */
  
  tft.setTextSize(1);
  tft.drawString("Scanning Cards...", 0, 0, 1);
  tft.drawPixel(15, 15, TFT_GREEN);
  delay(3000);
  tft.writecommand(0x10);
  delay(3000);
  tft.writecommand(0x11);
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
 /* if ( ! rfid.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! rfid.PICC_ReadCardSerial()) {
    return;
  }
  
  tft.drawString("**Card Detected:**", 0, 18, 1);

  //mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid)); //dump some details about the card */
}
