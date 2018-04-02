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
  // Starting display and setting contrast to lowest setting.
  display.begin();
  display.setContrast(0);
  // Getting random seed from A2 analog pin which is used for the random() below.
  randomSeed(analogRead(2));
  ballX = random(40, 88);
  ballY = random(3, 59);
  randomSeed(analogRead(2));
  // The horizontal direction in which the ball moves: 0 = to the right, 1 = to the left
  leftRight = random(2);
  delay(50);
  randomSeed(analogRead(3));
  // The vertical direction in which the ball moves: 0 = moving up, 1 = moving down
  upDown = random(2);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);

  // Debug
  //Serial.begin(9600);
}

void loop() {
  // Drawing the game on screen.
    display.clearBuffer();
    display.drawBox(ballX, ballY, 2, 2);
    display.drawBox(0, player1y, 2, barSize);
    display.drawBox(126, player2y, 2, barSize);
    display.sendBuffer();

// Buttons for player 1 (digital pin 2 and 3)
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

// Buttons for player 2 (digital pin 4 and 5)
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

// Game start, initial movement physics of ball
    if (wall == 4 && leftRight == 0 && upDown == 0) {
      ballX += ballSpeed;
      ballY -= ballSpeed;
    }

    if (wall == 4 && leftRight == 0 && upDown == 1) {
      ballX += ballSpeed;
      ballY += ballSpeed;
    }

    if (wall == 4 && leftRight == 1 && upDown == 0) {
      ballX -= ballSpeed;
      ballY -= ballSpeed;
    }

    if (wall == 4 && leftRight == 1 && upDown == 1) {
      ballX -= ballSpeed;
      ballY += ballSpeed;
    }

// Ball physics after hitting one of the borders.
    // Hitting left wall while coming from the botton moving upwards.
    if (wall == 0 && upDown == 0) {
    leftRight = 0;
    ballX += ballSpeed;
    ballY -= ballSpeed;
    }
  
    // Hitting the left wall while coming from the top moving downwards.
    if (wall == 0 && upDown == 1) {
    leftRight = 0;
    ballX += ballSpeed;
    ballY += ballSpeed;
    }

    // Hitting right wall, coming from the bottom and moving upward.
    if (wall == 2 && upDown == 0) {
      leftRight = 1;
      ballX -= ballSpeed;
      ballY -= ballSpeed;
    }

    // Hitting right wall, coming from the top moving downward.
    if (wall == 2 && upDown == 1) {
      leftRight = 1;
      ballX -= ballSpeed;
      ballY += ballSpeed;
    }

    // Hitting bottom wall while coming from the right moving leftward.
    if (wall == 1 && leftRight == 0) {
      upDown = 0;
      ballX += ballSpeed;
      ballY -= ballSpeed;
    }

    // Hitting bottom wall, coming from the left moving rightward.
    if (wall == 1 && leftRight == 1) {
      upDown = 0;
      ballX -= ballSpeed;
      ballY -= ballSpeed;
    }

    // Hitting top wall, coming from the right moving leftward.
    if (wall == 3 && leftRight == 0) {
      upDown = 1;
      ballX += ballSpeed;
      ballY += ballSpeed;
    }

    // Hitting top wall, coming from the left moving rightward.
    if (wall == 3 && leftRight == 1) {
      upDown = 1;
      ballX -= ballSpeed;
      ballY += ballSpeed;
    }
  
    // If ball hits the bottom - aka 63rd Y-pixel of the screen, it sets the
    // wall-1 flag, which changes ball movement
    if (ballY > 63) {
      wall = 1;
    }

    // If ball hits the top - 0th Y-pixel of the screen, it sets the wall-3 flag,
    // indicating that it came from top screen.
    if (ballY < 0) {
      wall = 3;
    }

    // Ball hits player 2 wall to the right
    if (ballX > 126) {
      // If-statement to narrow down the hitbox to check if the ball
      // hit the bar of the player.
      if (ballY >= player2y && ballY <= (player2y + barSize)) {
        // On success - hitting the bar, it sets the wall-2 flag,
        // indicating that it hit from the right wall
        wall = 2;
        // On failure to hit the bar:
      } else {
        // Player 1 loses, player 2 wins
        display.clearBuffer();
        display.setFont(u8g2_font_profont17_tf);
        display.setFontDirection(1);
        display.drawStr(12, 22, "Win");
        display.setFontdection(3);
        display.drawStr(116, 50, "Lose");
        display.sendBuffer();

        // Creates a new randomly generated game.
        createNewGame();

        // Waits 3 seconds until the game starts.
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

// Creates new game, randomizing the ball spawn point and flight direction.
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
