// LCD03
// By Slavko
// Prints temperature, a counter, 10s and 30s average
// and an arrow indicator of the temperature change trend
// Advanced version with graphs and more screens.
#include <LiquidCrystal.h>

// Function prototype
void printTemp(void);
void printGraph(const int, const float *, const float *, const float *);
int calcGraph(const float *, const float *, const float *);
float calcMin(const float *, const int);
float calcMax(const float *, const int);
float calcAvg(const float *, const int);
void shiftAvg(float *, const int);

// Global variables
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
float temp, temp1, temp2;
short i, j, counter;
int switchState;
int buttonPressed = 0, menu = 0;

// For every average graph screen, these variabels have to be created
// This is for the 10 seconds average and 2 minutes graphs
float temp_average_10s[10]; // Array which holds the last 10 seconds of temperature
float temp_avg_2m[12]; // Array which holds the 10 second average within 12 items (120 seconds)
float temp_avg_10; // The calculated average for 10 seconds of the 10-items temp-average_10s-array
int array_10_1 = 0, array_10_2 = 0; // array_10_1 is used for array position of temp_average_10s, array_10_2 is used to calculate averages for the first seconds
float ten_min, ten_max; // The calculated min/max values for the graph of the temp_avg_2m
int twelve_10 = 0; // Used as a counter to fill the 12 array-items of temp_avg_2m

// This is the variables set for 30 seconds average and 6 minutes graphs
float temp_average_30s[30];
float temp_avg_6m[12];
float temp_avg_30;
int array_30_1 = 0, array_30_2 = 0;
float thirty_min, thirty_max;
int twelve_30 = 0;

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

// Arrow down symbol for LCD
byte arrow_down[8] = {
  B00100,
  B00100,
  B00100,
  B10101,
  B01110,
  B00100,
  B00000,
};

// Arrow up symbol for LCD
byte arrow_up[8] = {
  B00100,
  B01110,
  B10101,
  B00100,
  B00100,
  B00100,
  B00000,
};

// 8 Graph icons for LCD for 0-100% bars
byte graph1[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
};

byte graph2[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
};

byte graph3[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111,
};

byte graph4[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111,
  B11111,
};

byte graph5[8] = {
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};

byte graph6[8] = {
  B00000,
  B00000,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};

byte graph7[8] = {
  B00000,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};

byte graph8[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};

void setup() {
  // Listen on digital pin 8 for button
  pinMode(8, INPUT);

  // Must initialise LCD
  lcd.begin(16, 2);
}

