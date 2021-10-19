#include <Adafruit_GFX.h>
#include <Adafruit_SharpMem.h>
#include <WiFi.h>
#include "time.h"
#include <string.h>
#include "FS.h"
#include "FFat.h"

#define SHARP_SCK  36
#define SHARP_MOSI 35
#define SHARP_SS   34

#define BLACK 0
#define WHITE 1
#define FORMAT_FFAT false

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
char serial_buffer[64];
char new_ssid[64];

void setup() {   
  if (FORMAT_FFAT) FFat.format();
  if(!FFat.begin()){
        Serial.println("FFat Mount Failed");
        return;
  } 
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  display.begin();
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(BLACK);
  Serial.begin(9600);
  Serial.setDebugOutput(true);
  delay(500);
  Serial.println("\nType 1 and ENTER within five seconds to setup WiFi access");
  Serial.setTimeout(5000);
  Serial.readBytesUntil('\n', serial_buffer, 64);
  Serial.print("You have typed: ");
  Serial.println(serial_buffer);

  if(serial_buffer[0] == '1') {
    Serial.print("Enter your SSID: ");
    Serial.setTimeout(60000);
    Serial.readBytesUntil('\n', serial_buffer, 64);
    Serial.print("You typed this SSID: ");
    Serial.println(serial_buffer);
    deleteFile(FFat, "/ssid.txt");
    writeFile(FFat, "/ssid.txt", serial_buffer);
    Serial.print("Enter your WiFi password: ");
    Serial.readBytesUntil('\n', serial_buffer, 64);
    Serial.print("You typed this password: ");
    Serial.println(serial_buffer);
    deleteFile(FFat, "/password.txt");
    writeFile(FFat, "/password.txt", serial_buffer);
    Serial.println("\nThank you. Here are your inputs. I hope they will work:");
    readFile(FFat, "/ssid.txt");
    readFile(FFat, "/password.txt");

    readFile_s(FFat, "/ssid.txt", *new_ssid);
    Serial.print("\nYour new SSID is according to the written file: ");
    Serial.println(new_ssid);
  }

  Serial.printf("Total space: %10u\n", FFat.totalBytes());
  Serial.printf("Free space: %10u\n", FFat.freeBytes());
//  writeFile(FFat, "/ssid.txt", "SWAG-NET 2.4");
//  writeFile(FFat, "/password.txt", "rdmNbz.11kYOLO");
  listDir(FFat, "/", 0);

  WiFi.begin(ssid, password);
  Serial.println("\nConnecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    display.setCursor(0, 0);
    display.println("Connecting");
    display.println("to WiFi...");
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
  
  for(int i = 0; i < wait_time; i += delay_ms) {
    delay(delay_ms);
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
  //Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
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


void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("- failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println(" - not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.path(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("\tSIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}


void readFile(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\r\n", path);

    File file = fs.open(path);
    if(!file || file.isDirectory()){
        Serial.println("- failed to open file for reading");
        return;
    }

    Serial.println("- read from file:");
    while(file.available()){
        Serial.write(file.read());
    }
    file.close();
}

 
void readFile_s(fs::FS &fs, const char * path, char buff) {
  
  Serial.printf("reading file: %s\r\n", path);

  File file = fs.open(path);
  if(!file || file.isDirectory()) {
    Serial.println(" - failed to open file for reading");
    return;
  }
  int i = 0;
  Serial.println("- read from file:");
  while(file.available()) {
    buff = file.read();
  }
  
  file.close();
}

void writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\r\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("- failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("- file written");
    } else {
        Serial.println("- write failed");
    }
    file.close();
}

void deleteFile(fs::FS &fs, const char * path){
    Serial.printf("Deleting file: %s\r\n", path);
    if(fs.remove(path)){
        Serial.println("- file deleted");
    } else {
        Serial.println("- delete failed");
    }
}
