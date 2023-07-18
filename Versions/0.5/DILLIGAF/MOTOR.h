/*
 ▄▀▀▄ ▄▀▀▄  ▄▀▀▀▀▄   ▄▀▀▀▀▄   ▄▀▀█▄▄   ▄▀▀▀▀▄   ▄▀▀▀▀▄
█   █    █ █      █ █      █ █ ▄▀   █ █      █ █      █
▐  █    █  █      █ █      █ ▐ █    █ █      █ █      █
   █   ▄▀  ▀▄    ▄▀ ▀▄    ▄▀   █    █ ▀▄    ▄▀ ▀▄    ▄▀
    ▀▄▀      ▀▀▀▀     ▀▀▀▀    ▄▀▄▄▄▄▀   ▀▀▀▀     ▀▀▀▀
$ Author: Risha $            █     ▐
$ Revision: 0.5 $            ▐

PascalCase  - Pins
camelCase - Variables
snake_case  - Functions

== TODO ==
- Test Full Speeds at different delays
- Difference between brake() and stop()
- Test speed_control()
*/



/*==============================================================================
                        Motor Drive Pins
==============================================================================*/


#define LeftMotorPwm  0   // left motor pwm (analog output)
#define RightMotorPwm 0   // right motor pwm (analog output)
#define LeftMotorEnable 0   // left motor enable (digital output)
#define RightMotorEnable  0   // right motor enable (digital output)


/*==============================================================================
                        PWM Frequency Modify
==============================================================================*/


void set_pwm_frequency(int pin, int divisor) {
  /*
  Divides a given PWM pin frequency by a divisor.

  Parameters
  ----------
    - pin : int
        the motor's pwm pin
      
    -divisor : int
        the number to divide the base frequency for the pin by

  Description
  ----------
  We need to modfiy Arduino's base PWM frequency. That frequency normally 490hz which is enough for controlling led brightness
  but not enough for controlling motor speed. With special timer configuration function we change frequency from 490 hz to 3.9 khz.
  
  The resulting frequency is equal to the base frequency divided by
  the given divisor:
    - Base frequencies:
       o The base frequency for pins 3, 9, 10, and 11 is 31250 Hz.
       o The base frequency for pins 5 and 6 is 62500 Hz.
    - Divisors:
       o The divisors available on pins 5, 6, 9 and 10 are: 1, 8, 64,
         256, and 1024.
       o The divisors available on pins 3 and 11 are: 1, 8, 32, 64,
         128, 256, and 1024.

  PWM frequencies are tied together in pairs of pins. If one in a
  pair is changed, the other is also changed to match:
    - Pins 5 and 6 are paired on timer0
    - Pins 9 and 10 are paired on timer1
    - Pins 3 and 11 are paired on timer2

  Note that this function will have side effects on anything else
  that uses timers:
    - Changes on pins 3, 5, 6, or 11 may cause the delay() and
      millis() functions to stop working. Other timing-related
      functions may also be affected.
    - Changes on pins 9 or 10 will cause the Servo library to function
      incorrectly.
  */

  byte mode;

  if (pin == 5 || pin == 6 || pin == 9 || pin == 10) {

    switch (divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }

    if (pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }

  }
}


/*==============================================================================
                          Moving & Turning
==============================================================================*/

  /*
  Full speed Moving & Turning

  Description
  ----------
  PWM Pin: Controls both speed and direction. %50 duty cyle means %0 speed. %95 Duty Cycle full speed to forward, %5 Duty Cycle Full speed to backward.
  For controlling driver you need to supply PWM signals between %5 to %95 duty cycle (fullness value)
  Disable Pin: This stops motors, When you apply Logic 1 (5V) it will stop motors.

  Duty Cycle = 255
  Full Forward Speed = 252/255
  Full Backward Speed = 2/255
  Stop = 128/255  (Half Duty Cycle and Enable Low Stops Motor)

  For stopping motors there is 2 methods
    1st you can apply %50 duty cycle PWM signal, that will stop motors.
    2nd is giving logic 1 to Disable signal.
  We strongly suggest you to use second disable pin option. Giving %50 signal to motor driver will result temperature increase of mosfets.
  */


void go_forward() {
  digitalWrite(LeftMotorEnable, HIGH);
  digitalWrite(RightMotorEnable, HIGH);
  analogWrite(LeftMotorPwm, 245);
  analogWrite(RightMotorPwm, 245);
}

void go_backward() {
  digitalWrite(LeftMotorEnable, HIGH);
  digitalWrite(RightMotorEnable, HIGH);
  analogWrite(LeftMotorPwm, 10);
  analogWrite(RightMotorPwm, 10);
}