void loop() {
  // Read A0 analog value and calculate voltage from value
  //sensor_volt = (analogRead(A0))/1024.0 * 5;

  // Calculate temp from two volt-values with 50 ms difference
  // to couter value spikes, calculate average of both and wait a total of 1 sec
  temp1 = (((analogRead(A0))/1024.0 * 5) - 0.5) * 100;
  delay(50);
  temp2 = (((analogRead(A0))/1024.0 * 5) - 0.5) * 100;
  temp = (temp1 + temp2) / 2;
  delay(950);

  // switchState 0 = no button pressed, 1 = button pressed
  switchState = digitalRead(8);

  // When switch is pressed, buttnPressed variable is set to 1
  if(switchState == 1)
    buttonPressed = 1;

  // Fill 10 and 30 sec average arrays with temp values
  temp_average_10s[array_10_1] = temp;
  temp_average_30s[array_30_1] = temp;

  // array_10_2 is used for calculating average value for the first 10 secs.
  // Here it increments from 0 up to 10 and stays at 10 forever.
  // NOTE: This was 9
  if(array_10_2 < 10)
    array_10_2++;

  // Same for array_30_2 but for the first 30 secs.
  // NOTE: This was 29
  if(array_30_2 < 30)
    array_30_2++;

  // array_10_1 is used for array position of the 10s average counter per second
  // Here it will be set to 0 if it hits 10
  // Additionally the twelve_10 counter is used for counting up to 12 and staying there forever.
  if(array_10_1 > 9){
    array_10_1 = 0;

    // The temp_avg_2m array is used for 2m averages with 10s averages per array-unit.
    // Here calcAvg is used to calculate the average of temp_average_10s and put it into
    // The temp_avg_2m array.
    // This special IF below fills the array and when twelve_10 has reached 12,
    // all contents of the temp_avg_2m array are shifted to the left (0 position value is discarded)
    // and only the last array member (11) will be updated with the latest average value.
    if(twelve_10 < 12){
      temp_avg_2m[twelve_10] = calcAvg(temp_average_10s, 10);
      twelve_10++;
    } else {
      shiftAvg(temp_avg_2m, 12);
      temp_avg_2m[11] = calcAvg(temp_average_10s, 10);
    }
  }

  // Same for array_30_1 as it is for array_10_1.
  if(array_30_1 > 29){
    array_30_1 = 0;

    if(twelve_30 < 12){
      temp_avg_6m[twelve_30] = calcAvg(temp_average_30s, 10);
      twelve_30++;
    } else {
      shiftAvg(temp_avg_6m, 12);
      temp_avg_6m[11] = calcAvg(temp_average_30s, 10);
    }
  }

  // Min and max values are calculated from the whole array.
  ten_min = calcMin(temp_avg_2m, 12);
  ten_max = calcMax(temp_avg_2m, 12);
  thirty_min = calcMin(temp_avg_6m, 12);
  thirty_max = calcMax(temp_avg_6m, 12);

  // This IF starts only if the counter for average calculation is 2 or above.
  // This will start adding values to the 10s average array.
  // Because i starts at 1, the ELSE is used to fill in the 0-position of the array.
  if(array_10_2 > 1){
    for(i = 1; i < array_10_2; i++)
      temp_avg_10 += temp_average_10s[i];

    for(i = 1; i < array_30_2; i++)
      temp_avg_30 += temp_average_30s[i];

  } else {
    temp_avg_10 = temp_average_10s[0];
    temp_avg_30 = temp_average_30s[0];
  }

  // Average values will be calculated by dividing the sum of all array contents
  // by the 2nd counter variable.
  temp_avg_10 = temp_avg_10 / array_10_2;
  temp_avg_30 = temp_avg_30 / array_30_2;

  // This controls the button.
  // If menu 0 is active and the button has been pressed, it will switch menu to 1.
  // If menu is 1 and button is pressed again, it will switch to 0.
  // Clears LCD only on button press.
  if(switchState == 1 && menu == 0){
    menu = 1;
    lcd.clear();
  } else if(switchState == 1 && menu == 1){
    menu = 2;
    lcd.clear();
  } else if(switchState == 1 && menu == 2){
    menu = 0;
    lcd.clear();
  }

  // This prints the screen contents according to the menu state.
  // NOTE: Could be a SWITCH instead of IF.
  if(menu == 0){
    printTemp();
  } else if (menu == 1){
    printGraph(2, &ten_min, &ten_max, temp_avg_2m);
  } else if (menu == 2){
    printGraph(6, &thirty_min, &thirty_max, temp_avg_6m);
  }

  // Finally increment the array position of temp_avg_10 and 30.
  array_10_1++;
  array_30_1++;
  // END OF LOOP
}

