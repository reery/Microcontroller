#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <WiFi.h>
#include "time.h"
#include <stdio.h>
#include <string.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CLK_PIN   18
#define DATA_PIN  23
#define CS_PIN    5

const char* ssid = "SWAG-NET 2.4";
const char* password = "rdmNbz.11kYOLO";

const char * ntpServer = "129.70.132.34";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;

int hours;
char ch_hours[7];

void printLocalTime(void){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time.");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");

  if(timeinfo.tm_min < 10) {
    sprintf(ch_hours, "%d:0%d", timeinfo.tm_hour, timeinfo.tm_min);
  } else {
  sprintf(ch_hours, "%d:%d", timeinfo.tm_hour, timeinfo.tm_min);
  }
}

MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

void setup(void)
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  P.begin();
  Serial.begin(115200);
  P.setIntensity(0);
  
  Serial.println("Connecting to WiFi");
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    P.displayText("Conn.", PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
    P.displayAnimate();
  }

  P.displayText("Success", PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
  P.displayAnimate();
  Serial.println("Connected.");
  delay(1000);

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

void loop(void)
{
  P.displayText(ch_hours, PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
  P.displayAnimate();
  printLocalTime();
  
  yield();
  delay(1000);  
}
