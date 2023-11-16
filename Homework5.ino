// Define connections to the shift register
const int latchPin = 11; // Connects to STCP (latch pin) on the shift register
const int clockPin = 10; // Connects to SHCP (clock pin) on the shift register
const int dataPin = 12; // Connects to DS (data pin) on the shift register

// Define connections to the digit control pins for a 4-digit display
const int segD1 = 4;
const int segD2 = 5;
const int segD3 = 6;
const int segD4 = 7;

bool counting = false;
bool lapsPause = false;

const unsigned long debounceDelay = 50;
const int startStopButtonPin = 3;
byte readingButtonStartStop = LOW;
byte startStopButtonPressed = LOW;
byte startStopButtonState = HIGH;
byte lastStartStopButtonState = LOW;
unsigned long lastStartStopDebounceTime= 0;

const int saveLapButtonPin = 2;
byte readingButtonSaveLap = LOW;
byte saveLapButtonPressed = LOW;
byte saveLapButtonState = HIGH;
byte lastSaveLapButtonState = LOW;
unsigned long lastSaveLapDebounceTime= 0;

const int resetButtonPin = 8;
byte readingButtonReset = LOW;
byte resetButtonPressed = LOW;
byte resetButtonState = HIGH;
byte lastResetButtonState = LOW;
unsigned long lastResetDebounceTime= 0;

int lapsSaved[4];
int laps = 0;

// Store the digits in an array for easy access
int displayDigits[] = {segD1, segD2, segD3, segD4};
const int displayCount = 4; // Number of digits in the display

// Define the number of unique encodings (0-9, A-F for hexadecimal)
const int encodingsNumber = 16;
// Define byte encodings for the hexadecimal characters 0-F
byte byteEncodings[encodingsNumber] = {
//A B C D E F G DP 
  B11111100, // 0 
  B01100000, // 1
  B11011010, // 2
  B11110010, // 3
  B01100110, // 4
  B10110110, // 5
  B10111110, // 6
  B11100000, // 7
  B11111110, // 8
  B11110110, // 9
  B11101110, // A
  B00111110, // b
  B10011100, // C
  B01111010, // d
  B10011110, // E
  B10001110  // F
};

unsigned long lastIncrement = 0;
unsigned long delayCount = 50;
unsigned long number = 0;

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  pinMode(startStopButtonPin, INPUT_PULLUP);
  pinMode(saveLapButtonPin, INPUT_PULLUP);
  pinMode(resetButtonPin, INPUT_PULLUP);

  for (int i = 0; i < displayCount; i++) {
    pinMode(displayDigits[i], OUTPUT);
    digitalWrite(displayDigits[i], LOW);
  }  

  Serial.begin(9600);
}

int index;
int lapsStored = 0;

void loop() {

  startStopDebounce();
  saveLapDebounce();
  resetDebounce();

  startStop();
  saveLap();
  reset();

  if (counting){
    if (millis() - lastIncrement > delayCount) {
      number++;
      number %= 10000;
      lastIncrement = millis();
    }
    writeNumber(number);
  }else {
    if(lapsPause){
      writeNumber(lapsSaved[index]);
    }else{
      writeNumber(number);
    }
  }
} 

void startStop(){
    if(startStopButtonPressed){
      counting = !counting;
      startStopButtonPressed = 0;
    if(lapsPause){
      lapsPause = 0;
    }
  }
}

void saveLap(){
  if(saveLapButtonPressed ){
    if(counting){
      if (laps <= 3){
        lapsSaved[laps] = number;
        laps++;
      }else{
        laps = 0;
        lapsSaved[laps] = number;
        laps++;
      }
      lapsStored ++;
      
    }else{
        if(!lapsPause){
          index = 0;
          lapsPause = 1;
        }else{
          if(index < lapsStored && index < 3 ){
            index++;
          }else{
            index = 0;
          }
        }

    }
    saveLapButtonPressed = 0;
  }
}

void reset(){
    if(resetButtonPressed){
    if (!counting){
      if(lapsPause){
        for(int i = 0; i< 4; i++){
          lapsSaved[i] = 0;
        }
        index = 0;
      }else{
        number = 0;
      }
    }
    resetButtonPressed = 0;
  }
}

void resetDebounce(){
  readingButtonReset = digitalRead(resetButtonPin);
  if (readingButtonReset != lastResetButtonState) {
    lastResetDebounceTime = millis();
  }

  if ((millis() - lastResetDebounceTime) > debounceDelay) {
    if (readingButtonReset != resetButtonState) {
      resetButtonState = readingButtonReset;
      if (resetButtonState == LOW) {
          resetButtonPressed = 1;  
      }
    }
  }

  lastResetButtonState = readingButtonReset;

}

void saveLapDebounce(){

  readingButtonSaveLap = digitalRead(saveLapButtonPin);
  if (readingButtonSaveLap != lastSaveLapButtonState) {
    lastSaveLapDebounceTime = millis();
  }

  if ((millis() - lastSaveLapDebounceTime) > debounceDelay) {
    if (readingButtonSaveLap != saveLapButtonState) {
      saveLapButtonState = readingButtonSaveLap;
      if (saveLapButtonState == LOW) {
          saveLapButtonPressed = 1;
          
      }
    }
  }

  lastSaveLapButtonState = readingButtonSaveLap;
}

void startStopDebounce(){

  readingButtonStartStop = digitalRead(startStopButtonPin);
  if (readingButtonStartStop != lastStartStopButtonState) {
    lastStartStopDebounceTime = millis();
  }

  if ((millis() - lastStartStopDebounceTime) > debounceDelay) {
    if (readingButtonStartStop != startStopButtonState) {
      startStopButtonState = readingButtonStartStop;
      if (startStopButtonState == LOW) {
          startStopButtonPressed = 1;

      }
    }
  }
  lastStartStopButtonState = readingButtonStartStop;

  
}

void writeReg(int digit) {
    // Prepare to shift data by setting the latch pin low
    digitalWrite(latchPin, LOW);
    // Shift out the byte representing the current digit to the shift register
    shiftOut(dataPin, clockPin, MSBFIRST, digit);
    // Latch the data onto the output pins by setting the latch pin high
    digitalWrite(latchPin, HIGH);
}

void activateDisplay(int displayNumber) {
  // Turn off all digit control pins to avoid ghosting
  for (int i = 0; i < displayCount; i++) {
    digitalWrite(displayDigits[i], HIGH);
  }
  // Turn on the current digit control pin
  digitalWrite(displayDigits[displayNumber], LOW);
}

void writeNumber(int number) {
  int currentNumber = number;
  int displayDigit = 3;
  int lastDigit = 0;

  while (currentNumber != 0) {
    lastDigit = currentNumber % 10;
    activateDisplay(displayDigit);
    if(displayDigit == 2){
      writeReg(byteEncodings[lastDigit] + 1);
    }else{
      writeReg(byteEncodings[lastDigit]);
    }

    displayDigit--;
    currentNumber /= 10;
    
    writeReg(B00000000); // Clear the register to avoid ghosting
  }
  for (int i = displayDigit; i >= 0; i--) {
      activateDisplay(i);
      writeReg(byteEncodings[0]); // Display zero
      delayMicroseconds(200);
      writeReg(B00000000);
    }
}