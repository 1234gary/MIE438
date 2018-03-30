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
  myStepper.setSpeed(6.5);
//    motors.setM1Speed(-100);
//    
//    motors.setM2Speed(-100);
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
int EPS = 10;
bool isFire = true;
int threshold = 0;
const double P = 5.0;
const double I = 0.0;
const double D = 0.0;
int error = 100;

void loop(){
  // scanning for fire
//  while (error > EPS){
    error = scanForFire(position);
//    wheelPID(error, P, I, D);
//  }
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
  int fireDirection;
  
  while(true){
    sensorRight = analogRead(A1);
    sensorMiddle = analogRead(A2);
    sensorLeft = analogRead(A3);

    if ((sensorMiddle > sensorRight * 1.4)&&(sensorMiddle > sensorLeft * 1.4)){
//      return stepPosition;
continue;
    }
    
    if (sensorRight > sensorLeft){
      fireDirection = LEFT;
    }else{
      fireDirection = RIGHT;
    }
    
    stepPosition = 50*fireDirection+stepPosition;
    myStepper.step(50*fireDirection);
    delay(500);
    
    Serial.println("____________Sensor Values____________");
    Serial.print("Left Sensor: ");
    Serial.println(sensorLeft);
    Serial.print("Middle Sensor: ");
    Serial.println(sensorMiddle);
    Serial.print("Right Sensor: ");
    Serial.println(sensorRight);
    Serial.print("Position:");
    Serial.println(position);
    }
}



void wheelPID(int error, double P, double I, double D) {
  int lastError;
  if (error > 0){
    return;
  }
  else {
    return;
  }
  
}
// Repeat:
// 1. align with the stepper until left and right sensor are low and middle sensor is high
// 2. move forward until desired sensor value is reached
// 3. if outside sensors go high, then repeat 1-2.

void alignFlame(){
}

void moveForward(int desiredPosition){
  
}

