# Introduction To Robotics (2023-2024)
A beginner-friendly repository to dive into the world of robotics, course taken in the 3rd year at the Faculty of Mathematics and Computer Science, University of Bucharest.
This repository contains a series of homework assignments, each comprising detailed requirements, implementation instructions, code samples, and accompanying image files.

## Homework 2
### Task Requirements
Use a separate potentiometer for controlling each color of the RGB LED: Red,
Green, and Blue. This control must leverage digital electronics. Specifically,
you need to read the potentiometer’s value with Arduino and then write a
mapped value to the LED pins.
### Components
• RGB LED <br />
• 3 Potentiometers <br />
• 3 Resistors <br />
• Wires
### Setup
<p float = "left">
<image src = "https://github.com/AncaaO/IntroductionToRobotics/assets/92025959/1eccb316-d90e-4d86-943a-d89dff2b5e6c" width="40%">
</p>
  
### Link to a video showcasing functionality
https://www.youtube.com/watch?v=Gws7tRzjlYU

## Homework 3
### Task Requirements
Design a control system that simulates a 3-floor elevator using the Arduino
platform. 

Here are the specific requirements:

• LED Indicators: Each of the 3 LEDs should represent one of the 3 floors.
The LED corresponding to the current floor should light up. Additionally,
another LED should represent the elevator’s operational state. It should
blink when the elevator is moving and remain static when stationary.<br />

• Buttons: Implement 3 buttons that represent the call buttons from the
3 floors. When pressed, the elevator should simulate movement towards
the floor after a short interval (2-3 seconds).<br />

• Buzzer (optional for Computer Science, mandatory for CTI):
The buzzer should sound briefly during the following scenarios:<br />
– Elevator arriving at the desired floor (something resembling a ”cling”).<br />
– Elevator doors closing and movement (pro tip: split them into 2
different sounds)<br />

• State Change & Timers: If the elevator is already at the desired floor,
pressing the button for that floor should have no effect. Otherwise, after
a button press, the elevator should ”wait for the doors to close” and then
”move” to the corresponding floor. If the elevator is in movement, it
should either do nothing or it should stack its decision (get to the first
programmed floor, open the doors, wait, close them and then go to the
next desired floor).<br />

• Debounce: Remember to implement debounce for the buttons to avoid
unintentional repeated button presses.<br />

### Setup
<p float = "left">
<image src = "https://github.com/AncaaO/IntroductionToRobotics/assets/92025959/6cc06e92-6609-40cd-81d6-54f553d1e471" width="40%">
</p>

### Link to a video showcasing functionality
https://www.youtube.com/watch?v=Mwtftu1TroQ

## Homework 4
### Task Requirements
Use the joystick to control the position of the segment 
and ”draw” on the display. The movement between segments
should be natural, meaning they should jump from the current position
only to neighbors, but without passing through ”walls”.

Here are the specific requirements:

The initial position should be on the DP. The current
position always blinks (irrespective of the fact that the segment is on or
off). Use the joystick to move from one position to neighbors (see table for
corresponding movement). Short pressing the button toggles the segment
state from ON to OFF or from OFF to ON. Long pressing the button
resets the entire display by turning all the segments OFF and moving the
current position to the decimal point. 

### Setup
<p float = "left">
<image src = "https://github.com/AncaaO/IntroductionToRobotics/assets/92025959/cf6b0bb7-c10f-4869-bed9-3dcd176caf69" width="40%">
</p>

### Link to a video showcasing functionality
https://youtu.be/rmZn8S3NRD4

## Homework 5
### Task Requirements
Using the 4 digit 7 segment display and 3 buttons, implement a stopwatch timer that counts in 10ths of a second
and has a save lap functionality (similar to most basic stopwatch functions
on most phones).

Here are the specific requirements:

The starting value of the 4 digit 7 segment display should
be ”000.0”. The buttons should have the following functionalities:<br />

• Button 1: Start / pause.<br />
• Button 2: Reset (if in pause mode). Reset saved laps (if in lap
viewing mode).<br />
• Button 3: Save lap (if in counting mode), cycle through last saved
laps (up to 4 laps).

