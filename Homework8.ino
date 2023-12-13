#include "LedControl.h" 
#include <LiquidCrystal.h>
#include <EEPROM.h>

// Pins for Joystick
const int pinSW = 2; // digital pin connected to switch output
const int pinX = A0; // A0 - analog pin connected to X output
const int pinY = A1; // A1 - analog pin connected to Y output

// Pins for LED Matrix
const byte dinPin = 12;
const byte clockPin = 11;
const byte loadPin = 10;

// Pin for button
const int startButtonPin = 13;

LedControl lc = LedControl(dinPin, clockPin, loadPin, 1); //DIN, CLK, LOAD, No. DRIVER

// Matrix variables
const byte matrixSize = 8;
// byte matrixBrightness = 2;
bool matrixChanged = true;
const int wallPercentage = 50;  // Percentage of walls generated on the matrix
int matrix[matrixSize][matrixSize] = {};

// Constants for LED movement based on joystick values
const int thresholdLow = 300;
const int thresholdHigh = 800;

// Variables for joystick
int xValue = 0;
int yValue = 0;
int swState;
bool joyMoved = false;
bool joyMovedY = false, joyMovedX = false;
//bool lastSwState = false;

// Timing variables for movement updates
const int moveInterval = 250;
unsigned long lastMoved = 0;

// Player position variables
static int currentRow = 0;
static int currentCol = 0;
static int lastRow = 0;
static int lastCol = 0;

// Player blink variables
long lastPlayerBlink = 0;
const int playerBlinkDelay = 400;
bool showPlayer = false;
bool lastSwState = false;

// Bomb variables
const int bombBlinkDelay = 100;
unsigned long lastBombBlink = 0;
const int detonationTime = 3000;
unsigned long bombPlantTime = 0;
static int bombRow = 0;
static int bombCol = 0;
bool bombPlanted = false;
bool showBomb = false;

// LCD
const byte rs = 9;
const byte en = 8;
const byte d4 = 7;
const byte d5 = 6;
const byte d6 = 5;
const byte d7 = 4;
const byte v0 = 3;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const byte leftArrow[8] = {
  B00000,
  B00100,
  B01000,
  B11111,
  B01000,
  B00100,
  B00000,
  B00000
};

const byte rightArrow[8] = {
  B00000,
  B00100,
  B00010,
  B11111,
  B00010,
  B00100,
  B00000,
  B00000
};

const byte upAndDownArrows[8] = {
  B00100,
  B01110,
  B10101,
  B00100,
  B00100,
  B10101,
  B01110,
  B00100
};

const byte downArrow[8] = {
  B00000,
  B00000,
  B00100,
  B00100,
  B10101,
  B01110,
  B00100,
  B00000
};

const byte upArrow[8] = {
  B00000,
  B00100,
  B01110,
  B10101,
  B00100,
  B00100,
  B00000,
  B00000
};

enum menuStates {
  START_GAME, SETTINGS, ABOUT, LCD_BRIGHTNESS, MATRIX_BRIGHTNESS, PLAYING, GAME_OVER
} menuState;

// Start button variables
byte startButtonState = HIGH; //pressed
byte startButtonReading = HIGH; //not pressed
byte lastStartButtonReading = HIGH; //not pressed

unsigned int lastDebounceTime = 0;
unsigned int debounceDelay = 50;

// About menu variables
unsigned long lastAboutTime = 0;
const int githubDelay = 2000;
const int gameDetailsDelay = 1000;

// LCD brightness control
int lcdBrightnessValue[5] = { 10, 40, 70, 100, 130 };
int currentLcdBrightnessVal = 0;
const byte firstBrightnessVal = 0;
const byte lastBrightnessVal = 4;

int matrixBrightnessValue[5] = { 1, 5, 8, 11, 15 };
int currentMatrixBrightnessVal = 0;

void setup() { 
  pinMode(pinSW, INPUT_PULLUP);
  pinMode(startButtonPin, INPUT_PULLUP);
  Serial.begin(9600);

  analogWrite(v0, 60);

  lcd.begin(16, 2);
  lcd.createChar(0, leftArrow);
  lcd.createChar(1, rightArrow);
  lcd.createChar(2, upArrow);
  lcd.createChar(3, downArrow);
  lcd.createChar(4, upAndDownArrows);

  introMessage();

  menuState = START_GAME;

  lc.shutdown(0, false); // turn off power saving, enables display
  lc.setIntensity(0, 5); // sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear screen

}

