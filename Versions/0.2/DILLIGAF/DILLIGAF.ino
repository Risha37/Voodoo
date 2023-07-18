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
$ Revision: 0.2 $

== TODO ==
- 
*/



/*===============================================================
                          LIBRARIES
===============================================================*/


#include  "DILLIGAF.h"    // import the header file which contains the sensors' pins & global variables.
#include  "MOTOR.h"   // import the motor drive functions


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

  pinMode(FrontLeftEdge, INPUT);
  pinMode(FrontRightEdge, INPUT);
  pinMode(BackEdge, INPUT);

  pinMode(DipSwitch0, INPUT_PULLUP);
  pinMode(DipSwitch1, INPUT_PULLUP);
  pinMode(DipSwitch2, INPUT_PULLUP);
  pinMode(DipSwitch3, INPUT_PULLUP);


  // ======= Activate Sensors ======= //

  digitalWrite(LeftSensor, HIGH);
  digitalWrite(FrontLeftSensor, HIGH);
  digitalWrite(FrontMiddleSensor, HIGH);
  digitalWrite(FrontRightSensor, HIGH);
  digitalWrite(RightSensor, HIGH);


  // Serial.begin(9600);   // This function Disables DipSwitch 0 and 1 -USE CAREFULLY-.


  // ======= 5sec Delay ======= //
  delay(4999);

}


/*===============================================================
                        Line Control
===============================================================*/


void line_control() {
  /*
  Description
  ----------
  This function controls the edge sensors readings and behavior.
    - Brakes are added for better manuevering control.
  */

  if (analogRead(FrontLeftEdge) < whiteValue && analogRead(FrontRightEdge) > blackValue) {
    // white edge on the left side
    delay(8);
    if (analogRead(FrontLeftEdge) < whiteValue) {
      go_backward();
      delay(1000);
      brake();
      delay(50);
      turn_right();
      delay(1200);
      brake();
      delay(50);
    }
  }

  else if (analogRead(FrontLeftEdge) > blackValue && analogRead(FrontRightEdge) < whiteValue) {
    // white edge on the right side
    delay(8);
    if (analogRead(FrontRightEdge) < whiteValue) {
      go_backward();
      delay(1000);
      brake();
      delay(50);
      turn_left();
      delay(1200);
      brake();
      delay(50);
    }
  }

  else if (analogRead(FrontLeftEdge) < whiteValue && analogRead(FrontRightEdge) < whiteValue) {
    // white edge on the both front sides
    delay(8);
    if (analogRead(FrontLeftEdge) < whiteValue && analogRead(FrontRightEdge) < whiteValue) {
      go_backward();
      delay(1000);
      brake();
      delay(50);
      turn_right();
      delay(2400);
      brake();
      delay(50);
    }
  }

  else if (analogRead(FrontLeftEdge) > blackValue && analogRead(FrontRightEdge) > blackValue && analogRead(BackEdge) < whiteValue) {
    // white edge on the back side
    delay(8);
    if (analogRead(BackEdge) < whiteValue) {
      go_forward();
      delay(1200);
      brake();
      delay(50);
    }
  }

}


/*===============================================================
                        Chase (Following)
===============================================================*/


void chase() {
  /*
  Description
  ----------

  */

  if (analogRead(FrontLeftEdge) < whiteValue || analogRead(FrontRightEdge) < whiteValue || analogRead(BackEdge) < whiteValue) {
    line_control();
  }

  else if (digitalRead(FrontMiddleSensor) == HIGH) {
    preyCount++; lastSensorValue = 0; delay(1);

    if (preyCount < 300) {
      go_forward();
      delay(1);
    }
    else if (preyCount == 300 && digitalRead(FrontMiddleSensor) == HIGH) {
      stop();
    }
  }

  else if (digitalRead(LeftSensor) == HIGH && digitalRead(FrontMiddleSensor) == LOW) {
    turn_left();
    delay(8);
    preyCount = 0;
  }

  else if (digitalRead(RightSensor) == HIGH && digitalRead(FrontMiddleSensor) == LOW) {
    turn_right();
    delay(8);
    preyCount = 0;
  }

  else if (digitalRead(FrontLeftSensor) == HIGH && digitalRead(FrontMiddleSensor) == LOW) {
    turn_left();
    delay(16);
    preyCount = 0;
  }

  else if (digitalRead(FrontRightSensor) == HIGH && digitalRead(FrontMiddleSensor) == LOW) {
    turn_right();
    delay(16);
    preyCount = 0;
  }

  else {
    stop();
  }

}


/*===============================================================
                        Hunt (Attacking)
===============================================================*/


void hunt() {
  /*
  Description
  ----------
  
  */

  if (analogRead(FrontLeftEdge) < whiteValue || analogRead(FrontRightEdge) < whiteValue || analogRead(BackEdge) < whiteValue) {
    line_control();
  }

  else if (digitalRead(LeftSensor) == HIGH) {
    delay(8);
    if (digitalRead(LeftSensor) == HIGH) {
      turn_left();
      lastSensorValue = 1;
    }
  }

  else if (digitalRead(FrontLeftSensor) == HIGH) {
    delay(8);
    if (digitalRead(FrontLeftSensor) == HIGH) {
      turn_left();
      lastSensorValue = 2;
    }
  }

  else if (digitalRead(FrontMiddleSensor)  == HIGH) {
    delay(8);
    if (digitalRead(FrontMiddleSensor) == HIGH) {
      go_forward();
      lastSensorValue = 3;
    }
  }

  else if (digitalRead(FrontRightSensor) == HIGH) {
    delay(8);
    if (digitalRead(FrontRightSensor) == HIGH) {
      turn_right();
      lastSensorValue = 4;
    }
  }

  else if (digitalRead(RightSensor) == HIGH) {
    delay(8);
    if (digitalRead(RightSensor) == HIGH) {
      turn_right();
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

  sensors_debug();


  // ======= Strategies ======= //
  // if (digitalRead(DipSwitch0) = HIGH && digitalRead(DipSwitch1) = HIGH && digitalRead(DipSwitch2) = HIGH && digitalRead(DipSwitch3) = HIGH) {

  // }

  // else if (digitalRead(DipSwitch0) = HIGH && digitalRead(DipSwitch1) = HIGH && digitalRead(DipSwitch2) = HIGH && digitalRead(DipSwitch3) = HIGH) {

  // }


}

