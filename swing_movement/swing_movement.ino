#include <AccelStepper.h>

// pins for stepper motor
#define X_DIR 7
#define X_STP 4
#define EN 8

// pins for encoder
#define A_PULSE 5
#define B_PULSE 2
#define Z_PULSE 3

// angle calculator
int currState;
int lastState;
int counter = 0;
// stepper motor w acceleration
AccelStepper stepper(AccelStepper::DRIVER, X_STP, X_DIR);
// encoder info
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
  
  lastState = digitalRead(A_PULSE);
  
  stepper.setEnablePin(4);
  // need to finetune accel && maxspeed
  stepper.setMaxSpeed(750000);
  stepper.setAcceleration(60000);
  stepper.moveTo(2500);
  stepper.run();
}
 
void loop()
{
  readEncoder(&encoderInfo);
  // if angle is not PID ready, swing up, else PID & balance
  swingUp();
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

/**
 * Swing up for the pendulum
 * 
 * Moves left or right depending on current direction of the swing
 */
void swingUp(){
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
  Serial.println(stepper.speed());
}

void balance() {
  return;
}
