//
//  ClosedLoopStepper.h
//  ClosedLoopStepper
//
//  Created by Kris Temmerman on 30/01/15.
//
//

#ifndef ClosedLoopStepper_ClosedLoopStepper_h
#define ClosedLoopStepper_ClosedLoopStepper_h

#include <Arduino.h>
#include "StepperSpeedControler.h"




#define  SERIAL_PORT_SPEED  115200

//#define  DEBUG_INPUT 1


class ClosedLoopStepper
{
    
    
public:
    ClosedLoopStepper(){};
    void setup();
    void loop();
    
   
    void updateSerial();
    void updateEncoder(unsigned long timeElapsed);
    
    char serialData[80];
   
    float Kp;
    float accel;
    int maxSpeed;
    float targetRotation;
    int encoderPosition;
    int encoderPositionOld;
    unsigned long prevTime;
    unsigned long encoderTime;
    
    float rotationSpeed;
    float rotationPosition;
    
    StepperSpeedControler stepper;

};


#endif
