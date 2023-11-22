// Define variables for sensor settings
int samplingInterval = 3; // Default sampling interval
int ultrasonicThreshold = 10; // Default ultrasonic threshold
int ldrThreshold = 300; // Default LDR threshold
unsigned long lastSample = 0;

const long delayInterval = 100;

//Define variables for photocell
const int ldrPin = 0; // the cell and 10K pulldown are connected to a0
int ldrValue;

//Define variables for ultrasonic
const int trigPin = 9;
const int echoPin = 10;
long duration = 0;
int distance = 0;

const int redLED = 6;  
const int greenLED = 5;
const int blueLED = 3;
// Define variables for RGB LED control
int redValue = 0;
int greenValue = 0;
int blueValue = 0;

bool automaticMode = true;

void setup() {

  Serial.begin(9600);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  displayMainMenu();
}

void loop() {

  if (Serial.available()) {
    int choice = Serial.parseInt();
    executeMenuChoice(choice);
    displayMainMenu();
  }
  sensorsReadings();

}

void sensorsReadings(){
  if (millis() > lastSample + samplingInterval * 1000){
    ldrValue = analogRead(ldrPin);

    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    // Sound wave reflects from the obstacle, so to calculate the distance we
    // consider half of the distance traveled.
    distance = duration*0.034/2;

    lastSample = millis();
  }
}

void displayMainMenu() {
  Serial.println("\nMain Menu");
  Serial.println("1. Sensor Settings");
  Serial.println("2. Reset Logger Data");
  Serial.println("3. System Status");
  Serial.println("4. RGB LED Control");
  Serial.println("Enter your choice: ");
}

void executeMenuChoice(int choice) {
  switch (choice) {
    case 1:
      sensorSettingsMenu();
      break;
    case 2:
      resetLoggerData();
      break;
    case 3:
      systemStatusMenu();
      break;
    case 4:
      rgbLEDControlMenu();
      break;
    default:
      Serial.println("Invalid choice. Please enter a valid option.");
  }
    // Clear the serial buffer
  if (Serial.available() > 0) {
    char clearChar = Serial.read();
  }
}

void sensorSettingsMenu() {
  int subChoice = 0;
  while (subChoice != 4) {
    Serial.println("Sensor Settings");
    Serial.println("1. Sensors Sampling Interval");
    Serial.println("2. Ultrasonic Alert Threshold");
    Serial.println("3. LDR Alert Threshold");
    Serial.println("4. Back");
    Serial.println("Enter your choice: ");
  
    while (!Serial.available()) {
      delay(10);
    }

    if (Serial.available()) {
      subChoice = Serial.parseInt();
      executeSensorSettingChoice(subChoice);
    }
  }
}

void executeSensorSettingChoice(int subChoice){
  switch (subChoice) {
    case 1:
      Serial.println("Introduce a sampling rate for the sensors (a value between 1 and 10)");
      samplingInterval = Serial.parseInt();

      while (!Serial.available()) {
        delay(10);
      }
      
      samplingInterval = Serial.parseInt();

      if (samplingInterval >= 1 && samplingInterval <= 10) {
        Serial.println("Sampling rate set to: " + String(samplingInterval));
        break; // Exit the loop once a valid value is entered
      } else {
        Serial.println("Invalid value. Please enter a value between 1 and 10.");
        int clearChar = Serial.read();
      }
      break;
    case 2:
      Serial.println("Enter the minimum ultrasonic threshold value: ");
      ultrasonicThreshold = Serial.parseInt();

      while (!Serial.available()) {
        delay(10);
      }
      
      ultrasonicThreshold = Serial.parseInt();
      
      if (ultrasonicThreshold >= 0) {
        Serial.println("Minimum ultrasonic threshold set to: " + String(ultrasonicThreshold));
        break; // Exit the loop once a valid value is entered
      } else {
        Serial.println("Invalid value. Please enter a non-negative value.");
        int clearChar = Serial.read();
      }

      break;
    case 3:
      Serial.println("Enter the photocell threshold value to signal sunset: ");
      ldrThreshold = Serial.parseInt();
      
      while (!Serial.available()) {
        delay(10);
      }
      
      ldrThreshold = Serial.parseInt();
      
      if (ldrThreshold >= 0 && ldrThreshold <= 1023) {
        Serial.println("Photocell threshold set to: " + String(ldrThreshold));
        break; // Exit the loop once a valid value is entered
      } else {
        Serial.println("Invalid value. Please enter a value between 0 and 1023.");
        int clearChar = Serial.read();
      }
      break;
    case 4:
      break;
    default:
      Serial.println("Invalid choice. Please enter a valid option.");
  }
}

