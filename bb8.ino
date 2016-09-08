int m1 = 2;
int m2 = 4;
int e1 = 3;
int e2 = 6;
int m3 = 5;
int m4 = 7;

void setup() {
  Serial.begin(9600);
  pinMode( 13 , OUTPUT);
  pinMode( m1 , OUTPUT);
  pinMode( m2 , OUTPUT);
  pinMode( m3 , OUTPUT);
  pinMode( m4 , OUTPUT);
  pinMode( e1 , OUTPUT);
  pinMode( e2 , OUTPUT);
  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);
 
}

void loop() {
  while (Serial.available() > 0) {
    int R = Serial.parseInt();
    int L = Serial.parseInt();
    if (Serial.read() == '\n') {
      roll(R, L);
    }
    else {
      roll(0, 0);
    }
  }
}

void roll(int rm, int lm) {
  if (rm >= 0) {
    analogWrite(e1, rm);
    digitalWrite(m1, 1);
    digitalWrite(m2, 0);
  }
  else {
    rm = -rm;
    analogWrite(e1, rm);
    digitalWrite(m1, 0);
    digitalWrite(m2, 1);
  }
  if (lm >= 0) {
    analogWrite(e2, lm);
    digitalWrite(m3, 1);
    digitalWrite(m4, 0);
  }
  else {
    lm = -lm;
    analogWrite(e2, lm);
    digitalWrite(m3, 0);
    digitalWrite(m4, 1);
  }
}