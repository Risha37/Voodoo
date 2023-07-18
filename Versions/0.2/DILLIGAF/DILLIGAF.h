/*
 ▄▀▀▄ ▄▀▀▄  ▄▀▀▀▀▄   ▄▀▀▀▀▄   ▄▀▀█▄▄   ▄▀▀▀▀▄   ▄▀▀▀▀▄
█   █    █ █      █ █      █ █ ▄▀   █ █      █ █      █
▐  █    █  █      █ █      █ ▐ █    █ █      █ █      █
   █   ▄▀  ▀▄    ▄▀ ▀▄    ▄▀   █    █ ▀▄    ▄▀ ▀▄    ▄▀
    ▀▄▀      ▀▀▀▀     ▀▀▀▀    ▄▀▄▄▄▄▀   ▀▀▀▀     ▀▀▀▀
$ Author: Risha $            █     ▐
$ Revision: 0.2 $            ▐


== TODO ==
- 
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
//   WOODPECKER
// };
// SEARCH searchTechnique = RANDOM_SEARCH;


/*==============================================================================
                          Buttons & Switches
==============================================================================*/


#define DipSwitch0  0   // switch 0 (digital input pullup - LSB)
#define DipSwitch1  0   // switch 1 (digital input pullup)
#define DipSwitch2  0   // switch 2 (digital input pullup)
#define DipSwitch3  0   // switch 3 (digital input pullup - MSB)


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