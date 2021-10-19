#include <Arduino.h>
//#include "MHZ19.h"                                        
#include <SoftwareSerial.h>
#include <Wire.h>
#include <U8g2lib.h>

#define RX_PIN 10
#define TX_PIN 11

SoftwareSerial mh(RX_PIN, TX_PIN);
U8G2_SSD1306_128X64_NONAME_2_HW_I2C display(U8G2_R0,/* reset=*/ U8X8_PIN_NONE);

unsigned long getDataTimer = 0;
byte command[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
byte sensor[9];
uint8_t bytesToRead = 0;
uint8_t bytePos = 0;
int ppm = 0;

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
//    Serial.begin(9600);
    display.begin();
    display.setContrast(0);
    mh.begin(9600);
}

void loop()
{
    if (millis() - getDataTimer >= 2000)
    {
        
        mh.write(command, 9);
//          Serial.print("available: ");
          bytesToRead = mh.available();
//          Serial.println(bytesToRead);

        while(mh.available() != 0) {
//          Serial.print("read: ");
          for(bytePos = 0; bytePos < 9; bytePos++) {
            sensor[bytePos] = mh.read();
//            Serial.print("read: ");
//            Serial.print(sensor[bytePos]);
//            Serial.print(" ");
          }
          
//          Serial.print("\navailable after read: ");
//          bytesToRead = mh.available();
//          Serial.println(bytesToRead);
          
          ppm = (int)sensor[2] * 256 + (int)sensor[3];
  
//          Serial.print("ppm: ");
//          Serial.println(ppm);
//          Serial.print("Temp: ");
//          Serial.println((int)sensor[4] - 38);
        }

        display.firstPage();
        do {
          display.setFont(u8g2_font_VCR_OSD_mr);
          display.drawStr(0, 15, "Temp ");
          display.setCursor(55, 15);
          display.print((int)sensor[4] - 38);
          display.drawStr(0, 40, "Co2  ");
          display.setCursor(55, 40);
          display.print(ppm);
        } while (display.nextPage());


        bytePos = 0;
        getDataTimer = millis();
    }
}
