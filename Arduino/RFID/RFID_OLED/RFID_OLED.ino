#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SS_PIN 10 

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ A5, /* data=*/ A4, /* reset=*/ U8X8_PIN_NONE); 
MFRC522 mfrc522(SS_PIN, RST_PIN);

char uid_str[50];

void setup() {
  u8g2.begin();
  SPI.begin();
  mfrc522.PCD_Init();  
  
}

void loop() {
    // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Dump debug info about the card; PICC_HaltA() is automatically called
  //mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
  mfrc522.PICC_DumpDetailsToScreen(&(mfrc522.uid), &uid_str);

  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_7x14_tn);  // choose a suitable font
  u8g2.drawStr(0,12, uid_str); // write something to the internal memory
  u8g2.sendBuffer();          // transfer internal memory to the display
  delay(30);
}
