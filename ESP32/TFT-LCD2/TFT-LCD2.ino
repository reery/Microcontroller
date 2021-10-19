#include <TFT_eSPI.h>
#include <WiFi.h>
#include <SPI.h>

#define TFT_GREY 0x5AEB
#define LED 5
TFT_eSPI tft = TFT_eSPI();

const char* ssid = "SWAG-NET 2.4";
const char* password = "rdmNbz.11kYOLO";
int timer = 0;
int networks = 0;
int rssiArray[126];
int rssiArrCounter = 0;
uint32_t pixelColor = 0;

void setup() {
 pinMode(LED, OUTPUT);
 digitalWrite(LED, HIGH);
 //Serial.begin(9600);

 tft.init();
 tft.setRotation(0);
 tft.fillScreen(TFT_BLACK);
 tft.setCursor(0, 0, 2);
 tft.setTextColor(TFT_WHITE,TFT_BLACK);  tft.setTextSize(1);
 tft.print("Connecting");
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
  tft.print(".");
  delay(200);
  timer ++;

  if (timer >= 20) {
    blinkingLed(LED, 8);
    digitalWrite(LED, HIGH);
    delay(500);
  }
  
 }
 
 if (WiFi.status() == WL_CONNECTED) {
   digitalWrite(LED, LOW);
   tft.println("");
   tft.println("Connected");
   delay(1000);
    tft.fillScreen(TFT_BLACK);

 }

}

void loop() {
  tft.setCursor(0, 0, 2);
  tft.setTextColor(TFT_WHITE,TFT_BLACK);  tft.setTextSize(1);
  tft.println("IP address:");
  tft.print("    ");
  tft.println(WiFi.localIP());
  tft.print("RSSI: ");
  tft.println(storeRssi());
//  tft.print("Networks: ");
//  tft.println(WiFi.scanNetworks());
  printRssiGraph();
  yield();
  delay(1000);

}

void printRssiGraph() {
  tft.fillRect(0, 50, 128, 110, TFT_ORANGE);
  tft.drawPixel(127, 50, TFT_RED);
  
  for(int i = 0; i < 126; i++) {
    if(rssiArray[i] > -66) {
      pixelColor = 0x07E0;
    } else if(rssiArray[i] < -80) {
      pixelColor = 0xFDA0;
    } else {
      pixelColor = 0xF800;
    }
    
    if(rssiArray[i] == 0){
      continue;
    }
    int mappedValue = map(rssiArray[i], -90, -30, 160, 80);
    tft.drawPixel(i, mappedValue, pixelColor);
    
    int pixelDiff = rssiArray[i] - rssiArray[i - 1];
    tft.setCursor(0, 50);
    tft.print(rssiArray[i]);
    tft.print(" - ");
    tft.print(rssiArray[i - 1]);
    tft.print(" = ");
    tft.print(pixelDiff);

    if(pixelDiff > 0) {
      for(int x = 0; x <= pixelDiff; x++) {
        tft.drawPixel(i, (map(rssiArray[i], -90, -30, 160, 80)), pixelColor);
      }
    }
    
  }

  
  
}

int storeRssi (void) {
  if (rssiArrCounter > 125) {
    rssiArrCounter = 125;
    shiftArray(rssiArray, 126);
  }
  rssiArray[rssiArrCounter] = WiFi.RSSI();
  rssiArrCounter++;

  return rssiArray[rssiArrCounter - 1];
}


void shiftArray(int * Array, const int arraySize){
  for(int i = 0; i < arraySize; i++){
    if(i < (arraySize - 1)){
      Array[i] = Array[i + 1];
    } else {
      Array[arraySize - 1] = 0;
    }
  }
}

void blinkingLed(int led_pin, int repetitions) {
  int counter = 0;
  for(counter = 0; counter < repetitions; counter ++) {
    digitalWrite(led_pin, HIGH);
    delay(200);
    digitalWrite(led_pin, LOW);
    delay(200);
  }
  
}
