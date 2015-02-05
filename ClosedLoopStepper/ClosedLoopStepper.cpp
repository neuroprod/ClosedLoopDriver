//
//  ClosedLoopStepper.cpp
//  ClosedLoopStepper
//
//  Created by Kris Temmerman on 30/01/15.
//
//

#include "ClosedLoopStepper.h"


void ClosedLoopStepper::setup()
{
    Serial.begin(115200);
    prevTime =0;
    encoderPositionOld=0;
    encoderTime =0;
    
    stepper.setup(22,24,3200);
    stepper.setTragetSpeed(0);
    targetRotation =0;
  
    Kp=20;
    accel =200;
    maxSpeed =2000;
}
void ClosedLoopStepper::loop()
{
    //
    
    unsigned long currentTime  = micros();
    unsigned long timeElapsed =   currentTime  -prevTime;
    prevTime = currentTime ;
    //
    updateSerial();
    //
    updateEncoder(timeElapsed);
    
    stepper.loop(timeElapsed);

}
void ClosedLoopStepper::updateEncoder(unsigned long timeElapsed)
{
    encoderTime +=timeElapsed;
    if(encoderTime>10000)
    {
        rotationPosition=encoderPosition/(1600.0f*80.0f/30.0f) *360.0f;
        
        
        
        
        float posChange =encoderPosition-encoderPositionOld;
        float rotChange = posChange /(1600.0f*80.0f/30.0f) *360.0f; //change in deg
        rotationSpeed  = rotChange  / ((float)encoderTime/1000000.0f); // rotspeed deg/sec
       
        
        
        float targetSpeed  =(targetRotation-rotationPosition )*Kp;
        float speedError  = (rotationSpeed-targetSpeed) ;
        if(speedError<0)
        {
            if(speedError<-accel)speedError =-accel;
        
        }else
        {
            if(speedError>accel)speedError =accel;
        }
        
        float targetMSpeed =rotationSpeed-speedError;
        if(targetMSpeed>maxSpeed)targetMSpeed=maxSpeed;
        if(targetMSpeed<-maxSpeed)targetMSpeed=-maxSpeed;
       
        stepper.setTragetSpeed(targetMSpeed);
        
        encoderPositionOld=encoderPosition;
        encoderTime =-timeElapsed;
    
        
        
        
    }



}

void ClosedLoopStepper::updateSerial()
{
    int numbytes =0;
    if(Serial.available()){
        numbytes  =Serial.readBytesUntil(0xff , serialData, 80);
    }
    
    if(numbytes>0)
    {
        int command = serialData[0];
        float target = serialData[1]*10000 +serialData[2]*100+serialData[3];
        
        if(command==1)
        {
        
          
            targetRotation =target/100.f-90.f;
            
            
#ifdef DEBUG_INPUT
            Serial.print("set Rotation :");
            Serial.println(targetRotation);
#endif
        }else if(command==2)
        {
            
            maxSpeed= target;
            //stepper.setTragetSpeed(maxSpeed);
#ifdef DEBUG_INPUT
            Serial.print("set MaxSpeed :");
            Serial.println(maxSpeed);
#endif
        }else if(command==4)
        {

            Kp= target/10.f;
#ifdef DEBUG_INPUT
            Serial.print("set damping :");
            Serial.println(Kp);
#endif
        }else if(command==5)
        {
            
            accel= target/10.f;
            Serial.print("set accel:");
            Serial.println(accel);
        }
        
    }
}