void loop() {
  swState = digitalRead(pinSW);
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
  startButtonReading = digitalRead(startButtonPin);

  switch (menuState) {
    case START_GAME:
      Serial.println("start game");
      lcd.setCursor(0, 0);
      lcd.write(byte(0));
      centerLcdText(F("START GAME"), 0);
      lcd.setCursor(15, 0);
      lcd.write(byte(1));
      centerLcdText(F("Push btn to play"), 1);
      showPlayer = showBomb = false;
      handleMenuChoice();
      break;
    case ABOUT:
      lcd.setCursor(0, 0);
      if (millis() - lastAboutTime < gameDetailsDelay) {
        centerLcdText(F("ABOUT"), 0);
        centerLcdText(F("PIXEL BOOM"), 1);
      } else if (millis() - lastAboutTime > gameDetailsDelay && millis() - lastAboutTime < gameDetailsDelay + githubDelay) {
        centerLcdText(F("By Anca Oprea"), 0);
        centerLcdText(F("github: AncaaO"), 1);
      } else {
        lastAboutTime = millis();
        lcd.clear();
      }
      handleMenuChoice();
      break;
    case PLAYING: 
      lcd.setCursor(0, 0);
      centerLcdText(F("PLAYING"), 0);
      handleGame();
      handleMenuChoice();
      break;
    case SETTINGS:
      lcd.setCursor(0, 0);
      lcd.write(byte(0));
      centerLcdText(F("SETTINGS"), 0);
      lcd.setCursor(15, 0);
      lcd.write(byte(1));
      centerLcdText(F("Push btn-adjust"), 1);
      handleMenuChoice();
      break;
    case LCD_BRIGHTNESS:
      lcdBrightness();
      turnOffMatrix();
      handleMenuChoice();
      break;
    case MATRIX_BRIGHTNESS:
      matrixBrightness();
      handleMenuChoice();
      break;
  }
}

void handleMenuChoice(){
  if (xValue < thresholdLow && joyMovedX == false) {
    switch(menuState) {
      case LCD_BRIGHTNESS:
        currentLcdBrightnessVal++;
        if (currentLcdBrightnessVal > lastBrightnessVal) {
          currentLcdBrightnessVal = lastBrightnessVal;
        }
        lcdBrightness();
        break;
      case MATRIX_BRIGHTNESS:
        currentMatrixBrightnessVal--;
        if (currentMatrixBrightnessVal > lastBrightnessVal) {
          currentMatrixBrightnessVal = lastBrightnessVal;
        }
        matrixBrightness();
        break;
    }
    joyMovedX = true;
  }

  if (xValue > thresholdHigh && joyMovedX == false) {
    switch(menuState) {
      case LCD_BRIGHTNESS:
        currentLcdBrightnessVal--;
        if (currentLcdBrightnessVal < firstBrightnessVal) {
          currentLcdBrightnessVal = firstBrightnessVal;
        }
        lcdBrightness();
        break;
      case MATRIX_BRIGHTNESS:
        currentMatrixBrightnessVal++;
        if (currentMatrixBrightnessVal < firstBrightnessVal) {
          currentMatrixBrightnessVal = firstBrightnessVal;
        }
        matrixBrightness();
        break;
    }
    joyMovedX = true;
  }

  if (xValue >= thresholdLow && xValue <= thresholdHigh) {
    joyMovedX = false;
  }

  if (yValue < thresholdLow && joyMovedY == false) {
    switch (menuState) {
      case START_GAME:
        navigateMenu(false);
        break;
      case SETTINGS:
        navigateMenu(false);
        break;
      case ABOUT:
        navigateMenu(false);
        break;
      case LCD_BRIGHTNESS:
        navigateSettingsSubmenu(false);
        break;
      case MATRIX_BRIGHTNESS:
        navigateSettingsSubmenu(false);
        break;

    }
    joyMovedY = true;
  }

  if (yValue > thresholdHigh && joyMovedY == false) {
    switch (menuState) {
      case START_GAME:
        navigateMenu(true);
        break;
      case SETTINGS:
        navigateMenu(true);
        break;
      case ABOUT:
        navigateMenu(true);
        break;
      case LCD_BRIGHTNESS:
        navigateSettingsSubmenu(true);
        break;
      case MATRIX_BRIGHTNESS:
        navigateSettingsSubmenu(true);
        break;
    }
    joyMovedY = true;
  }

  if (yValue >= thresholdLow && yValue <= thresholdHigh) {
    joyMovedY = false;
  }

  if (startButtonReading != lastStartButtonReading) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay){
    if (startButtonReading !=  startButtonState) {
      startButtonState = startButtonReading;

      if (startButtonState == LOW) {
        switch (menuState) {
          case START_GAME:
            menuState = PLAYING;
            startGame();
            lcd.clear();
            break;
          case SETTINGS:
            menuState = LCD_BRIGHTNESS;
            lcd.clear();
            break;
          case LCD_BRIGHTNESS:
            menuState = SETTINGS;
            lcd.clear();
            break;
          case MATRIX_BRIGHTNESS:
            menuState = SETTINGS;
            turnOffMatrix();
            lcd.clear();
            break;
          case PLAYING:
            menuState = START_GAME;
            lcd.clear();
            turnOffMatrix();
            break;
        }
      }
    }
  }
  lastStartButtonReading = startButtonReading;
}

