#include <TFT_eSPI.h>
#include <SPI.h>
#include "config.h"
#include "certificates.h"
#include <PromLokiTransport.h>
#include <PrometheusArduino.h>
PromLokiTransport transport;
PromClient client(transport);

// Grafana stuff
WriteRequest req(1, 1024);
// Define a TimeSeries which can hold up to 5 samples, has a name of `heap_free_bytes`
TimeSeries ts1(1, "co2", "{job=\"esp32-co2\",host=\"esp32\"}");

#define RX2 16
#define TX2 17
#define TFT_BACKLIGHT_PIN 15

TFT_eSPI tft = TFT_eSPI();
unsigned long getDataTimer = 0;
unsigned long getDataTimer1 = 0;
byte read_co2[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
byte read_abc[9] = {0xFF, 0x01, 0x7D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x82};
byte abc_on[9] = {0xFF, 0x01, 0x79, 0xA0, 0x00, 0x00, 0x00, 0x00, 0xE7};
byte abc_off[9] = {0xFF, 0x01, 0x79, 0x00, 0x00, 0x00, 0x00, 0x00, 0x87};
byte sensor[9];
byte byteBuffer[63];
byte abc_response[9];
uint8_t bytesToRead = 0;
uint8_t bytePos = 0;
int ppm = 0;
char checksum;

void setup() {
  // Grafana stuff
  //freeMemory();
  transport.setUseTls(true);
  transport.setCerts(grafanaCert, strlen(grafanaCert));
  transport.setWifiSsid(WIFI_SSID);
  transport.setWifiPass(WIFI_PASSWORD);
  transport.begin();
  
  // Configure the client
  client.setUrl(GC_URL);
  client.setPath((char*)GC_PATH);
  client.setPort(GC_PORT);
  client.setUser(GC_USER);
  client.setPass(GC_PASS);
  client.begin();
  
  // Add our TimeSeries to the WriteRequest
  req.addTimeSeries(ts1);
  
  
  Serial.begin(9600);
  while(!Serial);
  delay(200);
  
  tft.init();
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(2, 0, 4);
  tft.print("Reading sensor...");

  Serial.print("Connecting to MH-Z19...");
  Serial2.begin(9600, SERIAL_8N1, RX2, TX2);
  delay(200);
  Serial.println("\nConnected to MH-Z19");
  
  ledcAttachPin(TFT_BACKLIGHT_PIN, 0);
  ledcSetup(0, 1000, 8);

  Serial2.write(abc_off, 9);
  Serial.println("Written abc_off to MH-Z19");
  
  Serial.print("Bytes to read from MH-Z19: ");
  bytesToRead = Serial2.available();
  Serial.println(bytesToRead);
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
ledcWrite(0, 100);
}

void loop() {
  //ledcWrite(0, 50);
    int64_t time;
    time = transport.getTimeMillis();
    
    if (millis() - getDataTimer >= 5000) {
        
        Serial2.write(read_co2, 9); 
        Serial.print("Requested data...");

        bytesToRead = Serial2.available();
        Serial.print("There are ");
        Serial.print(bytesToRead);
        Serial.println(" bytes to read");

        if(bytesToRead > 9) {
          Serial.print(bytesToRead);
          Serial.println(" bytes are here, but it should be not more than 9");
          Serial.print("These are the bad bytes: ");

          for(bytePos = 0; bytePos < bytesToRead; bytePos++) {
            byteBuffer[bytePos] = Serial2.read();
            Serial.print(byteBuffer[bytePos]);
            Serial.print(" ");
          }
          Serial.println("");
        }

        bytesToRead = Serial2.available();

        if(bytesToRead > 0) {
          Serial.println("There are bytes to read: ");
          for(bytePos = 0; bytePos < bytesToRead; bytePos++) {
            sensor[bytePos] = Serial2.read();
            Serial.print(sensor[bytePos]);
            Serial.print(" ");
          }
        }
        

//        Serial.print("Bytes to read 2: ");
//        bytesToRead = Serial2.available();
//        Serial.println(bytesToRead);
//
//        while(Serial2.available() != 0) {
//          Serial.println("Reading 9 bytes");
//          for(bytePos = 0; bytePos < 9; bytePos++) {
//            sensor[bytePos] = Serial2.read();
//            Serial.print(sensor[bytePos]);
//            Serial.print(" ");
//          }
          Serial.println("\nFinished reading");

        Serial.print("Now there are ");
        bytesToRead = Serial2.available();
        Serial.print(bytesToRead);
        Serial.println(" bytes to be read. Wating 5 seconds...");
          
          ppm = (int)sensor[2] * 256 + (int)sensor[3];
          
          //tft.fillScreen(TFT_BLACK);
          tft.fillScreen(TFT_BLACK);
          tft.setCursor(2, 0, 4);
          tft.print("Temp: ");
          tft.println((int)sensor[4] - 38);
          tft.print("Co2: ");
          tft.println(ppm);

          ts1.addSample(time, ppm);
          PromClient::SendResult res = client.send(req);
          ts1.resetSamples();
//          tft.print("ABC Tick: ");
//          tft.println(sensor[6]);
//          tft.print("Available: ");
//          tft.println(bytesToRead);
//          tft.println("data read: ");
//          for(bytePos = 0; bytePos < 9; bytePos++) {
//            tft.print(sensor[bytePos]);
//            tft.print(" ");

          Serial.print("Checksum calculated: ");
          Serial.println(getChecksum(sensor));

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

byte getChecksum(byte * arr) {
  char i;
  byte checksum;
  for(i = 1; i < 8; i++) {
    checksum += arr[i];
  }

  return (0xff - checksum) + 0x01;
}
