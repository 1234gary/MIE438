#include <Servo.h>
#include <DRV8835MotorShield.h>
#include <X113647Stepper.h>

const int LEFT = -1;
const int RIGHT = 1;

static const int STEPS_PER_REVOLUTION = 64 * 32;  
X113647Stepper myStepper(STEPS_PER_REVOLUTION, 2, 3, 4, 5);
DRV8835MotorShield motors;
Servo myservo;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  myStepper.setSpeed(6.5);
  myservo.attach(9);
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
//}


int position = 90;
void loop() {
  scanForFire(position);
}


void scanForFire(int stepPosition) {
  int sensorLeft, sensorMiddle, sensorRight;
  int fireDirection;
  
  while(true){
    sensorLeft = analogRead(A1);
    sensorMiddle = analogRead(A2);
    sensorRight = analogRead(A3);

    if ((sensorMiddle > sensorLeft*1.4)&&(sensorMiddle > sensorRight*1.4)){
      continue;
    }
    if (sensorLeft > sensorRight){
      fireDirection = LEFT;
    }else{
      fireDirection = RIGHT;
    }
    
    stepPosition = min(max(15*fireDirection+stepPosition, 0),180);
    myservo.write(stepPosition);
    delay(500);
    
    Serial.print("Sensor Values: ");
    Serial.println(sensorLeft);
    Serial.println(sensorMiddle);
    Serial.println(sensorRight);
    Serial.println("pos");
    Serial.println(position);
    }
}

