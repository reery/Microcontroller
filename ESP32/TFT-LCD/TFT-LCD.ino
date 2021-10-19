#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Adafruit_ST7735.h>

#define LED 5
#define TFT_CS 12
#define TFT_RST 27
#define TFT_DC 14
#define TFT_SCLK 25   
#define TFT_MOSI 26   

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

const char* ssid = "SWAG-NET 2.4";
const char* password = "rdmNbz.11kYOLO";
int timer = 0;

void setup() {
 pinMode(LED, OUTPUT);
 digitalWrite(LED, HIGH);
 //Serial.begin(9600);
// Serial.print("Connecting to: ");
// Serial.println(ssid);

 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
  delay(200);
  //Serial.print(".");
  timer ++;

  if (timer >= 20) {
    blinkingLed(LED, 8);
    digitalWrite(LED, HIGH);
    delay(500);
  }

    tft.initR(INITR_BLACKTAB);  
  tft.fillScreen(ST7735_BLACK); 
  
 }
 
 if (WiFi.status() == WL_CONNECTED) {
   //Serial.println("Connected.");
   digitalWrite(LED, LOW);
   
//   Serial.print("IP address: ");
//   Serial.println(WiFi.localIP());
 }
 
}

void loop() {
  delay(1000);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(0);
  tft.setCursor(0,10);
  tft.print("IP address: ");
  tft.println(WiFi.localIP());
 // tft.setCursor(5,40);
  tft.print("RSSI: ");
  tft.println(WiFi.RSSI());

  delay(500);
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
