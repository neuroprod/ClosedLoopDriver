#include <ClosedLoopStepper.h>
#include <StepperSpeedControler.h>

#define encoder0PinA 2
#define encoder0PinB 3
volatile  int encoder0Pos = 0;
int encoder0PosCopy = 0;

unsigned int Aold = 0;
unsigned int Bnew = 0;

ClosedLoopStepper cls;

void setup() {
  cls.setup();
  // put your setup code here, to run once:
  pinMode(encoder0PinA, INPUT); 
  pinMode(encoder0PinB, INPUT);
  // encoder pin on interrupt 0 (pin 2)
  attachInterrupt(encoder0PinA, doEncoderA, CHANGE);
  // encoder pin on interrupt 1 (pin 3)
  attachInterrupt(encoder0PinB, doEncoderB, CHANGE);
  //turn on pullup resistors
  digitalWrite(encoder0PinA, HIGH);
  digitalWrite(encoder0PinB, HIGH);
}

void loop() {
  
  // put your main code here, to run repeatedly: 
   noInterrupts();
  encoder0PosCopy = encoder0Pos;
  interrupts();
  if (cls.encoderPosition != encoder0PosCopy) {
 //  Serial.println(encoder0PosCopy, DEC);
    cls.encoderPosition = encoder0PosCopy;
    
  }
  cls.loop();
}

// Interrupt on A changing state
void doEncoderA(){
  Bnew^Aold ? encoder0Pos++:encoder0Pos--;
  Aold=digitalRead(encoder0PinA);
}
// Interrupt on B changing state
void doEncoderB(){
  Bnew=digitalRead(encoder0PinB);
  Bnew^Aold ? encoder0Pos++:encoder0Pos--;
}
