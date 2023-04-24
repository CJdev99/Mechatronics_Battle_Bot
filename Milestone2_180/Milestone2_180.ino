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

void setup() {
  pinMode(IrRight, INPUT);
  pinMode(IrLeft, INPUT);
  pinMode(IrMid, INPUT);
  pinMode(SpeedPinR, OUTPUT);
  pinMode(Dir1R, OUTPUT);
  pinMode(Dir2R, OUTPUT);
  pinMode(SpeedPinL, OUTPUT);
  pinMode(Dir1L, OUTPUT);
  pinMode(Dir2L, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  CheckTime = millis(); //Allows for button to start the program
  while (digitalRead(button) == 0) {
    CurTime = millis();
    if (CurTime - CheckTime > 100) {
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
      LeftTurn();
      Serial.println(2);
    } else if (digitalRead(IrLeft) == LOW && digitalRead(IrMid) == HIGH && digitalRead(IrRight) == LOW) {
      LeftTurn();
      Serial.println(3);
    } else if (digitalRead(IrLeft) == LOW && digitalRead(IrMid) == HIGH && digitalRead(IrRight) == HIGH) {
      Left180(2600);
      Serial.println(4);
    } else if (digitalRead(IrLeft) == HIGH && digitalRead(IrMid) == LOW && digitalRead(IrRight) == LOW) {
      RightTurn();
      Serial.println(5);
    } else if (digitalRead(IrLeft) == HIGH && digitalRead(IrMid) == LOW && digitalRead(IrRight) == HIGH) {
      Left180(2600);
      Serial.println(6);
    } else if (digitalRead(IrLeft) == HIGH && digitalRead(IrMid) == HIGH && digitalRead(IrRight) == LOW) {
      Left180(2600);
      Serial.println(7);
    } else if (digitalRead(IrLeft) == HIGH && digitalRead(IrMid) == HIGH && digitalRead(IrRight) == HIGH) {
      Left180(2600);
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
void Left180(int time) {
  RightFor();
  LeftBack();
  analogWrite(SpeedPinR, 255);
  analogWrite(SpeedPinL, 185);
  delay(time);
}
void Right180(int time) {
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