void turn_left() {
  digitalWrite(LeftMotorEnable, HIGH);
  digitalWrite(RightMotorEnable, HIGH);
  analogWrite(LeftMotorPwm, 10);
  analogWrite(RightMotorPwm, 245);
}

void turn_right() {
  digitalWrite(LeftMotorEnable, HIGH);
  digitalWrite(RightMotorEnable, HIGH);
  analogWrite(LeftMotorPwm, 245);
  analogWrite(RightMotorPwm, 10);
}

void stop() {
  // Free Wheeling
  digitalWrite(LeftMotorEnable, LOW);
  digitalWrite(RightMotorEnable, LOW);
  analogWrite(LeftMotorPwm, 127);
  analogWrite(RightMotorPwm, 127);
}

void brake() {
  // Hard Stop
  digitalWrite(LeftMotorEnable, HIGH);
  digitalWrite(RightMotorEnable, HIGH);
  analogWrite(LeftMotorPwm, 128);
  analogWrite(RightMotorPwm, 128);
}


/*==============================================================================
                          Speed Control
==============================================================================*/


void motor_control(int leftSpeed, int rightSpeed) {
  /*
  This function controls motor speed raw between -255 to +255

  Parameters
  ----------
    - leftSpeed : int
        left motor speed (between -255 to +255 (negative backward, positive forward))

    - rightSpeed : int
        right motor speed (between -255 to +255 (negative backward, positive forward))
  */

  if (leftSpeed > 0) {
    leftSpeed = leftSpeed / 2 + 117;
    digitalWrite(LeftMotorEnable, HIGH);
    analogWrite(LeftMotorPwm, leftSpeed);
  } else if (leftSpeed < 0) {
    leftSpeed = abs(leftSpeed);
    leftSpeed = (137 - leftSpeed / 2);
    digitalWrite(LeftMotorEnable, HIGH);
    analogWrite(LeftMotorPwm, leftSpeed);
  } else {
    digitalWrite(LeftMotorEnable, LOW);
    analogWrite(LeftMotorPwm, 127);
  }

  if (rightSpeed > 0) {
    rightSpeed = rightSpeed / 2 + 117;
    digitalWrite(RightMotorEnable, HIGH);
    analogWrite(RightMotorPwm, rightSpeed);
  } else if (rightSpeed < 0) {
    rightSpeed = abs(rightSpeed);
    rightSpeed = (137 - rightSpeed / 2);
    digitalWrite(RightMotorEnable, HIGH);
    analogWrite(RightMotorPwm, rightSpeed);
  } else {
    digitalWrite(RightMotorEnable, LOW);
    analogWrite(RightMotorPwm, 127);
  }

}


/*==============================================================================
                          Motors Testing
==============================================================================*/


void motor_debugging() {

  Serial.println("Left Motor Full Forward");
  digitalWrite(LeftMotorEnable, HIGH);
  analogWrite(LeftMotorPwm, 245);
  delay(10000);
  stop();

  Serial.println("Right Motor Full Forward");
  digitalWrite(RightMotorEnable, HIGH);
  analogWrite(RightMotorPwm, 245);
  delay(10000);
  stop();

}


void motor_speed_test() {

  Serial.println("Full Forward");
  for (int i = 1000; i <= 10000; i+=1000) {
    go_forward();
    delay(i);
    stop();
    delay(10000);
  }


  delay(30000);


  Serial.println("Full Backward");
  for (int i = 1000; i <= 10000; i+=1000) {
    go_backward();
    delay(i);
    stop();
    delay(10000);
  }


  delay(30000);


  Serial.println("Full Turn Right");
  for (int i = 1000; i <= 10000; i+=1000) {
    turn_right();
    delay(i);
    stop();
    delay(10000);
  }


  delay(30000);


  Serial.println("Full Turn Left");
  for (int i = 1000; i <= 10000; i+=1000) {
    turn_left();
    delay(i);
    stop();
    delay(10000);
  }


  delay(30000);


  Serial.println("Full Turn Right");
  for (int i = 1000; i <= 10000; i+=1000) {
    turn_right();
    delay(i);
    stop();
    delay(10000);
  }


  delay(30000);


  Serial.println("Brake vs Free-Wheeling");

  Serial.println("Brake");
  go_froward();
  delay(2000);
  stop();
  delay(100);
  turn_left();
  delay(1000);
  stop();

  Serial.println("Free-Wheeling");
  go_froward();
  delay(2000);
  brake();
  delay(100);
  turn_left();
  delay(1000);
  brake();

}

