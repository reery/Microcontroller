#include <U8g2lib.h>
#include <Wire.h>
#define FONTHEIGHT 15
#define ANALOGPIN A0

U8G2_SSD1306_128X64_NONAME_F_HW_I2C display(U8G2_R0,/* reset=*/ U8X8_PIN_NONE);

uint8_t menu_font_height = 16;
uint8_t upButtonState = 0;
uint8_t downButtonState = 0;
uint8_t okButtonState = 0;
uint8_t backButtonState = 0;
int8_t menuOption = 0;
int analogReadout = 0;
int analogReadout1 = 0;
int analogReadMean = 0;
float batteryVoltage, voltsMean;
uint8_t batteryPercentage = 0;

void setup() {
  display.begin();
  display.setContrast(0);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(ANALOGPIN, INPUT);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // Debug
  //Serial.begin(9600);
}

void loop() {
    menuOption = showMenu();
    showBatteryTest(menuOption);
}

int8_t showMenu(void) {
   while (1) {
   display.clearBuffer();
   display.setFont(u8g2_font_9x15_mr);
   display.drawStr(0, 10, "Battery type");
   display.drawHLine(0, 15, 128);
   display.setDrawColor(2);
   if (menuOption == 0)
    display.setFont(u8g2_font_9x15B_mf);
    
   display.drawStr(2, 30, "Alkaline");
   display.setFont(u8g2_font_9x15_mr);
   
   if (menuOption == 1)
    display.setFont(u8g2_font_9x15B_mf);
    
   display.drawStr(2, 45, "Ni-MH");
   display.setFont(u8g2_font_9x15_mr);
   
   if (menuOption == 2)
    display.setFont(u8g2_font_9x15B_mf);
    
   display.drawStr(2, 60, "Li-Ion");
   display.setFont(u8g2_font_9x15_mr);
   display.drawBox(0, menu_font_height, 128, FONTHEIGHT);
  

   if (menuOption <= 2) {
    upButtonState = digitalRead(2);
    downButtonState = digitalRead(3);
    okButtonState = digitalRead(5);

    if (upButtonState == 1) {
      menuOption--;
      delay(50);
    }

    if (downButtonState == 1) {
      menuOption++;
      delay(50);
    }
   } else {
    menuOption = 0;
   }

   if (menuOption < 0) {
    menuOption = 2;
   }

   switch (menuOption) {
      case 0:
              menu_font_height = FONTHEIGHT + 1;
              break;
      case 1:
              menu_font_height = (FONTHEIGHT * 2) + 1;
              break;
      case 2: 
              menu_font_height = (FONTHEIGHT * 3) + 1;
              break;
    }
     display.sendBuffer();

     if (okButtonState == 1) {
      return menuOption;
      delay(100);
     }
   }
   
}

void showBatteryTest(int8_t menuOption) {
   while (1) {
   display.clearBuffer();
   display.setFont(u8g2_font_9x15_mr);
   display.setCursor(0, 10);
   
   switch (menuOption) {
    case 0: 
      display.print("Alkaline");
      display.setCursor(0, 33);
      batteryVoltage = calcVolts();
      display.print(batteryVoltage);
      display.setCursor(45, 33);
      display.print("V");
      printAlkalineCharge(&batteryVoltage);
      //display.setCursor(60, 33);
      //display.print(analogRead(ANALOGPIN));
      break;
    case 1:
      display.print("Ni-MH");
      display.setCursor(0, 33);
      batteryVoltage = calcVolts();
      display.print(batteryVoltage);
      display.setCursor(45, 33);
      display.print("V");
      printNiMhCharge(&batteryVoltage);
      break;
    case 2:
      display.print("Li-Ion");
      display.setCursor(0, 33);
      batteryVoltage = calcVolts();
      display.print(batteryVoltage);
      display.setCursor(45, 33);
      display.print("V");
      printLiIonCharge(&batteryVoltage);
      break;
   }
   display.drawHLine(0, 15, 128);
   display.sendBuffer();

  backButtonState = digitalRead(4);
  if (backButtonState == 1) {
    return;
  }
  }
}

float calcVolts(void) {
  analogReadout = analogRead(ANALOGPIN);
  delay(100);
  analogReadout1 = analogRead(ANALOGPIN);
  
  if (analogReadout1 > analogReadout + 10) {
    return 0.0;
  }
  
  if (analogReadout < 50) {
    return 0.0;
  } else {
    return (((float)analogReadout + (float)analogReadout1) / 2) * 5.0 / 1023.0;
  }
}

void printAlkalineCharge(float * batteryVoltage) {
  display.drawFrame(32, 40, 64, 20);
  display.drawBox(98, 46, 3, 8);
  display.setCursor(44, 54);
  if (*batteryVoltage > 1.5) {
    display.print("100%");
  } else if (*batteryVoltage < 1.0) {
    display.print("  0%");
  } else {
    batteryPercentage = (*batteryVoltage - 1) * 100.0 / 1.5;
    display.setCursor(53, 54);
    display.print(batteryPercentage);
    display.setCursor(71, 54);
    display.print("%");
  }
}

void printNiMhCharge(float * batteryVoltage) {
  display.drawFrame(32, 40, 64, 20);
  display.drawBox(98, 46, 3, 8);
  display.setCursor(44, 54);
  if (*batteryVoltage > 1.3) {
    display.print("100%");
  } else if (*batteryVoltage < 1.1) {
    display.print("  0%");
  } else {
    batteryPercentage = (*batteryVoltage - 1) * 100.0 / 1.3;
    display.setCursor(53, 54);
    display.print(batteryPercentage);
    display.setCursor(71, 54);
    display.print("%");
  }
}

void printLiIonCharge(float * batteryVoltage) {
  display.drawFrame(32, 40, 64, 20);
  display.drawBox(98, 46, 3, 8);
  display.setCursor(44, 54);
  if (*batteryVoltage > 4.15) {
    display.print("100%");
  } else if (*batteryVoltage < 3.6) {
    display.print("  0%");
  } else {
    batteryPercentage = (*batteryVoltage - 3.6) * 100.0 / 0.55;
    display.setCursor(53, 54);
    display.print(batteryPercentage);
    display.setCursor(71, 54);
    display.print("%");
  }
}

void printWarning(void){
  
}

