#include <Arduino.h>
#include "MHZ19.h"                                        
#include <SoftwareSerial.h>                                // Remove if using HardwareSerial
#include <Wire.h>
#include <U8g2lib.h>

#define RX_PIN 10                                          // Rx pin which the MHZ19 Tx pin is attached to
#define TX_PIN 11                                          // Tx pin which the MHZ19 Rx pin is attached to
#define BAUDRATE 9600                                      // Device to MH-Z19 Serial baudrate (should not be changed)

MHZ19 myMHZ19;                                             // Constructor for library
SoftwareSerial mySerial(RX_PIN, TX_PIN);                   // (Uno example) create device to MH-Z19 serial
U8G2_SSD1306_128X64_NONAME_2_HW_I2C display(U8G2_R0,/* reset=*/ U8X8_PIN_NONE);

unsigned long getDataTimer = 0;

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    //Serial.begin(9600);                                     // Device to serial monitor feedback
    display.begin();
    display.setContrast(0);
    mySerial.begin(BAUDRATE);                               // (Uno example) device to MH-Z19 serial start   
    myMHZ19.begin(mySerial);                                // *Serial(Stream) refence must be passed to library begin(). 
  
    myMHZ19.autoCalibration();                              // Turn auto calibration ON (OFF autoCalibration(false))
    
}

void loop()
{
    if (millis() - getDataTimer >= 2000)
    {
        int CO2; 

        /* note: getCO2() default is command "CO2 Unlimited". This returns the correct CO2 reading even 
        if below background CO2 levels or above range (useful to validate sensor). You can use the 
        usual documented command with getCO2(false) */

        CO2 = myMHZ19.getCO2();                             // Request CO2 (as ppm)
        
//        Serial.print("CO2 (ppm): ");                      
//        Serial.println(CO2);

        int8_t Temp;
        Temp = myMHZ19.getTemperature();                     // Request Temperature (as Celsius)
        
        display.firstPage();
        do {
          display.setFont(u8g2_font_VCR_OSD_mr);
        display.drawStr(0, 15, "Temp ");
        display.setCursor(55, 15);
        display.print(Temp);
        display.drawStr(0, 40, "Co2  ");
        display.setCursor(55, 40);
        display.print(CO2);
        } while (display.nextPage());
        
//        display.setCursor(0, 33);
//        display.print(F("Temp: "));
//        display.setCursor(45, 33);
//        display.print(F("Co2: "));


//        Serial.print("Temperature (C): ");                  
//        Serial.println(Temp);                               

        getDataTimer = millis();
    }
}
