#include <Wire.h>
#include <U8g2lib.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C display(U8G2_R0, 5, 4, LED_BUILTIN);

void setup() {
  //Wire.setClock(800000L);
  display.begin();
  display.setContrast(0);
  
  //Wire.begin(4,5);
  display.setFont(u8g2_font_6x10_mr);

  // Debug
  Serial.begin(115200);
}

void loop() {
  display.clearBuffer();
  display.drawStr(0, 10, "WiFi networks:");
  delay(500);
  display.sendBuffer();
  Serial.println("Test");
}
