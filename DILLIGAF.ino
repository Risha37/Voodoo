/*
 ██▒   █▓ ▒█████   ▒█████   ▓█████▄  ▒█████   ▒█████
▓██░   █▒▒██▒  ██▒▒██▒  ██▒ ▒██▀ ██▌▒██▒  ██▒▒██▒  ██▒
 ▓██  █▒░▒██░  ██▒▒██░  ██▒ ░██   █▌▒██░  ██▒▒██░  ██▒
  ▒██ █░░▒██   ██░▒██   ██░▒░▓█▄   ▌▒██   ██░▒██   ██░
   ▒▀█░  ░ ████▓▒░░ ████▓▒░░░▒████▓ ░ ████▓▒░░ ████▓▒░
   ░ ▐░  ░ ▒░▒░▒░ ░ ▒░▒░▒░ ░ ▒▒▓  ▒ ░ ▒░▒░▒░ ░ ▒░▒░▒░
   ░ ░░    ░ ▒ ▒░   ░ ▒ ▒░   ░ ▒  ▒   ░ ▒ ▒░   ░ ▒ ▒░
     ░░  ░ ░ ░ ▒  ░ ░ ░ ▒    ░ ░  ░ ░ ░ ░ ▒  ░ ░ ░ ▒
      ░      ░ ░      ░ ░      ░        ░ ░      ░ ░
$ Author: Risha $
$ Revision: 0.5 $

PascalCase  - Pins
camelCase - Variables
snake_case  - Functions

== TODO ==
- 
*/



/*===============================================================
                          LIBRARIES
===============================================================*/


#include  "DILLIGAF.h"    // import the header file which contains the sensors' pins & global variables
#include  "MOTOR.h"   // import the motor driver pins & functions


/*===============================================================
                          SETUP
===============================================================*/


void setup() {
  // put your setup code here, to run once:


  set_pwm_frequency(LeftMotorPwm, 8);   // (pin,divisor)
  set_pwm_frequency(RightMotorPwm, 8);  // (pin,divisor)


  // ======= OUTPUTS ======= //

  pinMode(LeftMotorPwm, OUTPUT);
  pinMode(RightMotorPwm, OUTPUT);
  pinMode(LeftMotorEnable, OUTPUT);
  pinMode(RightMotorEnable, OUTPUT);


  // ======= INPUTS ======= //

  pinMode(LeftSensor, INPUT);
  pinMode(FrontLeftSensor, INPUT);
  pinMode(FrontMiddleSensor, INPUT);
  pinMode(FrontRightSensor, INPUT);
  pinMode(RightSensor, INPUT);

  pinMode(PushButton, INPUT_PULLUP);

  pinMode(DipSwitch1, INPUT_PULLUP);
  pinMode(DipSwitch2, INPUT_PULLUP);
  pinMode(DipSwitch3, INPUT_PULLUP);
  pinMode(DipSwitch4, INPUT_PULLUP);


  // ======= Activate Sensors ======= //

  digitalWrite(LeftSensor, HIGH);
  digitalWrite(FrontLeftSensor, HIGH);
  digitalWrite(FrontMiddleSensor, HIGH);
  digitalWrite(FrontRightSensor, HIGH);
  digitalWrite(RightSensor, HIGH);


  // ======= Strategy ======= //
  dipNum = dip_switch();


  // Serial.begin(9600);

}


/*===============================================================
                        Line Control
===============================================================*/


