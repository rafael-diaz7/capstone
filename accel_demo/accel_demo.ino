#include <AccelStepper.h>

 //driver is A4988
 //shield: Generic Arduino Uno CNC Shield

 // direction pi
 #define X_DIR 7
 // step pin
 #define X_STP 4
 //enable pin
 #define EN 8

 AccelStepper stepper(AccelStepper::DRIVER, X_STP, X_DIR);

 void setup()
 {  
   pinMode(X_DIR, OUTPUT);
   pinMode(X_STP, OUTPUT);
   pinMode(EN, OUTPUT);
   digitalWrite(EN, LOW);

   Serial.begin(115200);
   stepper.setEnablePin(4);
   stepper.setMaxSpeed(500000);
   stepper.setAcceleration(40000);
   stepper.moveTo(-500);
 }

 void loop()
 {
  //Serial.println(stepper.distanceToGo());
   // If the cart reaches the end, return to the starting position
   if (stepper.distanceToGo() == 0)
     stepper.moveTo(0);

   stepper.run();
 }
