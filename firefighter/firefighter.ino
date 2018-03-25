#include <DRV8835MotorShield.h>
#include <X113647Stepper.h>

DRV8835MotorShield ms;

static const int STEPS_PER_REVOLUTION = 64 * 32;  
X113647Stepper myStepper(STEPS_PER_REVOLUTION, 8, 9, 10, 11);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // read the input on analog pin 0:
  int sensorValue = analogRead(A1);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage = sensorValue * (5.0 / 1023.0);
  // print out the value you read:
  Serial.println(voltage);
  delay(100);
  ms.setM1Speed(100);
}



//void setM1Speed(int speed) 
//void setM2Speed(int speed) 
//void setSpeeds(int m1Speed, int m2Speed) 
//void flipM1(bool flip) 
//void flipM2(bool flip) 

