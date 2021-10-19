#include <TFT_eSPI.h>
#include <SPI.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

uint16_t pixelColor = 0;
uint8_t location1[2];
uint8_t location2[2];
Adafruit_MPU6050 mpu;
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

TFT_eSPI tft = TFT_eSPI();

void setup() {
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");
  
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }

  Serial.println("");
  delay(100);
  
  pixelColor = convertColor(0,255,0);
  connectRandomLines(300, pixelColor);
}

void loop() {
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  /* Print out the values */
  tft.setCursor(0,0,1);
  tft.print("Acceleration X: ");
  tft.println(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");

  Serial.print("Rotation X: ");
  Serial.print(g.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);
  Serial.print(", Z: ");
  Serial.print(g.gyro.z);
  Serial.println(" rad/s");

  Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.println(" degC");

  Serial.println("");
  delay(500);
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
  Serial.print("Time taken: ");
  Serial.print(result);
  Serial.println(" ms");
}