void navigateMenu(bool right) {
  if (right) {
    if (menuState == 2) {
      menuState = 0;
    } else {
      menuState = menuState + 1;
    }
  } else {
    if (menuState == 0) {
      menuState = 2;
    } else {
      menuState = menuState - 1;
    }
  }
  lcd.clear();
}

void navigateSettingsSubmenu(bool right) {
  if (right) {
    if (menuState == 3) {
      menuState = 4;
    } else if (menuState == 4) {
      menuState = 3;
    }
  } else {
    if (menuState == 3) {
        menuState = 4;
    } else if (menuState == 4){
      menuState = 3;
    }
  }
  lcd.clear();
}


void introMessage(){
  centerLcdText(F("WELCOME TO"), 0);
  centerLcdText(F("PIXEL BOOM"), 1);
  delay(1000);
  lcd.clear();
  centerLcdText(F("READY TO FIGHT"), 0);
  centerLcdText(F("THE WALLS?"), 1);  
  delay(1000);
  lcd.clear();
}

void lcdBrightness() {
  lcd.setCursor(0, 0);
  lcd.print("LCD Brightness");

  analogWrite(v0, lcdBrightnessValue[currentLcdBrightnessVal]);

  lcd.setCursor(15, 0);
  if (currentLcdBrightnessVal == 0) {
    lcd.write(byte(3));
  } else if (currentLcdBrightnessVal == 1) {
    lcd.write(byte(4));
  } else if (currentLcdBrightnessVal == 2) {
    lcd.write(byte(4));
  } else if (currentLcdBrightnessVal == 3) {
    lcd.write(byte(4));
  } else if (currentLcdBrightnessVal == 4) {
    lcd.write(byte(2));
  }

  lcd.setCursor(0, 1);
  lcd.write(byte(0));
  centerLcdText(F("Push btn-exit"), 1); 
  lcd.setCursor(15, 1);
  lcd.write(byte(1));
}

void matrixBrightness() {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, true); // turns off LED at col, row
    }
  }
  lcd.setCursor(0, 0);
  lcd.print("Matrix Bright.");

  lc.setIntensity(0, matrixBrightnessValue[currentMatrixBrightnessVal]);

  lcd.setCursor(15, 0);
  if (currentMatrixBrightnessVal == 0) {
    lcd.write(byte(3));
  } else if (currentMatrixBrightnessVal == 1) {
    lcd.write(byte(4));
  } else if (currentMatrixBrightnessVal == 2) {
    lcd.write(byte(4));
  } else if (currentMatrixBrightnessVal == 3) {
    lcd.write(byte(4));
  } else if (currentMatrixBrightnessVal == 4) {
    lcd.write(byte(2));
  }

  lcd.setCursor(0, 1);
  lcd.write(byte(0));
  centerLcdText(F("Push btn-exit"), 1); 
  lcd.setCursor(15, 1);
  lcd.write(byte(1));
}

void startGame(){
  randomSeed(analogRead(A5));  // Seed random number generator
  generateWalls();
  currentRow = 0;
  currentCol = 0;
  bombPlanted = false;
  matrix[currentRow][currentCol] = 1; // Turn on the initial LED position
}

void handleGame() {
  if (swState == LOW && !bombPlanted) {
    plantBomb();
  }

  if ((millis() - bombPlantTime >= detonationTime) && bombPlanted) {
    detonateBomb(); 
  }

  if (millis() - lastMoved > moveInterval) {
    movePlayer();
    lastMoved = millis(); // Reset the movement timer
  }

  blinkPlayer();
  blinkBomb();

  if (matrixChanged) {
    updateMatrixDisplay();
    matrixChanged = false;
  }
}

