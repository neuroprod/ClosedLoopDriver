//
//  StepperSpeedControler.h
//  ClosedLoopStepper
//
//  Created by Kris Temmerman on 31/01/15.
//
//

#ifndef ClosedLoopStepper_StepperSpeedControler_h
#define ClosedLoopStepper_StepperSpeedControler_h
#include <Arduino.h>
class StepperSpeedControler
{
    
    
public:
    StepperSpeedControler(){};
    void setup(int _pullPin, int _dirPin, int _stepsRot);
    void setTragetSpeed(float speed);
    void loop( unsigned long timeEllapsed);
    
    unsigned long currentTime;
    int stepPauze;
    int pullPin;
    int dirPin;
    int stepsRot;
    float stepDeg;
    bool stepState ;
};

#endif
