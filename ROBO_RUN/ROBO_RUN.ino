//initialize Servo motor
#include <Servo.h>
Servo Weapon;  // Servo name is Weapon
int Val;
int LiftCase;
int check = 1;
int button = 46;
int CheckTime;
int CurTime;

//initialize drive motors
const int SpeedPinR = 2;
const int SpeedPinL = 3;
const int Dir1R = 24;
const int Dir2R = 26;
const int Dir1L = 28;
const int Dir2L = 30;

//initialize IR sensors
const int IrRight = 29;  // Right side line detection
const int IrLeft = 25;   //Left side line detection
const int IrMid = 27;    // Mid line detection
const int IrFindL = 46;  // senses object left of robot
const int IrFindR = 44;  // senses object right of robot
const int IrFindB = 42;  // senses objects behind robot

//initialize Ultrasonic sensor
const int trigPin = 9;
const int echoPin = 10;
float duration, distance;

void setup() {
  Weapon.attach(9);
  pinMode(IrRight, INPUT);
  pinMode(IrLeft, INPUT);
  pinMode(IrMid, INPUT);
  pinMode(SpeedPinR, OUTPUT);
  pinMode(Dir1R, OUTPUT);
  pinMode(Dir2R, OUTPUT);
  pinMode(SpeedPinL, OUTPUT);
  pinMode(Dir1L, OUTPUT);
  pinMode(Dir2L, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(button, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  CheckTime = millis(); //Allows for button to start the program, moves robot forward for 0.3 seconds. 
  while (digitalRead(button) == 0) {
    CurTime = millis();
    if (CurTime - CheckTime > 100) {
      StraightF(300);
      check = 0;
    }
  }

//Checks condition of front three IR sensors.
  while (check == 0) {
    delay(10);
    if (digitalRead(IrLeft) == LOW && digitalRead(IrMid) == LOW && digitalRead(IrRight) == LOW) {
      RightTurn();
      Serial.println(1);
    } else if (digitalRead(IrLeft) == LOW && digitalRead(IrMid) == LOW && digitalRead(IrRight) == HIGH) {
      RightTurn();
      Serial.println(2);
    } else if (digitalRead(IrLeft) == LOW && digitalRead(IrMid) == HIGH && digitalRead(IrRight) == LOW) {
      LeftTurn();
      Serial.println(3);
    } else if (digitalRead(IrLeft) == LOW && digitalRead(IrMid) == HIGH && digitalRead(IrRight) == HIGH) {
      Stop();
      check = 1;
      Serial.println(4);
    } else if (digitalRead(IrLeft) == HIGH && digitalRead(IrMid) == LOW && digitalRead(IrRight) == LOW) {
      LeftTurn();
      Serial.println(5);
    } else if (digitalRead(IrLeft) == HIGH && digitalRead(IrMid) == LOW && digitalRead(IrRight) == HIGH) {
      Stop();
      check = 1;
      Serial.println(6);
    } else if (digitalRead(IrLeft) == HIGH && digitalRead(IrMid) == HIGH && digitalRead(IrRight) == LOW) {
      Left90(1100);
      Serial.println(7);
    } else if (digitalRead(IrLeft) == HIGH && digitalRead(IrMid) == HIGH && digitalRead(IrRight) == HIGH) {
      Stop();
      check = 1;
      Serial.println(8);
    }
  }
}

//Functions
//--------------------------------------------------
void LeftFor() {
  digitalWrite(Dir1L, HIGH);
  digitalWrite(Dir2L, LOW);
}
void LeftBack() {
  digitalWrite(Dir1L, LOW);
  digitalWrite(Dir2L, HIGH);
}
void RightFor() {
  digitalWrite(Dir1R, HIGH);
  digitalWrite(Dir2R, LOW);
}
void Stop() {
  analogWrite(SpeedPinR, 0);
  analogWrite(SpeedPinL, 0);
}
void RightBack() {
  digitalWrite(Dir1R, LOW);
  digitalWrite(Dir2R, HIGH);
}
void Left90(int time) {
  RightFor();
  LeftBack();
  analogWrite(SpeedPinR, 255);
  analogWrite(SpeedPinL, 255);
  delay(time);
}
void Right90(int time) {
  RightBack();
  LeftFor();
  analogWrite(SpeedPinR, 255);
  analogWrite(SpeedPinL, 255);
  delay(time);
}
void LeftTurn() {
  LeftFor();
  RightFor();
  analogWrite(SpeedPinR, 255);
  analogWrite(SpeedPinL, 0);
}
void RightTurn() {
  LeftFor();
  RightFor();
  analogWrite(SpeedPinR, 0);
  analogWrite(SpeedPinL, 255);
}
void StraightF(int time) {
  LeftFor();
  RightFor();
  analogWrite(SpeedPinR, 255);
  analogWrite(SpeedPinL, 255);
  delay(time);
}
void StraightB() {
  LeftBack();
  RightBack();
  analogWrite(SpeedPinR, 255);
  analogWrite(SpeedPinL, 255);
}
float ReadDist() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration * .0343) / 2;  //multipy duration by speed of sound and divide by 2 because it has to travel the distance twice
  return (distance);
}
void ActuateWeapon() {
  if (ReadDist() < 9) {
    Val = map(100, 0, 100, 0, 180);
    Weapon.write(Val);
    LiftCase = 1;
  } else {
    Val = map(0, 0, 100, 0, 180);
    Weapon.write(Val);
    LiftCase = 0;
  }
  return (LiftCase);
}