// Program to demonstrate the MD_Parola library
//
// Simplest program that does something useful - Hello World!
//
// MD_MAX72XX library can be found at https://github.com/MajicDesigns/MD_MAX72XX
//

#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

// Define the number of devices we have in the chain and the hardware interface
// NOTE: These pin numbers will probably not work with your hardware and may
// need to be adapted
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CLK_PIN   18
#define DATA_PIN  23
#define CS_PIN    5

// Hardware SPI connection
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
// Arbitrary output pins
// MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

void setup(void)
{
  P.begin();
  P.displayText("derp", PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
  //Serial.begin(115200);
}

void loop(void)
{
  uint8_t i;
  
  for(i = 0; i < 15; i++) {
    P.setIntensity(i);
    P.displayAnimate();
    delay(500);
    //Serial.println(i);
    yield();
  }

  for(i = 15; i > 1; i--) {
    P.setIntensity(i);
    P.displayAnimate();
    delay(500);
    //Serial.println(i);
    yield();
  }
  
  
}
