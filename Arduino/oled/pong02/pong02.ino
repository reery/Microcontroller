#include <U8g2lib.h>
#include <Wire.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C display(U8G2_R0,/* reset=*/ U8X8_PIN_NONE);

int ballX;
int ballY;
uint8_t wall = 4;
int dir;
uint8_t initialize = 0;
uint8_t ballSpeed = 2;
int upDown;
int8_t player1y = 30;
int8_t player2y = 30;
uint8_t barSize = 14;
uint8_t player1buttonL = 0;
uint8_t player1buttonR = 0;
uint8_t player2buttonL = 0;
uint8_t player2buttonR = 0;
uint8_t barSpeed = 1;

void setup() {
  display.begin();
  display.setContrast(0);
  randomSeed(analogRead(2));
  ballX = random(40, 88);
  ballY = random(3, 59);
  // 0 = to the right, 1 = to the left
  randomSeed(analogRead(2));
  dir = random(2);
  // 0 = moving up, 1 = moving down
  delay(50);
  randomSeed(analogRead(3));
  upDown = random(2);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);

  // Debug
  //Serial.begin(9600);
}

void loop() {
    display.clearBuffer();
    display.drawBox(ballX, ballY, 2, 2);
    display.drawBox(0, player1y, 2, barSize);
    display.drawBox(126, player2y, 2, barSize);
    display.sendBuffer();

// Buttons player 1 (digital pin 2 and 3)
    if (player1y < 0) {
      player1y = 0;
    }

    if (player1y > (64 - barSize)) {
      player1y = 64 - barSize;
    }

    if (player1y >= 0 && player1y <= (64 - barSize)) {
      player1buttonL = digitalRead(2);
      player1buttonR = digitalRead(3);

      if (player1buttonL == HIGH && player1buttonR == LOW) {
        player1y -= barSpeed;
      }

      if (player1buttonR == HIGH && player1buttonL == LOW) {
        player1y += barSpeed;
      }
    }

// Buttons player 2 (digital pin 4 and 5)
    if (player2y < 0) {
      player2y = 0;
    }

    if (player2y > (64 - barSize)) {
      player2y = 64 - barSize;
    }

    if (player2y >= 0 && player2y <= (64 - barSize)) {
      player2buttonL = digitalRead(4);
      player2buttonR = digitalRead(5);

      if (player2buttonL == HIGH && player2buttonR == LOW) {
        player2y += barSpeed;
      }

      if (player2buttonR == HIGH && player2buttonL == LOW) {
        player2y -= barSpeed;
      }
    }

// Start
    if (wall == 4 && dir == 0 && upDown == 0) {
      ballX += ballSpeed;
      ballY -= ballSpeed;
    }

    if (wall == 4 && dir == 0 && upDown == 1) {
      ballX += ballSpeed;
      ballY += ballSpeed;
    }

    if (wall == 4 && dir == 1 && upDown == 0) {
      ballX -= ballSpeed;
      ballY -= ballSpeed;
    }

    if (wall == 4 && dir == 1 && upDown == 1) {
      ballX -= ballSpeed;
      ballY += ballSpeed;
    }

// Ball physics
    // Hitting left wall
    if (wall == 0 && upDown == 0) {
    dir = 0;
    ballX += ballSpeed;
    ballY -= ballSpeed;
    }

    if (wall == 0 && upDown == 1) {
    dir = 0;
    ballX += ballSpeed;
    ballY += ballSpeed;
    }

    // Hitting right wall
    if (wall == 2 && upDown == 0) {
      dir = 1;
      ballX -= ballSpeed;
      ballY -= ballSpeed;
    }

    if (wall == 2 && upDown == 1) {
      dir = 1;
      ballX -= ballSpeed;
      ballY += ballSpeed;
    }

    // Hitting bottom wall
    if (wall == 1 && dir == 0) {
      upDown = 0;
      ballX += ballSpeed;
      ballY -= ballSpeed;
    }

    if (wall == 1 && dir == 1) {
      upDown = 0;
      ballX -= ballSpeed;
      ballY -= ballSpeed;
    }

    // Hitting top wall
    if (wall == 3 && dir == 0) {
      upDown = 1;
      ballX += ballSpeed;
      ballY += ballSpeed;
    }

    if (wall == 3 && dir == 1) {
      upDown = 1;
      ballX -= ballSpeed;
      ballY += ballSpeed;
    }

    if (ballY > 63) {
      wall = 1;
    }

    if (ballY < 0) {
      wall = 3;
    }

    // Ball hits player 2 wall to the right
    if (ballX > 126) {
      if (ballY >= player2y && ballY <= (player2y + barSize)) {
        wall = 2;
      } else {
        // Player 1 loses, player 2 wins
        display.clearBuffer();
        display.setFont(u8g2_font_profont17_tf);
        display.setFontDirection(1);
        display.drawStr(12, 22, "Win");
        display.setFontDirection(3);
        display.drawStr(116, 50, "Lose");
        display.sendBuffer();

        createNewGame();

        delay(3000);
      }
    }

    // Ball hits player 1 wall to the left
    if (ballX < 1) {
      if (ballY >= player1y && ballY <= (player1y + barSize)) {
        wall = 0;
      } else {
        // Player 2 loses, player 1 wins
        display.clearBuffer();
        display.setFont(u8g2_font_profont17_tf);
        display.setFontDirection(1);
        display.drawStr(12, 16, "Lose");
        display.setFontDirection(3);
        display.drawStr(116, 46, "Win");
        display.sendBuffer();

        createNewGame();

        delay(3000);
      }
    }
}

void createNewGame(void) {
  randomSeed(analogRead(2));
  ballX = random(40, 88);
  ballY = random(3, 59);
  delay(50);
  randomSeed(analogRead(2));
  dir = random(2);
  delay(50);
  randomSeed(analogRead(3));
  upDown = random(2);
  wall = 4;
  player1y = 30;
  player2y = 30;
}
