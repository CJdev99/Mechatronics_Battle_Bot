//initialize Ultrasonic sensor
const int trigPin = 9;
const int echoPin = 10;
float duration, distance;
int Var;
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
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  // 1 => line detected
  bool IrArray[3] = {digitalRead(IrLeft), digitalRead(IrMid), digitalRead(IrRight)};
  // {left, mid, right}
  
  switch (ReadDist()) {
    
    case 1:
    // object close-> attack until out of bounds
      if (!IrArray[0] && !IrArray[1] && !IrArray[2]){
        StraightF();
        }
      else{
          //function to determine how to avoid boundary
          avoidBound(IrArray);
          }
      Serial.println(1);
      break;
    case 2:
      //StraightF();
      // object detected -> drive to object
      if (!IrArray[0] && !IrArray[1] && !IrArray[2]){
        StraightF();
        }
      else{
          //function to determine how to avoid boundary
          avoidBound(IrArray);
          }
      Serial.println(2);
      break;
    case 3:
      // random movement
      if (!IrArray[0] && !IrArray[1] && !IrArray[2]){
        Spin();
        }
      else{
          //function to determine how to avoid boundary
          avoidBound(IrArray);
          }

      Serial.println(3);
  }
 
}

int ReadDist() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration * .0343) / 2;  //multipy duration by speed of sound and divide by 2 because it has to travel the distance twice
  //Serial.println(distance);
  if (distance < 10) {
    Var = 1;
  } else if (distance >= 10 && distance < 55) {
    Var = 2;
  } else if (distance >= 55) {
    Var = 3;
  }
  return (Var);
}
void Spin() {
  int check = 1;
  CheckTime = millis();  //Allows for button to start the program
  while (check == 1) {
    CurTime = millis();
    RightFor();
    LeftBack();
    analogWrite(SpeedPinR, 255);
    analogWrite(SpeedPinL, 255);
    Serial.println("Spin");
    ReadDist();
    if (ReadDist() == 1 || ReadDist() == 2) {
      check = 0;
    } else if (CurTime - CheckTime > 2000) {
      Serial.println("spinForward");
      RightFor();
      LeftFor();
      analogWrite(SpeedPinL, 255);
      analogWrite(SpeedPinR, 255);
      delay(500);
      check = 0;
    }
  }
}
/// ---------------
// cmdvel
/*
void cmdvel(float lin, float ang){
    float right_speed = lin + track_width * ang / 2.0;
    float left_speed = lin - track_width * ang / 2.0;
        // motor power (scale 0-1000)
        
        int32_t right_power = right_speed / wheel_circumference/max_rpm * 60.0 * 255;
        int32_t left_power = left_speed / wheel_circumference/max_rpm * 60.0 * 255;
        //send right command
         analogWrite(SpeedPinR, right_power);
         if (right_power < 0){
            digitalWrite(Dir1L, LOW);
          }
          else{
            digitalWrite(Dir1L, HIGH);
            }

        //send left command
         analogWrite(SpeedPinL, 255);
         if (left_power < 0){
            digitalWrite(Dir2L, LOW);
          }
          else{
            digitalWrite(Dir2L, HIGH);
           }
}
*/
// ----------------
void StraightF() {
  LeftFor();
  RightFor();
  analogWrite(SpeedPinR, 255);
  analogWrite(SpeedPinL, 255);
}
void BackF() {
  LeftBack();
  RightBack();
  analogWrite(SpeedPinR, 255);
  analogWrite(SpeedPinL, 255);
  
}
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
void avoidBound(bool IrArray[3]) {
  // convert to binary number
  int binary = IrArray[2] + 2 * IrArray[1] + 4 * IrArray[0];

  switch (binary) {
    // 0,0,0
    case 0: {
      StraightF();
      break;
    }
    //0,0,1
    case 1: {
      Serial.println("turn left 90*");
      StraightF();
      LeftBack();
      delay(1100);
      StraightF();
      break;
    }
    // 0,1,0
    case 2: {
      //ReverseAndSpin();
      
      BackF();
      delay(500);
      StraightF();
      RightFor();
      LeftBack();
      delay(2000);
      StraightF();
      
      Serial.println("back up & 180*");
      break;
    }
    // 0,1,1
    case 3: {
      BackF();
      delay(250);
      StraightF();
      LeftBack();
      RightFor();
      delay(1200);
      StraightF();
      Serial.println("turn left 120*");
      break;

    }
    //1,0,0
    case 4: {
      // turn right
      StraightF();
      LeftFor();
      RightBack();
      delay(1000);
      StraightF();
      delay(100);
      Serial.println("turn left 90*");
      break;
    }
    // 1,0,1
    case 5: {
      Serial.println("180*?");
      BackF();
      delay(500);
      StraightF();
      RightFor();
      LeftBack();
      delay(1250);
      break;
    }
    // 1,1,0
    case 6: {
      Stop();
      BackF();
      delay(500);
      StraightF();
      RightFor();
      LeftBack();
      delay(1200);
      StraightF();
      Serial.println("180*");
      
      break;
    }
    default: {
      Serial.println("Invalid IR sensor values");
      break;
    }
  }
}
  
