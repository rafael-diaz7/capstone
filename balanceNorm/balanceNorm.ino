#include <AccelStepper.h>

// pins for stepper motor (new stepper motor = TMC2208)
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


// OSCILLATION WHEN Ku = 35, TU = 0.5
// PID
const float P0 = 1;
const float I0 = .2;
const float D0 = .35;
float pastTime;
float lastAngle;
int locationPID;

void setup()
{  
  pinMode(X_DIR, OUTPUT);
  pinMode(X_STP, OUTPUT);
  pinMode(EN, OUTPUT);
  digitalWrite(EN, LOW);
  Serial.begin(115200);
  
  lastState = digitalRead(A_PULSE);
  pastTime = micros();
  
  stepper.setEnablePin(4);
  stepper.setMaxSpeed(500000);
  stepper.setAcceleration(40000);
  
  //stepper.moveTo(-2500);
}

/**
 * updates known angle
 * checks angle, balance or swing
 * run the motor to the new destination
 */
void loop()
{
  readEncoder();
  if (150 < encoderInfo.angle && encoderInfo.angle < 210) {
    locationPID = PID();
    balance();
  }
//  else{
//    swingUp();
//  }
   stepper.run();
}

/**
 * Reads value from the encoder and finds the angle
 * 
 * Will update counter and angle variable
 * 
 * param EncoderInfo: encoderInfo struct containing angle and
 */
void readEncoder(){
  currState = digitalRead(A_PULSE);
  if (currState != lastState){
    encoderInfo.dirChange = true;
    if (digitalRead(B_PULSE) != currState) { 
      counter++; 
      encoderInfo.clockwise = true; // true
      }
    else { 
      counter--;
      encoderInfo.clockwise = false; // false 
      }
  }
  lastState = currState;
  encoderInfo.angle = abs(counter/2%1024/1024.0*360.0);
}

/**
 * Swing up for the pendulum
 * 
 * Moves left or right depending on current direction of the swing
 */
void swingUp(){
  if (encoderInfo.dirChange && encoderInfo.angle == 0.0){
    if (encoderInfo.clockwise) {
      stepper.moveTo(-2500);
    }
    else {
      stepper.moveTo(0);
    }
    encoderInfo.dirChange = false;
  }
}

/**
 * gets PID value
 * determines new location
 * if valid, set new location
 */
void balance() {
  float newLocation = stepper.currentPosition() + locationPID;
  if (-2500 < newLocation  && newLocation< 0){// && indexer % 5 == 0){
    stepper.moveTo(newLocation);
  }
}

float PID() {
  // delta theta
  float theta_error = 180 - encoderInfo.angle;
  float theta_error_sqr = theta_error * fabs(theta_error);
  float delta_t = micros() - pastTime;
  pastTime = micros();
  
  float integral = theta_error_sqr * delta_t;
  float derivative = (encoderInfo.angle - lastAngle) / delta_t;

  lastAngle = encoderInfo.angle;

  return P0 * theta_error_sqr + I0 * integral + D0 * derivative;
  
}