// FUNCTION: Prints default temp screen with 10s/30s averages, a trend indicator and a 60s timer
void printTemp(void){
  // Create chars for LCD
  lcd.createChar(0, degrees);
  lcd.createChar(1, arrow_down);
  lcd.createChar(2, arrow_up);

  // Print "Temp:" and "°C" in first row, "Avg:" in second
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.setCursor(6,0);
  lcd.print(temp, 1);
  lcd.setCursor(0, 1);
  lcd.print(" Avg: ");
  lcd.setCursor(10,0);
  lcd.write(byte(0));
  lcd.setCursor(11,0);
  lcd.print("C");

  // This prints the two average values on the 2nd row of LCD
  if(array_10_1 >= 0){
  lcd.setCursor(6,1);
  lcd.print(temp_avg_10, 1);
  lcd.setCursor(12,1);
  lcd.print(temp_avg_30, 1);
  }

  // This prints the up, down arrow and "-" if the 10s average is below/above
  // the 30s average.
  if(temp_avg_10 < temp_avg_30){
    lcd.setCursor(10,1);
    lcd.write(byte(1));
  } else if(temp_avg_10 > temp_avg_30){
    lcd.setCursor(10,1);
    lcd.write(byte(2));
  } else{
    lcd.setCursor(10,1);
    lcd.write("-");
  }

  // This prints a integer number counter on the top right of the LCD.
  // It counts to 60, then begins at 1 again.
  // Some IFs and setCursors are needed to print blanks in places when
  // the digits switch from 60 to 1 to clear the "6".
  if(counter < 10){
    lcd.setCursor(15,0);
    lcd.print(counter);
    counter++;
  } else if(counter < 61){
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
}

// FUNCTION: Prints graphs with given arguments
void printGraph(const int timeAverage, const float * minNum, const float * maxNum, const float * tempAvgArray){
  // Creating custom chars for percentage bars.
  lcd.createChar(0, graph1);
  lcd.createChar(1, graph2);
  lcd.createChar(2, graph3);
  lcd.createChar(3, graph4);
  lcd.createChar(4, graph5);
  lcd.createChar(5, graph6);
  lcd.createChar(6, graph7);
  lcd.createChar(7, graph8);

  // Printing default text.
  lcd.setCursor(1, 0);
  lcd.print(timeAverage);
  lcd.setCursor(2, 0);
  lcd.print("m:");

  // This neat loop prints the percentage bars.
  // i is used for the LCD position and starts at 4 (4th place on first line).
  // The loop ends, when the end of the first line of the LCD is reached.
  // j is used for the array position of temp_avg_2m.
  // The function calcGraph is called to return an integer between 0 and 7,
  // which is used to print the custom icons from 0 to 7 (graph1 to graph8).
  // Smaller return values print a fuller bar.
  for(i = 4, j = 0; i < 16; i++, j++){
    lcd.setCursor(i, 0);
    lcd.write(byte(calcGraph(minNum, maxNum, &tempAvgArray[j])));
  }

  // Prints 3 values on the second line: lowest avg. number, highest avg. number
  // and the latest avg. number
  lcd.setCursor(0, 1);
  lcd.print(*minNum, 1);

  lcd.setCursor(5, 1);
  lcd.print(*maxNum, 1);

  lcd.setCursor(12, 1);
  lcd.print(tempAvgArray[11], 1);
}

// FUNCTION: Needs the smallest, the biggest and one additional number from the array
// to calculate a percentage, where the additional number lies inbetween min and max.
int calcGraph(const float * minNum, const float * maxNum, const float * number){
  float percent;

  // The percentage is calculated here. Example:
  // Min: 18, Max: 24, latest number: 23
  // 17% = (24-23) * 100 / (24-18)
  // NOTE: Lower percentage means closer to the max value, thus a fuller bar
  // has to be printed.
  percent = ((*maxNum - *number) * 100 / (*maxNum - *minNum));

  // Lower percentage needs a fuller bar, thus returning 7 (all pixels on)
  if((int)percent <= 12)
    return 7;
  if((int)percent > 12 && (int)percent <= 25)
    return 6;
  if((int)percent > 25 && (int)percent <= 37)
    return 5;
  if((int)percent > 37 && (int)percent <= 50)
    return 4;
  if((int)percent > 50 && (int)percent <= 62)
    return 3;
  if((int)percent > 62 && (int)percent <= 75)
    return 2;
  if((int)percent > 75 && (int)percent <= 87)
    return 1;
  if((int)percent > 87 && (int)percent <= 100)
    return 0;

  // DEFAULT if value doesn't fit within the IFs above.
  return 0;
}

// FUNCTION: Returns the smallest number within a given array and its size.
float calcMin(const float * numArray, const int arraySize){
  float minValue = 1000.0, tempValue = 0.0;

  for(i = 0; i < arraySize; i++){
    tempValue = numArray[i];

    if(tempValue < minValue)
      minValue = tempValue;
  }

  return minValue;
}

// FUNCTION: Returns the biggest number within a given array and its size.
float calcMax(const float * numArray, const int arraySize){
  float maxValue = -100.0, tempValue = 0.0;

  for(i = 0; i < arraySize; i++){
    tempValue = numArray[i];

    if(tempValue > maxValue)
      maxValue = tempValue;
  }

  return maxValue;
}

// FUNCTION: Returns the average value within a given array and its size.
float calcAvg(const float * numArray, const int arraySize){
  float tempValue = 0;

  for(i = 0; i < arraySize; i++){
    tempValue += numArray[i];
  }

  return tempValue / arraySize;
}

// FUNCTION: Shifts an array of given size to the left by one.
// Puts a "0.0" in the last field (rightmost).
void shiftAvg(float * numArray, const int arraySize){
  for(i = 0; i < arraySize; i++){
    if(i < (arraySize - 1)){
      numArray[i] = numArray[i + 1];
    } else {
      numArray[arraySize - 1] = 0.0;
    }
  }
}
