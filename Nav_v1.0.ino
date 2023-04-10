// Encoder output to Arduino Interrupt pin. Tracks the tick count.
#define ENC_IN_LEFT_A 2
#define ENC_IN_RIGHT_A 3
 
// Other encoder output to Arduino to keep track of wheel direction
// Tracks the direction of rotation.
#define ENC_IN_LEFT_B 4
#define ENC_IN_RIGHT_B 11
 
// True = Forward; False = Reverse
boolean Direction_left = true;
boolean Direction_right = true;
 
// Minumum and maximum values for 16-bit integers
const int encoder_minimum = -32768;
const int encoder_maximum = 32767;
 
// Keep track of the number of wheel ticks
volatile int left_wheel_tick_count = 0;
volatile int right_wheel_tick_count = 0;
 
// One-second interval for measurements
int interval = 1000;
long previousMillis = 0;
long currentMillis = 0;

uint32_t starttime = 0;
uint32_t hstimer = 0;
uint32_t mstimer = 0;
unsigned int odom_idx = 0;
char odom_encoder_toss = 5;
uint32_t odom_encoder_left = 0;
uint32_t odom_encoder_right = 0;
float odom_x = 0.0;
float odom_y = 0.0;
float odom_yaw = 0.0;
float odom_last_x = 0.0;
float odom_last_y = 0.0;
float odom_last_yaw = 0.0;
float odom_last_time = 0;
int encoder_cpr = 4096;
int encoder_ppr = 1024;
double track_width = 0.15; //m
double wheel_circumference = 0.4; 
#define DELTAT(_nowtime, _thentime) ((_thentime > _nowtime) ? ((0xffffffff - _thentime) + _nowtime) : (_nowtime - _thentime))
#define NORMALIZE(_z) atan2(sin(_z), cos(_z))
void setup() {
 
  // Open the serial port at 9600 bps
  Serial.begin(9600); 
  // initialize variables

  // Set pin states of the encoder
  pinMode(ENC_IN_LEFT_A , INPUT_PULLUP);
  pinMode(ENC_IN_LEFT_B , INPUT);
  pinMode(ENC_IN_RIGHT_A , INPUT_PULLUP);
  pinMode(ENC_IN_RIGHT_B , INPUT);
 
  // Every time the pin goes high, this is a tick
  attachInterrupt(digitalPinToInterrupt(ENC_IN_LEFT_A), left_wheel_tick, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC_IN_RIGHT_A), right_wheel_tick, RISING);
}
 
void loop() {
 
  // Record the time
  currentMillis = millis();
 
  // If one second has passed, print the number of ticks
  if (currentMillis - previousMillis > interval) {
     
    previousMillis = currentMillis;
 
    Serial.println("Number of Ticks: ");
    Serial.println(right_wheel_tick_count);
    Serial.println(left_wheel_tick_count);
    Serial.println();
  }
}
 
// Increment the number of ticks
void right_wheel_tick() {
   
  // Read the value for the encoder for the right wheel
  int val = digitalRead(ENC_IN_RIGHT_B);
 
  if(val == LOW) {
    Direction_right = false; // Reverse
  }
  else {
    Direction_right = true; // Forward
  }
   
  if (Direction_right) {
     
    if (right_wheel_tick_count == encoder_maximum) {
      right_wheel_tick_count = encoder_minimum;
    }
    else {
      right_wheel_tick_count++;  
    }    
  }
  else {
    if (right_wheel_tick_count == encoder_minimum) {
      right_wheel_tick_count = encoder_maximum;
    }
    else {
      right_wheel_tick_count--;  
    }   
  }
}
 
// Increment the number of ticks
void left_wheel_tick() {
   
  // Read the value for the encoder for the left wheel
  int val = digitalRead(ENC_IN_LEFT_B);
 
  if(val == LOW) {
    Direction_left = true; // Reverse
  }
  else {
    Direction_left = false; // Forward
  }
   
  if (Direction_left) {
    if (left_wheel_tick_count == encoder_maximum) {
      left_wheel_tick_count = encoder_minimum;
    }
    else {
      left_wheel_tick_count++;  
    }  
  }
  else {
    if (left_wheel_tick_count == encoder_minimum) {
      left_wheel_tick_count = encoder_maximum;
    }
    else {
      left_wheel_tick_count--;  
    }   
  }
}

// calculate wheel velocity and distance traveled:

void odom_publish()
{

    // determine delta time in seconds
    uint32_t nowtime = millis();
    float dt = (float)DELTAT(nowtime, odom_last_time) / 1000.0;
    odom_last_time = nowtime;

    // determine deltas of distance and angle
    float linear = ((float)odom_encoder_right / (float)encoder_cpr * wheel_circumference + (float)odom_encoder_left / (float)encoder_cpr * wheel_circumference) / 2.0;
    //  float angular = ((float)odom_encoder_right / (float)encoder_cpr * wheel_circumference - (float)odom_encoder_left / (float)encoder_cpr * wheel_circumference) / track_width * -1.0;
    float angular = ((float)odom_encoder_right / (float)encoder_cpr * wheel_circumference - (float)odom_encoder_left / (float)encoder_cpr * wheel_circumference) / track_width;

    // Update odometry
    odom_x += linear * cos(odom_yaw);         // m
    odom_y += linear * sin(odom_yaw);         // m
    odom_yaw = NORMALIZE(odom_yaw + angular); // rad

    // Calculate velocities
    float vx = (odom_x - odom_last_x) / dt;
    float vy = (odom_y - odom_last_y) / dt;
    float vyaw = (odom_yaw - odom_last_yaw) / dt;

    odom_last_x = odom_x;
    odom_last_y = odom_y;
    odom_last_yaw = odom_yaw;
}
