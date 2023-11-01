const int groundFloor = 0;
const int topFloor = 2;

const unsigned long debounceDelay = 50;
const int movingTime = 2000;

const unsigned long flickerDelay = 200;
const int flickerLED = 9;
unsigned long lastFlicker = 0;
bool flickerState = 0;

const int buzzerPin = 10;

const int buttonFloor[3] = {4, 3, 2};
const int LEDFloor[3] = {13, 12, 11};
byte readingFloor[3] = {LOW, LOW, LOW};
unsigned int lastDebounceTimeFloor[3] = {0, 0, 0};
byte buttonPressedFloor[3] = {LOW, LOW, LOW};
byte buttonStateFloor[3] = {LOW, LOW, LOW};
unsigned long lastButtonStateFloor[3]= {0, 0 , 0};

int elevator = groundFloor;

int state = 0;
unsigned long movingStartTime = 0;

void setup() {
  for(int i=0; i<3; i++){
    pinMode(buttonFloor[i], INPUT_PULLUP);
    pinMode(LEDFloor[i], OUTPUT);
  }
  digitalWrite(LEDFloor[elevator], HIGH);
  pinMode(flickerLED, OUTPUT);
  Serial.begin(9600);
}

void loop() {

// to make the LED flicker
  if( state == 1 || state == 2){ //elevator is moving
    if(millis() > lastFlicker + flickerDelay){
      flickerState= !flickerState;
      digitalWrite(flickerLED, flickerState);
      lastFlicker = millis();
    }
  }
// debounce 
  readingFloor[0] = digitalRead(buttonFloor[0]);
  if (readingFloor[0] != lastButtonStateFloor[0]) {
    lastDebounceTimeFloor[0] = millis();
  }

  if ((millis() - lastDebounceTimeFloor[0]) > debounceDelay) {
    if (readingFloor[0] != buttonStateFloor[0]) {
      buttonStateFloor[0] = readingFloor[0];
      if (buttonStateFloor[0] == LOW) {
          buttonPressedFloor[0] = 1;
      }
    }
  }
  lastButtonStateFloor[0] = readingFloor[0];
  
  readingFloor[1] = digitalRead(buttonFloor[1]);
  if (readingFloor[1] != lastButtonStateFloor[1]) {
    lastDebounceTimeFloor[1] = millis();
  }

  if ((millis() - lastDebounceTimeFloor[1]) > debounceDelay) {
    if (readingFloor[1] != buttonStateFloor[1]) {
      buttonStateFloor[1] = readingFloor[1];
      if (buttonStateFloor[1] == LOW) {
          buttonPressedFloor[1] = 1;
      }
    }
  }
  lastButtonStateFloor[1] = readingFloor[1];

  readingFloor[2] = digitalRead(buttonFloor[2]);
  if (readingFloor[2] != lastButtonStateFloor[2]) {
    lastDebounceTimeFloor[2] = millis();
  }

  if ((millis() - lastDebounceTimeFloor[2]) > debounceDelay) {
    if (readingFloor[2] != buttonStateFloor[2]) {
      buttonStateFloor[2] = readingFloor[2];
      if (buttonStateFloor[2] == LOW) {
          buttonPressedFloor[2] = 1;
      }
    }
  }
  lastButtonStateFloor[2] = readingFloor[2];

    
  buttonPressedFloor[elevator] = 0;

//state = 0 => the elevator is stationary
//state = 1 => the elevator is going up
//state = 2 => the elevator is going down
switch(state){
  case 0:
        for(int i = 0; i <= topFloor; i++){
          if(buttonPressedFloor[i] == 1){
              movingStartTime = millis();
            if(i > elevator){
              state = 1;
            }else {
              state = 2;
            }
            i = topFloor + 1;
          }
        }
        break;

  case 1:
        if(millis() >= movingStartTime + movingTime){

          digitalWrite(LEDFloor[elevator], LOW);
          elevator++;
          digitalWrite(LEDFloor[elevator], HIGH);

          state = 0;
          if(buttonPressedFloor[elevator] == 1){
            tone(buzzerPin, 1000, 300);
          }
        }
        break;

  case  2:
      if(millis() >= movingStartTime + movingTime){

        digitalWrite(LEDFloor[elevator], LOW);
        elevator--;
        digitalWrite(LEDFloor[elevator], HIGH);

        state=0;
        if(buttonPressedFloor[elevator] == 1){
          tone(buzzerPin, 1000, 300);
        }
      }
      break;

  }
}
