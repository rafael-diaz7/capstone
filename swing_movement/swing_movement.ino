#include <AccelStepper.h>

// direction pi
#define X_DIR 7
// step pin
#define X_STP 4
//enable pin
#define EN 8

// pins for encoder
#define A_PULSE 5
#define B_PULSE 2
#define Z_PULSE 3

int currState;
int lastState;
float angle;
bool dirChange = false;
int counter = 0;
AccelStepper stepper(AccelStepper::DRIVER, X_STP, X_DIR);
struct EncoderInfo {
  float angle;
  bool dirChange;
  bool clockwise;
} encoderInfo;

void setup()
{  
  pinMode(X_DIR, OUTPUT);
  pinMode(X_STP, OUTPUT);
  pinMode(EN, OUTPUT);
  digitalWrite(EN, LOW);
  
  Serial.begin(115200);
  encoderInfo.angle = 0.0;
  encoderInfo.dirChange = false;
  encoderInfo.clockwise = false;
  lastState = digitalRead(A_PULSE);
  stepper.setEnablePin(4);
  stepper.setMaxSpeed(750000);
  stepper.setAcceleration(60000);
  stepper.moveTo(2500);
  stepper.run();
}
 
void loop()
{
  readEncoder(&encoderInfo);
  if (encoderInfo.dirChange){
    if (encoderInfo.clockwise) {
      stepper.moveTo(2500);
    }
    else {
      stepper.moveTo(0);
    }
    encoderInfo.dirChange = false;
  }
  stepper.run();
}

/**
 * Reads value from the encoder and finds the angle
 * 
 * Will update counter and angle variable
 * 
 * param EncoderInfo: encoderInfo struct containing angle and
 */
void readEncoder(EncoderInfo *encoderInfo){
  currState = digitalRead(A_PULSE);
  if (currState != lastState){
    encoderInfo->dirChange = true;
    if (digitalRead(B_PULSE) != currState) { 
      counter++; 
      encoderInfo->clockwise = true;
      }
    else { 
      counter--;
      encoderInfo->clockwise = false;      
      }
  }
  lastState = currState;
  encoderInfo->angle = counter%1024/1024.0*360.0;
}
