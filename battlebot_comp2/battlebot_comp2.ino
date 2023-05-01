//initialize Ultrasonic sensor
const int trigPin = 9;
const int echoPin = 10;
float duration, distance;
int Var;
int CheckTime;
int CurTime;
double track_width;
double wheel_circumference;
int max_rpm = 100;

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
uint32_t hstimer =0;
float randInt = 0;
int counter = 0;
//deltat macro
#define DELTAT(_nowtime, _thentime) ((_thentime > _nowtime) ? ((0xffffffff - _thentime) + _nowtime) : (_nowtime - _thentime))


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
  double track_width = 0.3; //m
  double wheel_circumference = 0.2042039120408256;  //m
  Serial.begin(9600);
}

void loop() {
  // 1 => line detected
  bool IrArray[3] = {digitalRead(IrLeft), digitalRead(IrMid), digitalRead(IrRight)};
  // {left, mid, right}
  //get time
  uint32_t nowtime = millis();
  switch (ReadDist()) {
    
    case 1:
    // object close-> attack until out of bounds
      if (!IrArray[0] && !IrArray[1] && !IrArray[2]){
        cmdvel(1,0);
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
        cmdvel(1,0);
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
        // if deltaT > 1 s, send random int again
       if (DELTAT(nowtime,hstimer) >= 220)
      {
        hstimer = nowtime;
        //float randInt1 = random(0,1);
        int switch_cmd  = counter%2;
        if (switch_cmd == 0){
          
          int posOrNeg = random(0,10);
          if (posOrNeg > 5){
            randInt = random(70,150);
            }
           else{
              randInt = -random(70,150);
             }
        }
        else{
          randInt = 0;
         }
        counter++;
        Serial.println("random: ");
        Serial.println(randInt);
        cmdvel(switch_cmd,randInt/100);
      }
        }
      else{
          //function to determine how to avoid boundary
          avoidBound(IrArray);
          }
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
  } else if (distance >= 10 && distance < 70) {
    Var = 2;
  } else if (distance >= 70) {
    Var = 3;
  }
  return (Var);
}
/*
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
*/
/// ---------------
// cmdvel

void cmdvel(float lin, float ang){

    float max_lin_speed = 1.0; // Replace with actual value
    float max_ang_speed = 1; // Replace with actual value
    float scaled_lin_speed = lin / max_lin_speed;
    float scaled_ang_speed = ang / max_ang_speed;
    Serial.println("lin: ");
        Serial.println(scaled_lin_speed);
    
     

    // Calculate motor power commands
    int32_t right_power = (scaled_lin_speed + scaled_ang_speed) * 255.0;
    int32_t left_power = (scaled_lin_speed - scaled_ang_speed) * 255.0;
    

    // Limit motor power commands to between 0 and 255
    right_power = constrain(right_power, -255, 255);
    left_power = constrain(left_power, -255, 255);
    /*
     Serial.println("right_power: ");
        Serial.println(right_power);
        Serial.println("left_power: ");
        Serial.println(left_power);
    */
        //send right command
        
          
         analogWrite(SpeedPinR, abs(right_power));
         if (right_power < 0){
            digitalWrite(Dir1R, LOW);
            digitalWrite(Dir2R, HIGH);
            Serial.println("right_power < 0: ");
    }
          else{
            digitalWrite(Dir1R, HIGH);
            digitalWrite(Dir2R, LOW);
            Serial.println("right_power > 0: ");
            }
        //send left command
         analogWrite(SpeedPinL, abs(left_power));
         if (left_power < 0){
            digitalWrite(Dir1L, LOW);
            digitalWrite(Dir2L, HIGH);
            Serial.println("left_power < 0: ");
          }
          else{
            digitalWrite(Dir1L, HIGH);
            digitalWrite(Dir2L, LOW);
            Serial.println("left_power > 0: ");
           }
        
      
        
}

void avoidBound(bool IrArray[3]) {
  // convert to binary number
  int binary = IrArray[2] + 2 * IrArray[1] + 4 * IrArray[0];

  switch (binary) {
    // 0,0,0
    case 0: {
      cmdvel(-1,0);
      break;
    }
    //0,0,1
    case 1: {
      Serial.println("turn left 90*");
      cmdvel(-1.0,0);
      delay(400);
      cmdvel(0,1);
      delay(random(1800,2200));
      cmdvel(1,0);
      delay(800);
      break;
    }
    // 0,1,0
    case 2: {
      //ReverseAndSpin();
      cmdvel(-1.0,0);
      delay(700);
      cmdvel(0,1);
      delay(2000);
      cmdvel(1,0);
      delay(800);
      break;
      
      Serial.println("back up & 180*");
      
    }
    // 0,1,1
    case 3: {
      cmdvel(-1.0,0);
      delay(600);
      cmdvel(0,1);
      delay(random(1800,2200));
      cmdvel(1,0);
      delay(1000);
      Serial.println("turn left 120*");
      break;

    }
    //1,0,0
    case 4: {
      // turn right
      cmdvel(-1.0,0);
      delay(600);
      cmdvel(0.0,-1.0);
      delay(2300);
      cmdvel(1,0);
      delay(1000);
      Serial.println("turn left 90*");
      break;
    }
    // 1,0,1
    case 5: {
      Serial.println("180*?");
      cmdvel(-1,0);
      delay(1000);
      cmdvel(0,-1);
      delay(1500);
      cmdvel(1,0);
      delay(400);
      break;
    }
    // 1,1,0
    case 6: {
      cmdvel(-1,0);
      delay(500);
      cmdvel(0.0,-1.0);
      delay(2300);
      cmdvel(1.0,0.0);
      Serial.println("180*");
      
      break;
    }
    default: {
      Serial.println("Invalid IR sensor values");
      break;
    }
  }
}
  
