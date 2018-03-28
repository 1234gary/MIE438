#include <DRV8835MotorShield.h>
#include <X113647Stepper.h>

DRV8835MotorShield ms;
const int LEFT = -1;
const int RIGHT = 1;

static const int STEPS_PER_REVOLUTION = 64 * 32;  
X113647Stepper myStepper(STEPS_PER_REVOLUTION, 2, 3, 4, 5);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  myStepper.setSpeed(6.5);
}

void loop() {
  // Scans and aligns robot towards fire before moving forward
  
  alignRobot();
//  ms.setSpeeds(100, 100);
//  delay(1000);
}


int scanForFire() {
  int sensorLeft = 0, sensorMiddle = 0, sensorRight = 0;
  int fireDirection;
  int stepPosition = 0;
  // Stop motor from moving
  ms.setSpeeds(0, 0);

  while (sensorMiddle <= max(sensorLeft, sensorRight)){
    sensorLeft = analogRead(A1);
    sensorMiddle = analogRead(A2);
    sensorRight = analogRead(A3);
    
    if (sensorLeft > sensorRight){
      fireDirection = LEFT;
    }else{
      fireDirection = RIGHT;
    }
    stepPosition += fireDirection*100;
    myStepper.step(fireDirection*100);
    Serial.print("Sensor Values: ");
    Serial.print(sensorLeft);
    Serial.print(sensorMiddle);
    Serial.println(sensorRight);
  }
  return stepPosition;
}

void alignRobot(){
  // Keeps on scanning for fire until vehicle is aligned
  int error = scanForFire();
  Serial.println("Error");
  Serial.println(error);
//  while (abs(error) > 20){
//    Serial.println("Error");
//    Serial.println(error);
//    if (error > 0){
//      ms.setSpeeds(0, 100);
//      delay(100);
//    }else{
//      ms.setSpeeds(100, 0);
//      delay(100);
//    }
//    error = scanForFire();
//  }
}

//void setM1Speed(int speed) 
//void setM2Speed(int speed) 
//void setSpeeds(int m1Speed, int m2Speed) 
//void flipM1(bool flip) 
//void flipM2(bool flip) 
//myStepper.step(STEPS_PER_REVOLUTION);
