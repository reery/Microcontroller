#include <Adafruit_GFX.h>
#include <Adafruit_SharpMem.h>
#include <WiFi.h>
#include "time.h"
#include <string.h>

#define SHARP_SCK  36
#define SHARP_MOSI 35
#define SHARP_SS   34

#define BLACK 0
#define WHITE 1

Adafruit_SharpMem display(SHARP_SCK, SHARP_MOSI, SHARP_SS, 144, 168);

const char* ssid = "SWAG-NET 2.4";
const char* password = "rdmNbz.11kYOLO";

const char * ntpServer = "129.70.132.34";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;

uint8_t hour = 0;
uint8_t minute = 0;
uint8_t second = 0;
char ch_hours[7];
char ch_day[20];
char ch_seconds[4];

void setup() {
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  display.begin();
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(BLACK);
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    display.setCursor(0, 0);
    display.println("Connecting");
    display.println("to WiFi...");
    Serial.println("\nConnecting to WiFi...");
    display.refresh();
    waitAndBlink(500, 100);
  }
  
  display.println("Connected");
  display.refresh();

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();
  
  display.refresh();
  digitalWrite(2, HIGH);
  delay(1000);

  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  digitalWrite(2, LOW);
  display.println("Disconnecting...");
  display.refresh();
  delay(1000);
  
  randomSeed(analogRead(1));
  hour = random(0, 23);
  minute = random(0, 59);

}

void loop() {
  display.setTextColor(BLACK);
  display.setTextSize(2);
  display.setCursor(20, 30);
  display.clearDisplay();
  display.print(ch_day);
  
  display.setTextSize(4);
  display.setCursor(12, 65);
  display.print(ch_hours);

  display.setTextSize(3);
  display.setCursor(52, 98);
  display.print(ch_seconds);
  
  display.refresh();
  printLocalTime();
  delay(1000);
//  tickingClock();
//  display.setTextSize(4);
//  display.setTextColor(BLACK);
//  display.setCursor(20,65);
//  display.print("19:47");
//  display.println("Hallo");
//  display.println("Schnuffi");
//  display.println("ich hoffe");
//  display.println("dir geht es");
//  display.println("gut");
//  display.refresh();
//  delay(10000);

}

void tickingClock(void) {
  display.setTextSize(4);
  display.setTextColor(BLACK);
  display.setCursor(10, 65);

  for(int i = 0; i < 1440; minute++) {
    display.setCursor(16, 65);
    display.clearDisplay();
    display.print(hour);
    display.print(":");
    display.print(minute);

    if(minute >= 59) {
      minute = 0;
      hour++;
    }

    if(hour > 23) {
      hour = 0;
    }
    display.refresh();
    delay(1000);
  }
}

void waitAndBlink(long wait_time, long delay_ms) {
  bool blink_toggle = 0;
  Serial.println("Entering waitAndBlink loop");
  
  for(int i = 0; i < wait_time; i += delay_ms) {
    delay(delay_ms);
    Serial.println(i);
    if(blink_toggle == 0) {
      blink_toggle = 1;
      digitalWrite(2, HIGH);
    } else {
      blink_toggle = 0;
      digitalWrite(2, LOW);
    }
  }
}


void printLocalTime(void){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time.");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  //sprintf(ch_day, "%d", timeinfo.tm_mday);
  strftime(ch_day, 20, "%A", &timeinfo);

  if(timeinfo.tm_sec < 10) {
    sprintf(ch_seconds, "0%d", timeinfo.tm_sec);
  } else {
    sprintf(ch_seconds, "%d", timeinfo.tm_sec);
  }

  if(timeinfo.tm_min < 10) {
    sprintf(ch_hours, "%d:0%d", timeinfo.tm_hour, timeinfo.tm_min);
  } else {
  sprintf(ch_hours, "%d:%d", timeinfo.tm_hour, timeinfo.tm_min);
  }
}