void line_control() {
  /*
  Description
  ----------
  This function controls the behavior of the robot based on edge sensor readings.
  It uses analog readings from front and back edge sensors to make decisions and adjust the robot's movement.

  Behavior
  ----------
  - The function checks the readings from the front left, front right and back edge sensors.
  - If a white edge is detected on the left side, the robot performs a backward maneuver, followed by a right turn.
  - If a white edge is detected on the right side, the robot performs a backward maneuver, followed by a left turn.
  - If white edges are detected on both front sides, the robot performs a backward maneuver, followed by a sharp 180 turn.
  - If a white edge is detected on the back side, the robot moves forward at full speed.
  - Brakes are added at appropriate stages to enhance maneuvering control.
*/

  if (analogRead(FrontLeftEdge) < whiteValue && analogRead(FrontRightEdge) > blackValue) {
    // white edge on the left side
    delay(8);
    if (analogRead(FrontLeftEdge) < whiteValue) {
      go_backward(1000);
      brake(50);
      turn_right(1200);
      brake(50);
    }
  }

  else if (analogRead(FrontLeftEdge) > blackValue && analogRead(FrontRightEdge) < whiteValue) {
    // white edge on the right side
    delay(8);
    if (analogRead(FrontRightEdge) < whiteValue) {
      go_backward(1000);
      brake(50);
      turn_left(1200);
      brake(50);
    }
  }

  else if (analogRead(FrontLeftEdge) < whiteValue && analogRead(FrontRightEdge) < whiteValue) {
    // white edge on the both front sides
    delay(8);
    if (analogRead(FrontLeftEdge) < whiteValue && analogRead(FrontRightEdge) < whiteValue) {
      go_backward(1000);
      brake(50);
      turn_right(1200);
      brake(50);
    }
  }

  else if (analogRead(FrontLeftEdge) > blackValue && analogRead(FrontRightEdge) > blackValue && analogRead(BackEdge) < whiteValue) {
    // white edge on the back side
    delay(8);
    if (analogRead(BackEdge) < whiteValue) {
      go_forward(1200);
      brake(50);
    }
  }

}


/*===============================================================
                        Hunt (Attack)
===============================================================*/


void hunt() {
  /*
  Description:
  ------------
  This function implements a behavior for a robot or vehicle. It combines line following, edge detection,
  and sensor readings to navigate and react to different scenarios.
  
  Behavior:
  ----------
  - If any of the edge sensors (`FrontLeftEdge`, `FrontRightEdge`, or `BackEdge`) detect a value lower than `whiteValue`, the `line_control()` function is called to follow the line.
  - If the `LeftSensor` is HIGH, the robot is turned left and `lastSensorValue` is set to 1.
  - If the `FrontLeftSensor` is HIGH, the robot is turned right and `lastSensorValue` is set to 4.
  - If the `FrontMiddleSensor` is HIGH, the robot goes forward and `lastSensorValue` is set to 3.
  - If the `FrontRightSensor` is HIGH, the robot is turned left and `lastSensorValue` is set to 2.
  - If the `RightSensor` is HIGH, the robot is turned right and `lastSensorValue` is set to 5.
  - If none of the above conditions are met, the function checks the value of `lastSensorValue` to determine the appropriate motor control actions:
    - If `lastSensorValue` is 1 or 2, the robot is controlled to move backward and turn slightly to the right.
    - If `lastSensorValue` is 3, the robot is controlled to move forward with equal motor speeds.
    - If `lastSensorValue` is 4 or 5, the robot is controlled to move forward and turn slightly to the left.
 */

  if (analogRead(FrontLeftEdge) < whiteValue || analogRead(FrontRightEdge) < whiteValue || analogRead(BackEdge) < whiteValue) {
    line_control();
  }

  else if (digitalRead(FrontMiddleSensor)  == HIGH) {
    delay(8);
    if (digitalRead(FrontMiddleSensor) == HIGH) {
      go_forward(-1);
      lastSensorValue = 3;
    }
  }

  else if (digitalRead(FrontLeftSensor) == HIGH) {
    delay(8);
    if (digitalRead(FrontLeftSensor) == HIGH) {
      turn_right(-1);
      lastSensorValue = 4;
    }
  }

  else if (digitalRead(FrontRightSensor) == HIGH) {
    delay(8);
    if (digitalRead(FrontRightSensor) == HIGH) {
      turn_left(-1);
      lastSensorValue = 2;
    }
  }

  else if (digitalRead(LeftSensor) == HIGH) {
    delay(8);
    if (digitalRead(LeftSensor) == HIGH) {
      turn_left(-1);
      lastSensorValue = 1;
    }
  }

  else if (digitalRead(RightSensor) == HIGH) {
    delay(8);
    if (digitalRead(RightSensor) == HIGH) {
      turn_right(-1);
      lastSensorValue = 5;
    }
  }

  else {
    if (lastSensorValue == 1) {
      motor_control(-150, 200);
    }
    else if (lastSensorValue == 2) {
      motor_control(-150, 200);
    }
    else if (lastSensorValue == 3) {
      motor_control(220, 220);
    }
    else if (lastSensorValue == 4) {
      motor_control(200, -150);
    }
    else if (lastSensorValue == 5) {
      motor_control(200, -150);
    }
  }

}


