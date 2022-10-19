// direction pi
#define X_DIR 7
// step pin
#define X_STP 4
//enable pin
#define EN 8

#define Z_PULSE 3

int currState;
int lastState;
int counter = 0;


void setup() {
  Serial.begin(115200);
  pinMode(X_DIR, OUTPUT);
  pinMode(X_STP, OUTPUT);
  pinMode(EN, OUTPUT);
  pinMode(Z_PULSE, OUTPUT);
  digitalWrite(EN, LOW);
  lastState = digitalRead(5);
}

//void loop(){
//  int l = digitalRead(5);
//  int r = digitalRead(2);
//  // technically this movement part is only until it reaches a point where balancing can occur - refactor later
//  // l == r if going clockwise
//  if (l == r) {
//    moveCart(LOW, 500, 1500);
//    delayMicroseconds(1000);
//  }
//  else { moveCart(HIGH, 500, 1500); delayMicroseconds(1000); }
//}

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

void loop() {
  currState = digitalRead(5);
  if (currState != lastState){
    if (digitalRead(2) != currState){
      counter++;
    }
    else { counter--; }
//    Serial.print("position: ");
//    Serial.print(counter);
//    Serial.print("\tangle: ");
    if (counter%4 == 0) {
      Serial.println(((counter%1024)/1024.0*360.0));
    }
  }
  lastState = currState;
}
