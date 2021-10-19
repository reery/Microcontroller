#include <U8g2lib.h>
#include <Wire.h>
#include <ESP8266WiFi.h>

const char* ssid = "SWAG-NET 2.4";
const char* password = "​rdmNbz.11kYOLO";

U8G2_SSD1306_128X64_NONAME_F_SW_I2C display(U8G2_R0, 5, 4, LED_BUILTIN);

int i = 0, n = 0, cursorPosY = 20, nwCount = 5;

void setup() {
  //Wire.setClock(800000L);
  display.begin();
  display.setContrast(0);
  
  //Wire.begin(4,5);
  display.setFont(u8g2_font_6x10_mr);
  
  WiFi.mode(WIFI_STA);
  /*WiFi.begin(ssid, password);
  
  display.setCursor(0, 30);
  display.print(ssid);
  display.drawStr(0, 40, "IP: ");
  display.setCursor(20, 40);
  display.print(WiFi.localIP());

  display.sendBuffer();*/

  // Debug
  //Serial.begin(115200);
}

void loop() {
  i = WiFi.scanNetworks();
  
  display.clearBuffer();
  display.drawStr(0, 10, "WiFi networks:");
  display.setCursor(90, 10);
  display.print(n);
  display.setCursor(102, 10); display.print("/");
  display.setCursor(108, 10); display.print(i);
  display.sendBuffer();
  delay(500);
  
  if (i == 0) {
    display.setCursor(0, 20);
    display.print("No networks found");
    display.sendBuffer();
  }

  if (i < 5) {
    cursorPosY = 20;
    
    for(n = 0; n < i; n++, cursorPosY += 10) {
      display.setCursor(0, cursorPosY);
      display.print(WiFi.SSID(n));
      display.sendBuffer();
      yield();
    }
    
    } else if (i >= 5) {
      cursorPosY = 20;
      for(n = 0; n < i; n++, cursorPosY += 10) {
        
        if(n % 5 == 0) {
          delay(3000);
          display.clearBuffer();
          display.drawStr(0, 10, "WiFi networks:");
          cursorPosY = 20;
          display.setCursor(90, 10);
          display.print(n + 1);
          display.setCursor(102, 10); display.print("/");
          display.setCursor(108, 10); display.print(i);
          display.sendBuffer();
        }
        
        //display.clearBuffer();
        display.drawStr(0, 10, "WiFi networks:");
        display.setCursor(90, 10);
        display.print(n + 1);
        display.setCursor(102, 10); display.print("/");
        display.setCursor(108, 10); display.print(i);
        display.setCursor(0, cursorPosY);
        display.print(WiFi.SSID(n));
        display.sendBuffer();
        yield();
      }
      
    }
    delay(3000);
    n = 0;
}
