#include <WiFi.h>
#define LED 5
const char* ssid = "SWAG-NET 2.4";
const char* password = "rdmNbz.11kYOLO";

void setup() {
 pinMode(LED, OUTPUT);
 digitalWrite(LED, HIGH);
 Serial.begin(9600);
 Serial.print("Connecting to WiFi: ");
 Serial.println(ssid);

 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
  delay(200);
  Serial.print(".");
 }
 Serial.println("Connected.");
 digitalWrite(LED, LOW);
}

void loop() {
 if (WiFi.status() == WL_CONNECTED) {
   delay(1000);
   Serial.println(WiFi.localIP());
   Serial.println(WiFi.status());
   Serial.println(WiFi.RSSI());
   Serial.println("");
 }
}
