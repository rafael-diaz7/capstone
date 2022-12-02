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
int counter = 0;


/**
 * code here runs once during setup of arduino
 */
void setup() {
  Serial.begin(115200);
  pinMode(X_DIR, OUTPUT);
  pinMode(X_STP, OUTPUT);
  pinMode(EN, OUTPUT);
  digitalWrite(EN, LOW);
  lastState = digitalRead(A_PULSE);
}

/**
 * code here will run infinitely unless exited
 */
void loop() {
  for (int i = 0; i < 20; i++){
    int temp = digitalRead(i);
    if (temp){
      Serial.println(i);
    }
  }
  delay(2000);
//  readEncoder();
}

/**
 * function to move the cart
 * params:
 *  int dir: direcition... LOW == right, HIGH == left
 *  int steps: number of steps to take
 *  int dly: delay[ms] between each step (determines speed)
 */
void moveCart(int dir, int steps, int dly){
  digitalWrite(X_DIR, dir);
    for (int j = 0; j < steps; j++){
      // high then low cause step
      digitalWrite(X_STP, HIGH);
      delayMicroseconds(dly);
      digitalWrite(X_STP, LOW);
    }
}

/**
 * Reads value from the encoder and finds the angle
 * 
 * Will update counter and angle variable
 */
void readEncoder(){
  currState = digitalRead(A_PULSE);
  if (currState != lastState){
    if (digitalRead(B_PULSE) != currState) { counter++; }
    else { counter--; }
  }
  lastState = currState;
  if (counter % 8 == 0){ Serial.println((counter/2%1024)/1024.0*360.0); }
}
