#include <Servo.h>
#include <DRV8835MotorShield.h>
#include <X113647Stepper.h>
int PowerPin = 13; 

const int LEFT = -1;
const int RIGHT = 1;

static const int STEPS_PER_REVOLUTION = 64 * 32;  
X113647Stepper myStepper(STEPS_PER_REVOLUTION, 2, 3, 4, 5);
DRV8835MotorShield motors;
Servo myservo;

void setup() {
  // put your setup code here, to run once:
  pinMode(PowerPin, OUTPUT);     
  digitalWrite(PowerPin, HIGH);
  Serial.begin(9600);
  myStepper.setSpeed(8);
  motors.flipM1(true);
  motors.flipM2(true);
}

//void myservo.write(pos); where pos between (0, 180) to move servo
//void myStepper.step(steps); to move stepper
//void setM1Speed(int speed); to move motor shield wheels 
//void setM2Speed(int speed) 
//void setSpeeds(int m1Speed, int m2Speed) 
//void flipM1(bool flip) 
//void flipM2(bool flip) 
//Serial.println("Hello World")
//sensor_reading = analogRead(A1)

//void loop() {
//    myservo.write(180);              // tell servo to go to position in variable 'pos'
//    delay(500);                       // waits 15ms for the servo to reach the position
// 
//    myservo.write(0);              // tell servo to go to position in variable 'pos'
//    delay(500);            
//    
//}


int position = 0;
int EPS = 21;
bool isFire = true;
int threshold = 0;
const double P = 5.0;
const double I = 0.0;
const double D = 0.0;
int error = 0;

void loop(){
  int sensorRight = analogRead(A1);
  int sensorMiddle = analogRead(A2);
  int sensorLeft = analogRead(A3);
  int maxValue = max(sensorMiddle, sensorRight);
    maxValue = max(maxValue, sensorLeft);
    if (maxValue < 10)
      return;
      
   Serial.print("Middle Sensor: ");
    Serial.println(analogRead(A2));
  if (analogRead(A2) > 200){
    motors.setSpeeds(0, 0);
    return; 
  }
  // scanning for fire
  error = scanForFire(error);
  while (abs(error) > EPS){
    error = scanForFire(error);
    wheelPID(error, P, I, D);
  }
//  Serial.println("Final Error:");
//  Serial.println(error);

  motors.setSpeeds(200, 200);
  delay(300);
}

//void loop(){
//  while (isFire) {
//    newPosition = scanForFire(position);
//    wheelPID(newPosition, P, I, D);
//    threshold = moveForward(desiredPosition);
//    firePump();
//    isFire = checkFire(threshold);
//  }
//}

bool checkFire(int threshold){
  int sensorRight = analogRead(A1);
  int sensorMiddle = analogRead(A2);
  int sensorLeft = analogRead(A3);
  if (sensorRight > threshold || sensorLeft > threshold || sensorMiddle > threshold){
    return true;
  }
  return false;
}

int scanForFire(int stepPosition) {
  int sensorLeft, sensorMiddle, sensorRight;
  int fireDirection, stride;
  motors.setSpeeds(0, 0);
  while(true){
    sensorRight = analogRead(A1);
    sensorMiddle = analogRead(A2);
    sensorLeft = analogRead(A3);

     Serial.println("____________Sensor Values____________");
//    Serial.print("Left Sensor: ");
//    Serial.println(sensorLeft);
//    Serial.print("Middle Sensor: ");
//    Serial.println(sensorMiddle);
//    Serial.print("Right Sensor: ");
//    Serial.println(sensorRight);
    Serial.print("Position:");
    Serial.println(stepPosition);
  
    int maxValue = max(sensorMiddle, sensorRight);
    maxValue = max(maxValue, sensorLeft);
    if (maxValue < 10)
      continue;
    
    if ((sensorMiddle > sensorRight * 1.8)&&(sensorMiddle > sensorLeft * 1.8)){
      return stepPosition;
    }
    if (sensorRight > sensorLeft){
      fireDirection = LEFT;
    }else{
      fireDirection = RIGHT;
    }
//    if (abs(stepPosition) < 50){
//      stride = 10;
//    }else{
//      stride = 50;
//    }
stride = 10;
    
    stepPosition = stride*fireDirection+stepPosition;
    myStepper.step(stride*fireDirection);
    delay(10);
  }
}



void wheelPID(int error, double P, double I, double D) {
  int lastError;
  int m1Speed, m2Speed;
  if (error > 0){
    m1Speed = min(error * P, 200);
    m2Speed = max(-error * P, -200);
  }
  else {
    m1Speed = max(error * P, -200);
    m2Speed = min(-error * P, 200);
  }
  motors.setSpeeds(m1Speed, m2Speed);
  delay(50);
  motors.setSpeeds(0, 0);
}
// Repeat:
// 1. align with the stepper until left and right sensor are low and middle sensor is high
// 2. move forward until desired sensor value is reached
// 3. if outside sensors go high, then repeat 1-2.

void alignFlame(){
}

void moveForward(int desiredPosition){
  
}

