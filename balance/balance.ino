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
volatile int counter = 0;

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
//const float KU = 3000;
//const float TU = 0.3;
//const float P0 = 0.45 * KU;
//const float I0 = 0.54 * KU / TU;

const float KU = 35;
const float TU = 0.5;
const float P0 = 0.6 * KU;
const float I0 = 1.2 * KU / TU;
const float D0 = 3 * KU * TU / 40;
int lastPID = 0;
//const float P0 = 25;
//const float I0 = 90;
//const float D0 = 1.8;

const float C0 = .01;
const int SAMPLE_SIZE = 20;
int indexer = 0;
float integrals[SAMPLE_SIZE];
float derivatives[SAMPLE_SIZE];
float pastTime;
float lastAngle;
float integralSum;
float derivativesSum;
float accelerationPID;
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

  attachInterrupt(digitalPinToInterrupt(B_PULSE), readEncoder, CHANGE);
  
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
//  locationPID = PID();
//  if (150 < encoderInfo.angle && encoderInfo.angle < 210) {
//    balance();
//  }
////  else{
////    swingUp();
////  }
//  stepper.run();
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
      encoderInfo.clockwise = true;
      }
    else { 
      counter--;
      encoderInfo.clockwise = false;      
      }
  }
  lastState = currState;
  encoderInfo.angle = abs(counter/2%1024/1024.0*360.0);
  if (counter % 8 == 0) {
    Serial.println(encoderInfo.angle);
  }
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
  // lowercase delta theta
  float theta_error = 180 - encoderInfo.angle;
  float theta_error_sqr = theta_error * fabs(theta_error);
  float delta_t = micros() - pastTime;
  pastTime = micros();
  
  // instead of summing with a loop, we save time by just always saving a sum and just subtracting and adding as needed
  integralSum -= integrals[indexer];
  integrals[indexer] = theta_error * delta_t;
  integralSum += integrals[indexer];

  derivativesSum -= derivatives[indexer];
  derivatives[indexer] = (encoderInfo.angle - lastAngle) / delta_t;
  derivativesSum += derivatives[indexer++];

  lastAngle = encoderInfo.angle;
  
  // check if we are out of bounds, if so, reset to 0
  if (indexer >= SAMPLE_SIZE){
    indexer = 0;
  }

  return  C0 * (P0 * theta_error + I0 * integralSum + D0 * derivativesSum);
  //return  C0 * P0 * theta_error_sqr;//C0 * (P0 * theta_error_sqr + I0 * integral + D0 * derivative);
  //return C0 * (P0 * theta_error_sqr + I0 * integral);// + D0 * derivative);
  
  
}