void resetLoggerData() {
  Serial.println("Reset Logger Data");
  Serial.println("Are you sure? Enter 1 for Yes, 2 for No: ");
  // Code to handle confirmation input
}

void systemStatusMenu() {
  int subChoice = 0;
  while (subChoice != 4) {
    Serial.println("System Status");
    Serial.println("1. Current Sensor Readings");
    Serial.println("2. Current Sensor Settings");
    Serial.println("3. Display Logged Data");
    Serial.println("4. Back");
    Serial.println("Enter your choice: ");

    while (!Serial.available()) {
      delay(10);
    }
    subChoice = Serial.parseInt();
    switch (subChoice) {
      case 1:
        sensorsReadings();
        Serial.println("Sensors readings: ");

        Serial.print("Photocell value: ");
        Serial.println(ldrValue);

        Serial.print("Ultrasonic value: ");
        Serial.println(distance);
        break;
      case 2:
        Serial.println("Current sensor settings:");
        Serial.print("Sensors sampling rate:");
        Serial.println(samplingInterval);
        Serial.print("Photocell threshold value:");
        Serial.println(ultrasonicThreshold);
        Serial.print("Distance sensor threshold value:");
        Serial.println(ldrThreshold);
        break;
      case 3:
        Serial.println("Code to display logged data");
        // Code to display logged data
        break;
      case 4:
        break;
      default:
        Serial.println("Invalid choice. Please enter a valid option.");
    }
  }
}

void rgbLEDControlMenu() {
  int subChoice = 0;
  while (subChoice != 3) {
    Serial.println("RGB LED Control");
    Serial.println("1. Manual Color Control");
    Serial.println("2. LED: Toggle Automatic ON/OFF");
    Serial.println("3. Back");
    Serial.println("Enter your choice: ");

    while (!Serial.available()) {
      delay(10);
    }
    subChoice = Serial.parseInt();
    switch (subChoice) {
      case 1:
        Serial.println("Enter Red/Green/Blue values (0-255) separated by '/': ");

        while (!Serial.available()) {
          delay(10);
        }

        redValue = Serial.parseInt();
        Serial.read(); // Consume the slash character
        greenValue = Serial.parseInt();
        Serial.read();
        blueValue = Serial.parseInt();

        if (redValue >= 0 && redValue <= 255 && greenValue >= 0 && greenValue <= 255 && blueValue >= 0 && blueValue <= 255) {
          analogWrite(redLED, redValue);
          analogWrite(greenLED, greenValue);
          analogWrite(blueLED, blueValue);
          Serial.println("RGB values set successfully.");
        } else {
          Serial.println("Invalid values. Please enter values between 0 and 255.");
        }
        break;
      case 2:
        automaticMode = !automaticMode; // Toggle automatic mode
        if (automaticMode) {
          if (distance < ultrasonicThreshold || ldrValue < ldrThreshold) {
            // Alert condition: Set LED to RED
            analogWrite(redLED, 255);
            analogWrite(greenLED, 0);
            analogWrite(blueLED, 0);
          } else {
            // No alert condition: Set LED to GREEN
            analogWrite(redLED, 0);
            analogWrite(greenLED, 255);
            analogWrite(blueLED, 0);
          }
        } else {
          // Manual mode: Use last saved RGB values
          analogWrite(redLED, redValue);
          analogWrite(greenLED, greenValue);
          analogWrite(blueLED, blueValue);
        }
        break;
      case 3:
        break;
      default:
        Serial.println("Invalid choice. Please enter a valid option.");
    }
  }
}


