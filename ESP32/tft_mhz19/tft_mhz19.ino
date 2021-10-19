#include <TFT_eSPI.h>
#include <SPI.h>

#define RX2 16
#define TX2 17
#define TFT_BACKLIGHT_PIN 34

TFT_eSPI tft = TFT_eSPI();
unsigned long getDataTimer = 0;
unsigned long getDataTimer1 = 0;
byte read_co2[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
byte read_abc[9] = {0xFF, 0x01, 0x7D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x82};
byte abc_on[9] = {0xFF, 0x01, 0x79, 0xA0, 0x00, 0x00, 0x00, 0x00, 0xE7};
byte abc_off[9] = {0xFF, 0x01, 0x79, 0x00, 0x00, 0x00, 0x00, 0x00, 0x87};
byte sensor[9];
byte abc_response[9];
uint8_t bytesToRead = 0;
uint8_t bytePos = 0;
int ppm = 0;

void setup() {
  tft.init();
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(2, 0, 4);
  tft.print("Reading sensor...");
  Serial2.begin(9600, SERIAL_8N1, RX2, TX2);
  Serial.begin(9600);
  ledcAttachPin(15, 0);
  ledcSetup(0, 1000, 8);
  


  Serial2.write(abc_off, 9);
//            Serial.print("bytes to read: ");
//            bytesToRead = Serial2.available();
//            Serial.println(bytesToRead);
//            Serial.print("read: ");
//          while(Serial2.available() != 0) {
//
//          for(bytePos = 0; bytePos < 9; bytePos++) {
//            abc_response[bytePos] = Serial2.read();
//            
//            Serial.print(abc_response[bytePos]);
//            Serial.print(" ");
//          }
//          Serial.println("");
//  
//        }
tft.fillScreen(TFT_BLACK);
ledcWrite(0, 50);
}

void loop() {
  //ledcWrite(0, 50);
    if (millis() - getDataTimer >= 2000) {
        
        Serial2.write(read_co2, 9);
//          Serial.print("available: ");
//          bytesToRead = Serial2.available();
//          Serial.println(bytesToRead);

        while(Serial2.available() != 0) {
//          Serial.print("read: ");
          for(bytePos = 0; bytePos < 9; bytePos++) {
            sensor[bytePos] = Serial2.read();
//            Serial.print("read: ");
//            Serial.print(sensor[bytePos]);
//            Serial.print(" ");
          }
          
//          Serial.print("\navailable after read: ");
//          bytesToRead = Serial2.available();
//          Serial.println(bytesToRead);
          
          ppm = (int)sensor[2] * 256 + (int)sensor[3];
          bytesToRead = Serial2.available();
          
          //tft.fillScreen(TFT_BLACK);
          tft.setCursor(2, 0, 4);
          tft.print("Temp: ");
          tft.println((int)sensor[4] - 38);
          tft.print("Co2: ");
          tft.println(ppm);
//          tft.print("ABC Tick: ");
//          tft.println(sensor[6]);
//          tft.print("Available: ");
//          tft.println(bytesToRead);
//          tft.println("data read: ");
//          for(bytePos = 0; bytePos < 9; bytePos++) {
//            tft.print(sensor[bytePos]);
//            tft.print(" ");
          }
  
//          Serial.print("ppm: ");
//          Serial.println(ppm);
//          Serial.print("Temp: ");
//          Serial.println((int)sensor[4] - 38);

        bytePos = 0;
        getDataTimer = millis();
    }
}

//    if (millis() - getDataTimer1 >= 10000) {
//        Serial2.write(read_abc, 9);
//          Serial.print("available to read: ");
//          bytesToRead = Serial2.available();
//          Serial.println(bytesToRead);
//         while(Serial2.available() != 0) {
//            Serial.print("abc data: ");
//            
//            for(bytePos = 0; bytePos < 9; bytePos++) {
//              abc_response[bytePos] = Serial2.read();          
//              Serial.print(abc_response[bytePos]);
//              Serial.print(" ");
//            }
//         }
//         Serial.println("");
//         getDataTimer1 = millis();
//    }
//}

char getChecksum(char * arr) {
  char i, checksum;
  for(i = 1; i < 8; i++) {
    checksum += arr[i];
  }

  return 0xff - checksum;
}
