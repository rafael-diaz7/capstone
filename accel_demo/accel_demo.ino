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

 int accels [] = {100, 1000, 2500, 5000, 10000, 25000, 50000};

 void setup()
 {  
   pinMode(X_DIR, OUTPUT);
   pinMode(X_STP, OUTPUT);
   pinMode(EN, OUTPUT);
   digitalWrite(EN, LOW);

   Serial.begin(115200);
   stepper.setEnablePin(4);
 }

 void loop()
 {
  for (int i : accels){
    displayAccel(i);
   }
 }

void displayAccel(int acceleration){
  stepper.setAcceleration(acceleration);
  stepper.runToNewPosition(-2500);
  stepper.runToNewPosition(0);
}
