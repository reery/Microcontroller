//#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define WIDTH 115

float temp, temp1, temp2;
unsigned int seconds = 0, minutes = 0, now = 0, previous = 0;
unsigned int previous1 = 0;
unsigned short row = 0, rowArray_counter, i, j, minuteArray_counter = 0;
float minTemp = 18, maxTemp = 24, tempDiff = 0;
float rowArray[WIDTH];
float height = 0;
uint16_t heightInt = 0;
unsigned short secondsCounter = 0;
//float minuteArray[WIDTH];

/*static const unsigned char PROGMEM degrees_bmp[] =
{ B00100,
  B01010,
  B00100 };*/

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.ssd1306_command(SSD1306_SETCONTRAST);
  display.ssd1306_command(0);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  pinMode(8, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  temp1 = (((analogRead(A0))/1024.0 * 5) - 0.5) * 100;

  now = millis();
  
  if(now - previous > 1000){
    display.clearDisplay();
    previous = now;
    
    drawGraph(rowArray, &minTemp, &maxTemp);
    display.setTextSize(2);
    display.setCursor(0,0);
    display.println("XXX: ");
    display.setCursor(60,0);
    display.println(temp, 1);
    display.setCursor(72,0);
    //display.drawBitmap(64, 0, degrees_bmp, 8, 3, 1);
    //display.println("C");
    display.drawFastHLine(0,16, 128, WHITE);
    printOneMinute();
    
    //display.ssd1306_command(SSD1306_DISPLAYOFF);



  if(now - previous1 > 60000){
    previous1 = now;
    temp2 = (((analogRead(A0))/1024.0 * 5) - 0.5) * 100;
    temp = (temp1 + temp2) / 2;
    rowArray[rowArray_counter] = temp;

    maxTemp = calcMax(rowArray, WIDTH) + 3.0;
    
    if(rowArray_counter < WIDTH - 1){
      minTemp = maxTemp - 7;
    } else
    minTemp = calcMin(rowArray, WIDTH) - 3.0;

    if(rowArray_counter >= WIDTH - 1){
      shiftArray(rowArray, WIDTH);
    } else if(rowArray_counter < WIDTH)
      rowArray_counter++;
  }
  }
   
}

uint16_t calculateHeight(float * temp, float * minTemp, float * maxTemp){
  if(*temp <= *minTemp)
    return minTemp;
  else if(*temp >= *maxTemp)
    return *maxTemp;

  tempDiff = *maxTemp - *minTemp;
  height = (*maxTemp - *temp) * 47.0 / tempDiff;
  return 17 + (uint16_t)height;
}

void drawGraph(float * rowArray, float * minTemp, float * maxTemp){
  for(i = 0; i < WIDTH; i++){
    if(rowArray[i] < *minTemp){
      heightInt = 64;
    } else if(rowArray[i] > *maxTemp){
      heightInt = 17;
    } else { 
    heightInt = calculateHeight(&rowArray[i], minTemp, maxTemp);
    }
    
    display.drawPixel(i, heightInt, WHITE);
    fillPixelGraph(&heightInt, &i);
  }
}

void shiftArray(float * rowArray, const int arraySize){
  for(i = 0; i < arraySize; i++){
    if(i < (arraySize - 1)){
      rowArray[i] = rowArray[i + 1];
    } else {
      rowArray[arraySize - 1] = 0.0;
    }
  }
}

void fillPixelGraph(uint16_t * height, uint16_t * x){
  for(j = 64; j > *height; j--)
    display.drawPixel(*x, j, WHITE);
}

float calcMax(const float * numArray, const int arraySize){
  float maxValue = -100.0, tempValue = 0.0;

  for(i = 0; i < arraySize; i++){
    tempValue = numArray[i];

    if(tempValue > maxValue)
      maxValue = tempValue;
  }

  return maxValue;
}

float calcMin(const float * numArray, const int arraySize){
  float minValue = 1000.0, tempValue = 0.0;

  for(i = 0; i < arraySize; i++){
    tempValue = numArray[i];

    if(tempValue < minValue)
      minValue = tempValue;
  }

  return minValue;
}

void printOneSec(void){
    display.setTextSize(1);
    display.setCursor(116,20);
    display.println((int)maxTemp);
    display.setCursor(116,56);
    display.println((int)minTemp);
    display.setCursor(116,0);
    display.println("1");
    display.setCursor(110,8);
    display.println("sec");
    display.display();
}

void printOneMinute(void){
    display.setTextSize(1);
    display.setCursor(116,20);
    display.println((int)maxTemp);
    display.setCursor(116,56);
    display.println((int)minTemp);
    display.setCursor(116,0);
    display.println("1");
    display.setCursor(110,8);
    display.println("min");
    display.display();
}

float calcAvg(const float * numArray, const int arraySize){
  float tempValue = 0;

  for(i = WIDTH - arraySize; i < arraySize; i++){
    tempValue += numArray[i];
  }

  return tempValue / arraySize;
}
