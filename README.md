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
![Homework2 setup](https://github.com/AncaaO/IntroductionToRobotics/assets/92025959/1eccb316-d90e-4d86-943a-d89dff2b5e6c)

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
The buzzer should sound briefly during the following scenarios:
– Elevator arriving at the desired floor (something resembling a ”cling”).
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
![setup](https://github.com/AncaaO/IntroductionToRobotics/assets/92025959/6cc06e92-6609-40cd-81d6-54f553d1e471)

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
![setup](https://github.com/AncaaO/IntroductionToRobotics/assets/92025959/cf6b0bb7-c10f-4869-bed9-3dcd176caf69)


### Link to a video showcasing functionality
https://youtu.be/rmZn8S3NRD4
