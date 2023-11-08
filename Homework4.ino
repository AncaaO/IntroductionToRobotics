const int pinA = 12;
const int pinB = 10;
const int pinC = 9;
const int pinD = 8;
const int pinE = 7;
const int pinF = 6;
const int pinG = 5;
const int pinDP = 4;

const int segSize = 8;

const int pinSW = 2; 
const int pinX = A0;
const int pinY = A1;
int xValue = 0;
int yValue = 0;

int segments[segSize] = {
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};

byte segmentsStates[segSize] = {
  LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW
};

int currentSegment = pinDP;
const int noOfDirections = 4;
unsigned long lastMove = 0;

int up = 0;
int down = 1;
int right = 2;
int left = 3;
int move;

unsigned int lastBlink = 0;
const int timeOfBlink = 500;
bool blinkState = 0;

unsigned long timeOfMove = 1000;

const int minTrigger = 100;
const int maxTrigger = 900;

int buttonRead;
int lastButtonState;
unsigned long lastDebounceTime = 0; 
int buttonState;
int buttonPressed;
unsigned long debounceDelay = 50;

unsigned long lastInterruptTime = 0;

int lastXValue = 0;
int lastYValue=0;
int joyMoved = 0;

bool hold = 0;
unsigned long holdState = 0;
unsigned long holdTime= 1000;

int index = 0;

byte segmentsMovements[segSize][noOfDirections] = {
  //Up Dn LT RT
  { 12, 5, 6, 10},  // a  12
  { 12, 5, 6, 10},  // b  10
  { 5, 8, 7, 9},  // c  9
  { 5, 8, 7, 9},  // d  8
  { 5, 8, 7, 9},  // e  7
  { 12, 5, 6, 10},  // f  6
  { 12, 8, 5, 5}, // g  5
  { 4, 4, 9, 4}     // dp4
};

void setup() {
  pinMode(pinSW, INPUT_PULLUP);
  for (int i = 0; i < segSize; i++) {
    pinMode(segments[i], OUTPUT);
  }

  Serial.begin(9600);
}

void loop() {

  Serial.println(buttonPressed);
  buttonDebounce();

  if(segments[index] != currentSegment)
    digitalWrite(segments[index], segmentsStates[index]);
  index= (index + 1) % segSize;

  blink();

  yValue = analogRead(pinX);
  xValue = analogRead(pinY);

  joyMovement();

}

void joyMovement(){
  if ((minTrigger <= xValue && xValue <= maxTrigger) and (minTrigger <= yValue && yValue <= maxTrigger)) {
    joyMoved = false;
  }

  if (xValue < minTrigger && joyMoved == false) {
    move = right;
    joyMoved = true;
  }

  if (xValue > maxTrigger && joyMoved == false) {
    move = left;
    joyMoved = true;
  }

  if (yValue < minTrigger && joyMoved == false) {
    move = down;
    joyMoved = true;
  }

  if (yValue > maxTrigger && joyMoved == false) {
    move = up;
    joyMoved = true;
  }

  if ((millis() >= lastMove + timeOfMove) && (joyMoved == 1))
  {
    currentSegment = segmentsMovements[translatePosition(currentSegment)][move];
    joyMoved = 0;
    lastMove = millis();

  }
  if(buttonPressed){
    segmentsStates[translatePosition(currentSegment)] = !segmentsStates[translatePosition(currentSegment)];
    buttonPressed = 0; 
  }
}

int translatePosition(int position){
  switch(position){
    case 4: return 7;
    case 5: return 6;
    case 6: return 5;
    case 7: return 4;
    case 8: return 3;
    case 9: return 2;
    case 10: return 1;
    case 12: return 0;
  }
}

void blink(){
  if(millis() > lastBlink + timeOfBlink){
      blinkState = !blinkState;
      digitalWrite(currentSegment, blinkState);
      lastBlink = millis();
  }
}

void buttonDebounce(){
  buttonRead = digitalRead(pinSW);
    if (buttonRead != lastButtonState) {
      lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay) {

    if (buttonRead != buttonState) {
      buttonState = buttonRead;
      if (buttonState == LOW) {
        buttonPressed = 1;
        hold = 1;
        holdState = millis();
      }else{
        if(hold == 1){
          hold = 0;
        }
      }
    }
  }
  
  if(hold == 1 && holdState + holdTime <= millis()){
    hold = 0;
    for (int i = 0; i < segSize; i++){
      segmentsStates[i] = LOW;
    }
    currentSegment = pinDP;
  }

  lastButtonState = buttonRead;
}
