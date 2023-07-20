/*
 ▄▀▀▄ ▄▀▀▄  ▄▀▀▀▀▄   ▄▀▀▀▀▄   ▄▀▀█▄▄   ▄▀▀▀▀▄   ▄▀▀▀▀▄
█   █    █ █      █ █      █ █ ▄▀   █ █      █ █      █
▐  █    █  █      █ █      █ ▐ █    █ █      █ █      █
   █   ▄▀  ▀▄    ▄▀ ▀▄    ▄▀   █    █ ▀▄    ▄▀ ▀▄    ▄▀
    ▀▄▀      ▀▀▀▀     ▀▀▀▀    ▄▀▄▄▄▄▀   ▀▀▀▀     ▀▀▀▀
$ Author: Risha $            █     ▐
$ Revision: 0.9 $            ▐

PascalCase  - Pins
camelCase - Variables
snake_case  - Functions

== TODO ==
- 
*/



/*==============================================================================
                          Global Variables
==============================================================================*/


unsigned int dipNum;
const int blackValue = 650, whiteValue = 500;
int lastSensorValue = 0;
bool buttonPressed = false, robotOn = false;
unsigned long buttonPressTime = 0;


/*==============================================================================
                        Finite State Machine
/*==============================================================================





/*==============================================================================
                          Opponent Sensors
==============================================================================*/


#define LeftSensor  0   // left sensor (digital input)
#define FrontLeftSensor 0   // left diagonal sensor (digital input)
#define FrontMiddleSensor 0   // front middle sensor (digital input)
#define FrontRightSensor  0   // right diagonal sensor (digital input)
#define RightSensor 0   // right sensor (digital input)


/*==============================================================================
                        Edge Detection Sensors
==============================================================================*/


#define FrontLeftEdge A0   // front left edge sensor (analog input)
#define FrontRightEdge  A0   // front left edge sensor (analog input)
#define BackEdge  A0   // back edge sensor (analog input)


/*==============================================================================
                          Buttons & Switches
==============================================================================*/


#define PushButton  0   // push button (digital input pullup)


#define DipSwitch1  0   // switch 1 (digital input pullup - MSB)
#define DipSwitch2  0   // switch 2 (digital input pullup)
#define DipSwitch3  0   // switch 3 (digital input pullup)
#define DipSwitch4  0   // switch 4 (digital input pullup - LSB)

int dip_switch() {
  /*
  Returns
  ----------
    - dipNum : unsigned int
        The value of `dipNum` determined by the dip switch states (ranging from 0 to 15).

  Description
  ----------
  Determines the value of the variable `dipNum` based on the combination of the four dip switch states.
  Each dip switch represents a binary value, where LOW represents 0 and HIGH represents 1.
  */

  unsigned int dipNum = 0;

  dipNum |= digitalRead(DipSwitch1) << 3;
  dipNum |= digitalRead(DipSwitch2) << 2;
  dipNum |= digitalRead(DipSwitch3) << 1;
  dipNum |= digitalRead(DipSwitch4);

  return dipNum;
}


/*==============================================================================
                          Sensors Debugging
==============================================================================*/


void sensor_debug() {

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