/*===============================================================
                        MAIN LOOP
===============================================================*/


void loop() {
  // put your main code here, to run repeatedly:


  if (digitalRead(PushButton) == LOW && !buttonPressed && !robotOn) {
    buttonPressTime = millis();
    buttonPressed = true;
  }

  else if (digitalRead(PushButton) == LOW && !buttonPressed && robotOn) {
    stop(1000);
    robotOn = false;
    buttonPressed = false;
  }
  else if (buttonPressed && (millis() - buttonPressTime) >= 5000) {
    robotOn = true;
    buttonPressed = false;
  }


  if (robotOn) {

    lastSensorValue = 3;

    switch (dipNum) {

      case 0:   // Testing
        sensor_debug();
        motor_debugging();
        break;
      
      case 1:   // Direct Forward Attack
        while (digitalRead(PushButton) != LOW) {
          motor_control(250, 250); delay(3000);
          motor_control(1, 1); delay(500);
          motor_control(-250, -250); delay(3000);
          motor_control(1, 1); delay(40000);
        }
        break;

      case 2:   // Left Arc
        motor_control(-250, 250); delay(560);
        motor_control(1, 1); delay(50);
        motor_control(250, 250); delay(800);
        motor_control(1, 1); delay(50);
        motor_control(250, 140); delay(1600);
        motor_control(1, 1); delay(50);
        motor_control(250, -250); delay(400);
        break;

      case 3:   // Right Arc
        motor_control(250, -250); delay(500);
        brake(50);
        motor_control(250, 250); delay(800);
        brake(50);
        motor_control(150, 250); delay(1600);
        brake(50);
        motor_control(-250, 250); delay(600);
        break;

      case 4:   // Left Zig-Zag
        motor_control(-250, 250); delay(700);
        motor_control(1, 1); delay(50);
        motor_control(250, 250); delay(1900);
        motor_control(1, 1); delay(50);
        motor_control(250, -250); delay(400);
        motor_control(1, 1); delay(50);
        motor_control(250, 250); delay(1200);
        break;

      case 5:   // Right Zig-Zag
        motor_control(250, -250); delay(400);
        motor_control(1, 1); delay(50);
        motor_control(250, 250); delay(1600);
        motor_control(1, 1); delay(50);
        motor_control(-250, 250); delay(1200);
        motor_control(1, 1); delay(50);
        motor_control(250, 250); delay(1200);
        break;

      case 6:   // Down Down Down
        motor_control(250, 250); delay(600);
        motor_control(1, 1); delay(100);
        motor_control(-250, -250); delay(600);
        motor_control(250, 250); delay(600);
        motor_control(1, 1); delay(100);
        motor_control(-250, -250); delay(600);
        motor_control(250, 250); delay(600);
        motor_control(1, 1); delay(100);
        motor_control(-250, -250); delay(600);
        motor_control (250, 250); delay(1200);
        break;

      case 7:

        break;

      case 8:

        break;

      case 9:

        break;

      case 10:

        break;

      case 11:

        break;

      case 12:

        break;

      case 13:

        break;

      case 14:

        break;

      case 15:

        break;

      default:

        break;

    }

    while (digitalRead(PushButton) != LOW) {
      hunt();
    }

  }
}

