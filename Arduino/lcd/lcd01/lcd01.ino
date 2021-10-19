// LCD01
// By Slavko
// Prints temperature, a counter, 10s and 30s average
// and an arrow indicator of the temperature change trend
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
float sensor_volt, temp, temp1, temp2, temp_avg_10, temp_avg_30;
int i, counter, counter_10_1 = 0, counter_10_2 = 0, counter_30_1 = 0, counter_30_2 = 0;
float temp_average_10s[10];
float temp_average_30s[30];

// Degrees symbol for LCD byte 0
byte degrees[8] = {
  B00010,
  B00101,
  B00010,
  B00000,
  B00000,
  B00000,
  B00000,
};

// Arrow down symbol for LCD byte 1
byte arrow_down[8] = {
  B00100,
  B00100,
  B00100,
  B10101,
  B01110,
  B00100,
  B00000,
};

// Arrow up symbol for LCD byte 2
byte arrow_up[8] = {
  B00100,
  B01110,
  B10101,
  B00100,
  B00100,
  B00100,
  B00000,
};

void setup() {
  // Create chars for LCD
  lcd.createChar(0, degrees);
  lcd.createChar(1, arrow_down);
  lcd.createChar(2, arrow_up);

  // Must initialise LCD
  lcd.begin(16, 2);

  // Print "Temp:" and "°C" in first row, "Avg:" in second
  lcd.print("Temp: ");
  lcd.setCursor(0, 1);
  lcd.print("Avg:");
  lcd.setCursor(10,0);
  lcd.write(byte(0));
  lcd.setCursor(11,0);
  lcd.print("C");
}

void loop() {
  // Read A0 analog value and calculate voltage from value
  sensor_volt = (analogRead(A0))/1024.0 * 5;

  // Calculate temp from two volt-values with 50 ms difference
  // to couter value spikes, print in first row and wait a total of 1 sec
  lcd.setCursor(6,0);
  temp1 = (sensor_volt - 0.5) * 100;
  delay(50);
  temp2 = (sensor_volt - 0.5) * 100;
  temp = (temp1 + temp2) / 2;
  lcd.print(temp, 1);
  delay(950);

  // Fill 10 and 30 sec average arrays with temp values
  temp_average_10s[counter_10_1] = temp;
  temp_average_30s[counter_30_1] = temp;

  // counter_10_2 is used for calculating average value
  // Here it increments from 0 up to 10 and stays at 10
  if(counter_10_2 < 9)
    counter_10_2++;

  // Same for counter_30_2
  if(counter_30_2 < 29)
    counter_30_2++;

  // counter_10_1 is used for array position
  // Here it will be set to 0 if it hits 10
  if(counter_10_1 > 9)
    counter_10_1 = 0;

  // Same for counter_30_1
  if(counter_30_1 > 29)
    counter_30_1 = 0;

  // This IF starts only if the counter for average calculation is 2 or above.
  // This will start adding values to the array.
  // Because i starts at 1, the ELSE is used to fill in the 0-position of the array.
  if(counter_10_2 > 1){
  for(i = 1; i < counter_10_2; i++)
    temp_avg_10 += temp_average_10s[i];

  for(i = 1; i < counter_30_2; i++)
    temp_avg_30 += temp_average_30s[i];
  } else {
    temp_avg_10 = temp_average_10s[0];
    temp_avg_30 = temp_average_30s[0];
  }

  // Average values will be calculated by dividing the sum of all array contents
  // by the 2nd counter variable.
  temp_avg_10 = temp_avg_10 / counter_10_2;
  temp_avg_30 = temp_avg_30 / counter_30_2;

  // This prints the two average values on the LCD
  if(counter_10_1 > 0){
  lcd.setCursor(5,1);
  lcd.print(temp_avg_10, 1);
  lcd.setCursor(11,1);
  lcd.print(temp_avg_30, 1);
  }

  // This prints the up, down arrow "-" if the 10s average is below/above
  // the 30s average.
  if(temp_avg_10 < temp_avg_30){
    lcd.setCursor(9,1);
    lcd.write(byte(1));
  } else if(temp_avg_10 > temp_avg_30){
    lcd.setCursor(9,1);
    lcd.write(byte(2));
  } else{
    lcd.setCursor(9,1);
    lcd.write("-");
  }

  // This prints a integer number counter on the top right of the LCD.
  // It counts to 30, then begins at 1 again.
  // Some IFs and setCursors are needed to print blanks in places when
  // the digits switch from 30 to 1 to clear the "3".
  if(counter < 10){
    lcd.setCursor(15,0);
    lcd.print(counter);
    counter++;
  } else if(counter < 31){
    lcd.setCursor(14,0);
    lcd.print(counter);
    counter++;
  } else{
    counter = 1;
    lcd.setCursor(14,0);
    lcd.print(" ");
    lcd.setCursor(15,0);
    lcd.print(counter);
    counter++;
  }

  // Finally increment the array position
  counter_10_1++;
  counter_30_1++;
}