### Setup
<p float = "left">
<image src = "https://github.com/AncaaO/IntroductionToRobotics/assets/92025959/980bfe73-6479-4324-9a2c-68f212c5f490" width="40%">
</p>

### Link to a video showcasing functionality
[Here!](https://youtu.be/6Nk67_HRk4g)


## Homework 6
### Task Requirements
Create a "Smart Environmental Monitoring and Logging System" with Arduino. This system will employ two sensors, an Ultrasonic Sensor and a Light Sensor, to collect environmental information, store it in EEPROM memory, and offer visual cues through an RGB LED. User interaction will be facilitated via a Serial Menu.

### Menu Structure

**1. Sensor Settings** // Go to submenu <br />
&nbsp; 1.1 Sensors Sampling Interval. Here you are prompted for
a value between 1 and 10 seconds. Use this value as a sampling rate
for the sensors.<br />
&nbsp; 1.2 Ultrasonic Alert Threshold. Here you are prompted
for a threshold value for the ultrasonic sensor. That is the min 
value (signaling that something is too close).
When sensor value exceeds the threshold value, an alert is
given in the form of a message. If the LED is set to
Automatic Mode (see section 4.2), it also turns red if any of
the sensors are outside the value.<br />
&nbsp; 1.3 LDR Alert Threshold. Here you are prompted for a
threshold value for the LDR sensor. That is the
max value (signals that night is coming).
When sensor value exceeds the threshold value, an alert is
given in the form of a message. If the LED is set to
Automatic Mode (see section 4.2), it also turns red if any of
the sensors are outside the value.<br />
&nbsp; 1.4 Back // Return to main menu<br />

**2. Reset Logger Data.** Should print a message, promting if you to
confirm to delete all data. Something like ”are you sure?”, followed by
the submenu with YES or NO. Both sensor data should be reset at the same
time.<br />
&nbsp; 2.1 Yes.<br />
&nbsp; 2.2 No.<br />

**3. System Status** // Check current status and health<br />
&nbsp; 3.1 Current Sensor Readings. Prints sensor readings
from all sensors when choosing the sumbmenu option.<br />
&nbsp; 3.2 Current Sensor Settings. Displays the sampling rate and
threshold value for all sensors.<br />
&nbsp; 3.3 Display Logged Data.<br />
&nbsp; 3.4 Back. Return to Main menu.<br />

**4. RGB LED Control** // Go to submenu<br />
&nbsp; 4.1 Manual Color Control. Set the RGB colors manually. You
input them by putting a string (Red/Green/Blue). The user is informed of the expected format.<br />
&nbsp; 4.2 LED: Toggle Automatic ON/OFF. If automatic mode is
ON, then the led color turns GREEN when all sensors value do
not exceed threshold values (aka no alert) and RED when there is an
alert (aka ANY sensor value exceeds the threshold). When automatic
mode is OFF, then the LED should use the last saved RGB values (0/0/0 by default). <br />
&nbsp; 4.3 Back // Return to main menu<br />

### Setup
<p float = "left">
<image src = "https://github.com/AncaaO/IntroductionToRobotics/assets/92025959/f62db33e-df2a-42ac-8d48-0d0b5be97d61" width="40%">
</p>

### Link to a video showcasing functionality
[Here!](https://youtu.be/H-LIGSMY_tA)


## Homework 7
### Task Requirements
Develop a small game on the 8x8 matrix. The game must have at least 3 types
of elements: player (blinks slowly), bombs/bullets (blinks fast), wall (doesn’t
blink). It generates walls on the map (50% of the map) every time the program 
is reset and the player moves around and destroys them (Bomberman style).

### Setup
<p float = "left">
<image src = "https://github.com/AncaaO/IntroductionToRobotics/assets/92025959/b7f78529-a1a0-456f-ae6c-f4eb5d380c9d" width="40%">
</p>

### Link to a video showcasing functionality
[Here!](https://youtu.be/A54bzUWjvX8)

