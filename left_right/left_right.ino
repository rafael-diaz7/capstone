// Direction pin
#define X_DIR 7

// Step pin
#define X_STP 4
// Enable pin
#define EN 8

void setup() {
  // put your setup code here, to run once:
  Serial.begin (9600);
  pinMode(X_DIR, OUTPUT);
  pinMode(X_STP, OUTPUT);
  pinMode(EN, OUTPUT);
  digitalWrite(EN,LOW);
}

void loop() {

  int steps = 500;
  int nLoops = 5;
  for(int i=0; i< nLoops; i++){
    Serial.println("Loop");
    Serial.println(i);
    
    digitalWrite(X_DIR, HIGH);
    for (int j = 0; j < steps; j++){
      digitalWrite(X_STP, HIGH);
      delayMicroseconds(1500);
      digitalWrite(X_STP, LOW);
    }
    delayMicroseconds(100);
    
    digitalWrite(X_DIR, LOW);
    for (int j = 0; j < steps; j++){
      digitalWrite(X_STP, HIGH);
      delayMicroseconds(1500);
      digitalWrite(X_STP, LOW);
    }
  }
}
