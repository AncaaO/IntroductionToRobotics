const int analog_in0 = A0;
const int analog_in1 = A1;
const int analog_in2 = A2;

const int RLED = 9;  
const int GLED = 10;
const int BLED = 11;

const int analog_max_val = 1023;
const int LED_max_val = 255;

int value0 = 0;
int value1 = 0;
int value2 = 0;

void setup ()
{
  Serial.begin(9600);
  pinMode(RLED, OUTPUT);
  pinMode(GLED, OUTPUT);
  pinMode(BLED, OUTPUT);
}

void loop()
{
  value0 = analogRead(analog_in0);
  value1 = analogRead(analog_in1);
  value2 = analogRead(analog_in2);

  value0 = map(value0, 0, analog_max_val, 0, LED_max_val);
  value1 = map(value1, 0, analog_max_val, 0, LED_max_val);
  value2 = map(value2, 0, analog_max_val, 0, LED_max_val);

  analogWrite(RLED, value0);
  analogWrite(GLED, value1);
  analogWrite(BLED, value2);
}
