#include <TFT_eSPI.h>
#include <SPI.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#define BOTTOM 152 // 160 total pixels but 0-159, minus 1 white line and 8 for the first block
#define RIGHT 80

uint16_t pixelColor = 0;
uint8_t location1[2];
uint8_t location2[2];
Adafruit_MPU6050 mpu;
sensors_event_t a, g, temp;
float accXfloat;
float accYfloat;
int accXpos, accYpos;
int accXint, accYint;
int arrX[10], arrY[10], arrCounter = 0, i = 0;
uint8_t blockArray[10][19]; // define the block field for tetris [x][y]

TFT_eSPI tft = TFT_eSPI();

void setup() {
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens
  
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  //initializeGyro();

  
  //pixelColor = convertColor(0,255,0);
}

void loop() {
  drawUI();
  tetrisPhysics(1);
  tetrisPhysics(20);
  tetrisPhysics(40);
  //delay(16);
}

void tetrisPhysics(int x) {
  int y = 1;
  int blockX = 0, blockY = 0, yCounter = 0, xCounter = 0;
  delay(1000);
  for(i = 0; i < BOTTOM - 1; i++, y++) {
    
    tft.fillRect(x, y - 1, 32, 1, TFT_BLACK); // erases top line to reduce flickering
    tft.fillRect(x, y, 32, 8, convertColor(13,160,247)); // draws the actual tetromino

    // checks in which y-quadrant the left most block is
    // improve performance: just check blocks below the moving block, skip the rest
    for(yCounter = BOTTOM - 1, blockY = 0; y < yCounter; yCounter = yCounter - 8, blockY++);

    // check for x-quadrant
    for(xCounter = 0, blockX = 0; x > xCounter; xCounter = xCounter + 8, blockX++);

    tft.setTextPadding(20);
    tft.setCursor(103, 0, 1);
    tft.print("x");
    tft.setCursor(103, 8, 1);
    tft.fillRect(103, 8, 24, 8, TFT_BLACK); // erase text so it doesn't stay
    tft.print(x);
    tft.setCursor(103, 20, 1);
    tft.print("y");
    tft.setCursor(103, 28, 1);
    tft.fillRect(103, 28, 24, 8, TFT_BLACK); // erase text so it doesn't stay
    tft.print(y);
    tft.setCursor(103, 40, 1);
    tft.print("bx");
    tft.fillRect(103, 48, 24, 8, TFT_BLACK); // erase text so it doesn't stay
    tft.setCursor(103, 48, 1);
    tft.print(blockX);
    tft.setCursor(103, 60, 1);
    tft.print("by");
    tft.fillRect(103, 68, 24, 8, TFT_BLACK); // erase text so it doesn't stay
    tft.setCursor(103, 68, 1);
    tft.print(blockY);
    delay(25); // should use millis()
    
    // checks if the next quadrant (bottom) is occupied, and if so, the loop has to end
    if(blockArray[blockX][blockY-1] >= 1) {
      blockArray[blockX][blockY] = 1;
      break;
    }

    if(blockArray[blockX+1][blockY-1] >= 1) {
      blockArray[blockX+1][blockY] = 1;
      break;
    }

    if(blockArray[blockX+1][blockY-1] >= 1) {
      blockArray[blockX+1][blockY] = 1;
      break;
    }

    if(blockArray[blockX+1][blockY-1] >= 1) {
      blockArray[blockX+1][blockY] = 1;
      break;
    }

  }

  blockArray[blockX][blockY] = 1;
  blockArray[blockX+1][blockY] = 1;
  blockArray[blockX+2][blockY] = 1;
  blockArray[blockX+3][blockY] = 1;
  for(i = 19; i >= 0; i--) {
    Serial.print("blockArray[0]["); Serial.print(i); Serial.print("]: "); Serial.println(blockArray[0][i]);
  }
  Serial.println("");
  
}

void drawUI(void) {
  tft.drawFastVLine(0,0,160,TFT_WHITE); // left line
  tft.drawFastVLine(81,0,160,TFT_WHITE);  // right line
  tft.drawFastHLine(1,159,80,TFT_WHITE); // bottom line
}

// Converts RGB888 to RGB565
// See: http://www.barth-dev.de/online/rgb565-color-picker/
uint16_t convertColor(uint8_t red, uint8_t green, uint8_t blue) {
  uint16_t Rgb565;
  Rgb565 = (red & 0b11111000) <<8;
  Rgb565 = Rgb565 + ((green & 0b11111100 ) <<3);
  Rgb565 = Rgb565 + ((blue) >> 3);

  return Rgb565;
}

// Creates a random number and puts it into a given array of two
void getRandomXY(uint8_t arr[2]) {
  randomSeed(analogRead(5));
  arr[0] = random(0, 127);
  arr[1] = random(0, 159);
}

