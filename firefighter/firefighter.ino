#include <Servo.h>
#include <DRV8835MotorShield.h>
#include <X113647Stepper.h>

int PowerPin = 13; 
int PumpPin = 12;
const int LEFT = -1;
const int RIGHT = 1;

static const int STEPS_PER_REVOLUTION = 64 * 32;  
X113647Stepper myStepper(STEPS_PER_REVOLUTION, 2, 3, 4, 5);
DRV8835MotorShield motors;
Servo myservo;

// Set up pin assignments and serial
void setup() {
  pinMode(PowerPin, OUTPUT);     
  digitalWrite(PowerPin, HIGH);
  pinMode(PumpPin, OUTPUT);   
  Serial.begin(9600);
  myStepper.setSpeed(8);
  motors.flipM1(true);
  motors.flipM2(true);
}

int EPS = 2;
const double P = 20.0;
const double I = 0.0;
const double D = 20.0;
int error = 0;

// Align cannon and robot towards the fire, then move forward towards the fire
// while the cannon and robot remain aligned. If the fire threshold is large enough
// put out the fire.
void loop(){
  takeOutFire();
  error = scanForFire(error);
  while (abs(error) > EPS){
    error = scanForFire(error);
    wheelPID(error);
  }
  motors.setSpeeds(100, 100);
  delay(100);
}

// Align the cannon towards the fire. If no fire is detected, pivot around the
// same spot to search 360 degrees around the robot.
int scanForFire(int stepPosition) {
  int sensorLeft, sensorMiddle, sensorRight;
  int fireDirection, stride;
  motors.setSpeeds(0, 0);
  while(true){
    sensorRight = analogRead(A3);
    sensorMiddle = analogRead(A4);
    sensorLeft = analogRead(A5);

    Serial.print("Position:");
    Serial.println(stepPosition);
    Serial.println(sensorLeft);
    Serial.println(sensorMiddle);
    Serial.println(sensorRight);

    // Check for minimal fire threshold
    int maxValue = max(max(sensorMiddle, sensorRight), sensorLeft);
    if (maxValue < 10){
      motors.setSpeeds(80, -80);
      delay(250);
      motors.setSpeeds(0, 0);
      delay(250);
      continue;
    }
    
    takeOutFire();

    // Check if middle sensor is aligned towards fire
    if ((sensorMiddle > sensorRight * 1.4)&&(sensorMiddle > sensorLeft * 1.4)){
      return stepPosition;
    }

    // Check which direction to move in
    fireDirection = (sensorRight > sensorLeft)?LEFT:RIGHT;
    stride = (sensorRight > sensorLeft)?40:49;

    // Move towards fire
    stepPosition += fireDirection;
    myStepper.step(stride*fireDirection);
    delay(50);
  }
}

// Align the robot to the cannon direction with PID control
int derivative;
int lastError = 0;
void wheelPID(int error) {
  int m1Speed, m2Speed, gain, turnDir;
  derivative = error - lastError;
  lastError = error;

  // Calculate PID gain
  gain = P*error + D*derivative;
  turnDir = (gain>0)?1:-1;
  gain = max(100, abs(gain));

  // Move motors using PID gain
  m1Speed = max(min(turnDir*gain * P, 200), -200);
  m2Speed = max(min(-turnDir*gain * P, 200), -200);
  motors.setSpeeds(m1Speed, m2Speed);
  delay(50);
  motors.setSpeeds(0, 0);
}

// Shoot water out of the cannon
void sprayCannon(){
    delay(2000);  
    digitalWrite(PumpPin, HIGH);
    swivelStepper(); 
    digitalWrite(PumpPin, LOW);
    delay(2000);
}

// Sweep the cannon back and forth as it shoots water
void swivelStepper(){
  for (int i = 0; i < 5; i++){
      myStepper.step(-40);
      delay(50);
    }
    for (int i = 0; i < 5; i++){
      myStepper.step(46);
      delay(50);
    }
    for (int i = 0; i < 5; i++){
      myStepper.step(-40);
      delay(50);
    }
}

// Check if fire threshold is high enough, and if it is, shoot the fire out
void takeOutFire(){
  if (analogRead(A4) > 950){
    Serial.println("Fire");
    motors.setSpeeds(0, 0);
    sprayCannon();
    return; 
  }
}

