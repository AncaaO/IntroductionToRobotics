#include "LedControl.h" // need the library

// Pins for Joystick
const int pinSW = 2; // digital pin connected to switch output
const int pinX = A0; // A0 - analog pin connected to X output
const int pinY = A1; // A1 - analog pin connected to Y output

// Pins for LED Matrix
const byte dinPin = 12;
const byte clockPin = 11;
const byte loadPin = 10;

const byte matrixSize = 8;
byte matrixBrightness = 2;
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

LedControl lc = LedControl(dinPin, clockPin, loadPin, 1); //DIN, CLK, LOAD, No. DRIVER

static int currentRow = 0;
static int currentCol = 0;
static int lastRow = 0;
static int lastCol = 0;

// Timing variables for movement updates
const int moveInterval = 250;
unsigned long lastMoved = 0;

long lastPlayerBlink = 0;
const int playerBlinkDelay = 400;
bool showPlayer = false;

// const int bombBlinkDelay = 100;
// const int detonationTime = 3000;
// unsigned long lastBombBlink = 0;
// static int bombRow = 0;
// static int bombCol = 0;

// bool bombPlanted = false;
// bool showBomb = false;

void setup() {
  pinMode(pinSW, INPUT_PULLUP);

  Serial.begin(9600);

  randomSeed(analogRead(A5));  // Seed random number generator
  generateWalls();

  lc.shutdown(0, false); // turn off power saving, enables display
  lc.setIntensity(0, matrixBrightness); // sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear screen

  matrix[currentRow][currentCol] = 1; // Turn on the initial LED position

}

void loop() {
  swState = digitalRead(pinSW);

  // if (swState == HIGH && !bombPlanted) {
  //   plantBomb();
  // }
  // if ((millis() - lastBombBlink >= detonationTime) && bombPlanted) {
  //   detonateBomb();
  // }

  if (millis() - lastMoved > moveInterval) {
    movePlayer();
    lastMoved = millis(); // Reset the movement timer
  }

  blinkPlayer();
//  blinkBomb();

  if (matrixChanged) {
    updateMatrixDisplay();
    matrixChanged = false;
  }

}

bool movePlayer() {
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
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
    matrix[lastRow][lastCol] = 0; // Turn off the LED at the last position
    matrix[currentRow][currentCol] = 1; // Turn on the LED at the new position
  }
}

// void plantBomb() {
//   bombPlanted = true;
//   lastBombBlink = millis();
//   bombRow = currentRow;
//   bombCol = currentCol;
//   matrix[bombRow][bombCol] = 2; // Set the bomb at the player's position

//   matrixChanged = true;
// }

// void detonateBomb() {
//   bombPlanted = false;
//   matrix[bombRow][bombCol] = 0; // Remove the bomb
//   matrixChanged = true;
// }

void generateWalls() {
  byte totalCells = matrixSize * matrixSize;
  byte wallsToGenerate = (totalCells * wallPercentage) / 100;
  
  // Generate walls randomly
  while (wallsToGenerate > 0) {
    byte x = random(0, matrixSize);
    byte y = random(0, matrixSize);
    // Serial.print('X');
    // Serial.println(x);
        
    if ((x >= 2 || y >= 2) && matrix[x][y] == 0) {
      matrix[x][y] = 1;  // 1 represents a wall
      wallsToGenerate--;
    }
  }
}

// void updateMatrixDisplay() {
//   for (int row = 0; row < matrixSize; row++) {
//     for (int col = 0; col < matrixSize; col++) {
//       if (row == currentRow && col == currentCol) {
//         lc.setLed(0, row, col, showPlayer); // Toggle LED at the player's position
//       } else {
//         lc.setLed(0, row, col, matrix[row][col]); // Update other LEDs normally
//       }
//     }
//   }
// }
void updateMatrixDisplay() {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      if (row == currentRow && col == currentCol) {
        lc.setLed(0, row, col, showPlayer); // Toggle LED at the player's position
      // } else if (matrix[row][col] == 2) {
      //   lc.setLed(0, row, col, showBomb); // Show the bomb if the player is not there
      } else {
        lc.setLed(0, row, col, matrix[row][col]); // Update other LEDs normally
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

// void blinkBomb() {
//   if (bombPlanted && (millis() - lastBombBlink >= bombBlinkDelay)) {
//     showBomb = !showBomb;
//     lastBombBlink = millis();
//     matrixChanged = true;
//   }
// }