void connectRandomLines(uint32_t lines, uint16_t lineColor) {
  getRandomXY(location1);
  getRandomXY(location2);
  tft.drawLine(location1[0], location1[1], location2[0], location2[1], lineColor);
  tft.drawCircle(location1[0], location1[1], 3, convertColor(255,0,0));
  delay(100);
  int start = millis();
  int ending = 0;
  
  for(int i = 0; i < (lines - 1) ; i++) {
    getRandomXY(location1);
    tft.drawLine(location1[0], location1[1], location2[0], location2[1], lineColor);
    location2[0] = location1[0];
    location2[1] = location1[1];
  }
  ending = millis();
  int result = ending - start;
//  Serial.print("Time taken: ");
//  Serial.print(result);
//  Serial.println(" ms");
}

void initializeGyro(void) {

//    Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
//    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
//  Serial.println("MPU6050 Found!");
  
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
//  Serial.print("Accelerometer range set to: ");
//  switch (mpu.getAccelerometerRange()) {
//  case MPU6050_RANGE_2_G:
//    Serial.println("+-2G");
//    break;
//  case MPU6050_RANGE_4_G:
//    Serial.println("+-4G");
//    break;
//  case MPU6050_RANGE_8_G:
//    Serial.println("+-8G");
//    break;
//  case MPU6050_RANGE_16_G:
//    Serial.println("+-16G");
//    break;
//  }
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
// Serial.print("Gyro range set to: ");
//  switch (mpu.getGyroRange()) {
//  case MPU6050_RANGE_250_DEG:
//    Serial.println("+- 250 deg/s");
//    break;
//  case MPU6050_RANGE_500_DEG:
//    Serial.println("+- 500 deg/s");
//    break;
//  case MPU6050_RANGE_1000_DEG:
//    Serial.println("+- 1000 deg/s");
//    break;
//  case MPU6050_RANGE_2000_DEG:
//    Serial.println("+- 2000 deg/s");
//    break;
//  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
//  Serial.print("Filter bandwidth set to: ");
//  switch (mpu.getFilterBandwidth()) {
//  case MPU6050_BAND_260_HZ:
//    Serial.println("260 Hz");
//    break;
//  case MPU6050_BAND_184_HZ:
//    Serial.println("184 Hz");
//    break;
//  case MPU6050_BAND_94_HZ:
//    Serial.println("94 Hz");
//    break;
//  case MPU6050_BAND_44_HZ:
//    Serial.println("44 Hz");
//    break;
//  case MPU6050_BAND_21_HZ:
//    Serial.println("21 Hz");
//    break;
//  case MPU6050_BAND_10_HZ:
//    Serial.println("10 Hz");
//    break;
//  case MPU6050_BAND_5_HZ:
//    Serial.println("5 Hz");
//    break;
//  }
//
//  Serial.println("");
  delay(100);
}

void printValues(void) {

  mpu.getEvent(&a, &g, &temp);
  tft.setCursor(0,0,1);
  tft.println("Acceleration: ");
  tft.print("X: ");
  tft.println(a.acceleration.x);
  tft.print("Y: ");
  tft.println(a.acceleration.y);
  tft.print("Z: ");
  tft.println(a.acceleration.z);
  tft.println("");
  tft.println("Rotation: ");
  tft.print("X: ");
  tft.println(g.gyro.x);
  tft.print("Y: ");
  tft.println(g.gyro.y);
  tft.print("Z: ");
  tft.println(g.gyro.z);
  tft.println("");
  tft.print("Temperature: ");
  tft.print(temp.temperature);
  tft.println("C");
  
}

void printGyro(void) {
  //tft.fillCircle(accXpos,accYpos,2,TFT_BLACK);
  tft.fillRect(35,101,58,58, TFT_BLACK);
  tft.drawRect(34,100,60,60,TFT_RED);
  accXfloat = a.acceleration.x * 100;
  accYfloat = a.acceleration.y * 100;
  
  if(accXfloat >= 0) {
    accXint = int(accXfloat);
    if(accXint > 1000) accXint = 1000;
    accXpos = map(accXint,0,1000,64,34);
  } 

    if(accXfloat < 0) {
    accXint = int(accXfloat);
    if(accXint < -1000) accXint = -1000;
    accXpos = map(accXint,-1000,0,94,64);
  }

  if(accYfloat <= 0) {
    accYint = int(accYfloat);
    if(accYint < -1000) accYint = -1000;
    accYpos = map(accYint,-1000,0,100,130);
  }

  if(accYfloat > 0) {
    accYint = int(accYfloat);
    if(accYint > 1000) accYint = 1000;
    accYpos = map(accYint,0,1000,130,160);
  }
  uint8_t mblurColor = 20;

  for(i = 0; i < 10; i++) {
    tft.fillCircle(arrX[i], arrY[i],2,convertColor(mblurColor,0,0));
    mblurColor = mblurColor + 20;
  }
  
  tft.fillCircle(accXpos,accYpos,2,TFT_RED);
     
  arrX[arrCounter] = accXpos;
  arrY[arrCounter] = accYpos;

    
  if(arrCounter < 9) {
    arrCounter++;
  } else {
    arrCounter = 9;
    for(i = 0; i < 9; i++) {
    arrX[i] = arrX[i+1];
    arrY[i] = arrY[i+1];
   }
  }
}
