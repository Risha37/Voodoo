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
- Test DipSwitch which HIGH which LOW
- Test Sensors can it work LOW,HIGH and 0,1
- Test whiteValue, blackValue
*/



/*==============================================================================
                          Global Variables
==============================================================================*/





/*==============================================================================
                        Finite State Machine
/*==============================================================================


// enum STATE {
//   START_ROUTINE,
//   SEARCH,
//   ATTACK
// };
// STATE currentState = START_ROUTINE;

// enum SEARCH {
//   RANDOM_SEARCH,
//   TORNADO_SEARCH,
//   WOOD_PECKER
// };
// SEARCH searchTechnique = RANDOM_SEARCH;


/*==============================================================================
                          Buttons & Switches
==============================================================================*/


#define PushButton  0   // push button (digital input pullup)

#define DipSwitch1  0   // switch 1 (digital input pullup - MSB)
#define DipSwitch2  0   // switch 2 (digital input pullup)
#define DipSwitch3  0   // switch 3 (digital input pullup)
#define DipSwitch4  0   // switch 4 (digital input pullup - LSB)

int dipNum;

void dip_switch() {
  /*
  Description
  ----------
  */

  if (digitalRead(DipSwitch1) = LOW && digitalRead(DipSwitch2) = LOW && digitalRead(DipSwitch3) = LOW && digitalRead(DipSwitch4) = LOW) {
    dipNum = 0;
  }

  else if (digitalRead(DipSwitch0) = LOW && digitalRead(DipSwitch1) = LOW && digitalRead(DipSwitch2) = LOW && digitalRead(DipSwitch3) = HIGH) {
    dipNum = 1;
  }

}


/*==============================================================================
                          Opponent Sensors
==============================================================================*/


#define LeftSensor  0   // left sensor (digital input)
#define FrontLeftSensor 0   // left diagonal sensor (digital input)
#define FrontMiddleSensor 0   // front middle sensor (digital input)
#define FrontRightSensor  0   // right diagonal sensor (digital input)
#define RightSensor 0   // right sensor (digital input)

int preyCount = 0, lastSensorValue = 0;    // how many times did the sensor detect the opponent , last side the opponent has been detected


/*==============================================================================
                        Edge Detection Sensors
==============================================================================*/


#define FrontLeftEdge A0   // front left edge sensor (analog input)
#define FrontRightEdge  A0   // front left edge sensor (analog input)
#define BackEdge  A0   // back edge sensor (analog input)

const int blackValue = 650, whiteValue = 500;


/*==============================================================================
                          Sensors Debugging
==============================================================================*/


void sensors_debug() {
  /*
  Description
  ----------
  */

  Serial.println("Opponent Sensors: (L - FL - FM - FR - R)");
  Serial.print(digitalRead(LeftSensor));
  Serial.print(" - ");
  Serial.print(digitalRead(FrontLeftSensor));
  Serial.print(" - ");
  Serial.print(digitalRead(FrontMiddleSensor));
  Serial.print(" - ");
  Serial.print(digitalRead(FrontRightSensor));
  Serial.print(" - ");
  Serial.print(digitalRead(RightSensor));

  Serial.print("\n\n");

  Serial.println("Edge Sensors: (FL - FR - B)");
  Serial.print(analogRead(FrontLeftEdge));
  Serial.print(" - ");
  Serial.print(analogRead(FrontRightEdge));
  Serial.print(" - ");
  Serial.print(analogRead(BackEdge));

  Serial.print("\n\n");

}