void movePlayer() {
  lastRow = currentRow;
  lastCol = currentCol;

  // Move if within bounds and not a wall
  if (xValue < thresholdLow && currentRow < matrixSize - 1) {
    if (matrix[currentRow + 1][currentCol] != 1) {
      currentRow++; 
    }
  } else if (xValue > thresholdHigh && currentRow > 0) {
    if (matrix[currentRow - 1][currentCol] != 1) {
      currentRow--;
    }
  } else if (yValue < thresholdLow && currentCol > 0) {
    if (matrix[currentRow][currentCol - 1] != 1) {
      currentCol--;
    }
  } else if (yValue > thresholdHigh && currentCol < matrixSize - 1) {
    if (matrix[currentRow][currentCol + 1] != 1) {
      currentCol++;
    }
  }

  if (currentRow != lastRow || currentCol != lastCol) {
    matrixChanged = true;
    if (matrix[lastRow][lastCol] != 2) {
      matrix[lastRow][lastCol] = 0; // Turn off the LED at the last position
    }
    matrix[currentRow][currentCol] = 1; // Turn on the LED at the new position
  }
}

void plantBomb() {
  bombPlanted = true;
  bombPlantTime = millis();
  lastBombBlink = millis();
  bombRow = currentRow;
  bombCol = currentCol;
  matrix[bombRow][bombCol] = 2; // Set the bomb at the player's position
  matrixChanged = true;
}

void detonateBomb() {
  bombPlanted = false;
  matrix[bombRow][bombCol] = 0; // Remove the bomb
  if (bombRow > 0) {
    matrix[bombRow - 1][bombCol] = 0; // Clear above
  }
  if (bombCol > 0) {
    matrix[bombRow][bombCol - 1] = 0; // Clear to the left
  }
  if (bombRow < matrixSize - 1) {
    matrix[bombRow + 1][bombCol] = 0; // Clear below
  }
  if (bombCol < matrixSize - 1) {
    matrix[bombRow][bombCol + 1] = 0; // Clear to the right
  }
  matrixChanged = true;
}

void generateWalls() {
  byte totalCells = matrixSize * matrixSize;
  byte wallsToGenerate = (totalCells * wallPercentage) / 100;
  
  while (wallsToGenerate > 0) {
    byte x = random(0, matrixSize);
    byte y = random(0, matrixSize);
        
    if ((x >= 2 || y >= 2) && matrix[x][y] == 0) {
      matrix[x][y] = 1;  // 1 represents a wall
      wallsToGenerate--;
    }
  }
}

void updateMatrixDisplay() {
  if(menuState == PLAYING) {
    for (int row = 0; row < matrixSize; row++) {
      for (int col = 0; col < matrixSize; col++) {
        if (matrix[row][col] == 2) {
          lc.setLed(0, row, col, showBomb); // Toggle bomb at the bomb's position
        } else if (row == currentRow && col == currentCol) {
          lc.setLed(0, row, col, showPlayer); // Toggle LED at the player's position
        } else {
          lc.setLed(0, row, col, matrix[row][col]); // Update other LEDs normally
        }
      }
    }
  }
}

void blinkPlayer() {
  if (millis() - lastPlayerBlink >= playerBlinkDelay) {
    matrixChanged = true;
    showPlayer = !showPlayer;
    lastPlayerBlink = millis();
  }
}

void blinkBomb() {
  if (bombPlanted && (millis() - lastBombBlink >= bombBlinkDelay)) {
    showBomb = !showBomb;
    lastBombBlink = millis();
    matrixChanged = true;
  }
}

bool gameOver(){
  bool wallsDetected = false;

  // Check for remaining walls
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      if (matrix[row][col] == 1 && !(row == currentRow && col == currentCol)) {
        wallsDetected = true;
        break;
      }
    }
    if (wallsDetected) {
      break;
    }
  }

  // Check if the player is near the bomb
  bool playerNearBomb = ((currentRow == bombRow && currentCol == bombCol) ||
                        (currentRow == bombRow - 1 && currentCol == bombCol) || // Above
                        (currentRow == bombRow + 1 && currentCol == bombCol) || // Below
                        (currentRow == bombRow && currentCol == bombCol - 1) || // Left
                        (currentRow == bombRow && currentCol == bombCol + 1)); // Right

  // Return true if the matrix is empty or the player is near the bomb
  return !wallsDetected || playerNearBomb;
}

void turnOffMatrix(){
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, false); // turns off LED at col, row
      matrix[row][col] = 0;
    }
  }
}

// Centre LCD text
void centerLcdText (String msg, short line) {
  short offset = (16 - msg.length())/2;
  lcd.setCursor(offset, line);
  lcd.print(msg);
}

