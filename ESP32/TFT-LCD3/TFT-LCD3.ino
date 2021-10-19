#include <TFT_eSPI.h>
#include <SPI.h>
#define LED 5

long pixelDrawn = 0;
bool pixelDrawnViewportSet = 0;
int linesDrawn = 0;

TFT_eSPI tft = TFT_eSPI();

void setup() {
 pinMode(LED, OUTPUT);
 digitalWrite(LED, HIGH);
 //Serial.begin(9600);

 tft.init();
 tft.setRotation(0);
 tft.fillScreen(TFT_BLACK);
 randomSeed(analogRead(5));
}

void loop() {
//  tft.setViewport(0, 0, 128, 160);
//  tft.frameViewport(TFT_WHITE, 1);
  
  while(pixelDrawn < 1000) {
    printDots();
    setPixelDrawnViewport();
  }

  delay(2000);

  drawRandomLines(1);
  
  yield();
  
}

void printDots(void) {
  tft.drawPixel(random(0,127), random(13,159), random(0,65535));
  pixelDrawn++;
}

void setPixelDrawnViewport(void) {
  tft.setViewport(0, 0, 128, 12);
//  tft.frameViewport(TFT_WHITE, 1);
  tft.setCursor(2, 3, 1);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.fillScreen(TFT_WHITE);
  tft.print("Pixel drawn: ");
  tft.println(pixelDrawn);
  tft.resetViewport();
  delay(random(10, 30));
  yield();
}

void drawRandomLines(int num) {
  if(linesDrawn >= num) {
    return;
  }
  
  for(int i = 0; i < num; i++) {
    int x = random(0,127);
    int y = random(13,159);
    while(tft.readPixel(x, y) == 0) {
      topBannerMessage("Looking for pixel...");
      x = random(0,127);
      y = random(13,159);
      continue;
    }

  topBannerMessage("Found pixel");
  delay(1000);
  tft.setViewport(0, 0, 128, 12);
  tft.setCursor(2, 3, 1);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.fillScreen(TFT_WHITE);
  tft.print(x);
  tft.print("  ");
  tft.println(y);
  tft.resetViewport();
  delay(1000);
  linesDrawn++;
  }

  
}

void topBannerMessage(char text[32]) {
  tft.setViewport(0, 0, 128, 12);
//  tft.frameViewport(TFT_WHITE, 1);
  tft.setCursor(2, 3, 1);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.fillScreen(TFT_WHITE);
  tft.println(text);
  tft.resetViewport();
  yield();
}
