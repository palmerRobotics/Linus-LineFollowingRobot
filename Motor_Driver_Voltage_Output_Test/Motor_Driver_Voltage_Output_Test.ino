/***************************************************************
 *              MOTOR DRIVER VOLTAGE OUTPUT TEST               *
 * This code increases the pwm of the motor motor from 0 to 100%
 * in 3 second increments.  This allows enough time to measure 
 * the voltage across the motor terminals with a multimeter.
 * The purpose of this test is to find what is the motor voltage
 * at full pwm (with a 9V power source) and approximately what 
 * pwm will give a motor voltage of 6 volts.  The output voltage
 * was measured useing a multimeter.  An enhancemnent would be 
 * to have the arduino read the motor driver output.  
 **************************************************************/

#include <DRV8835MotorShield.h>

DRV8835MotorShield motors;

void setup() {
  Serial.begin(9600);
  Serial.print("Initializing\n");
  motors.flipM1(false); //set to forward rotation
  motors.flipM2(true);
  //pinMode(A5, INPUT); //Using the arduino to measure the output voltage requires some extra work, as the arduino pins and read a maximum of 5V.  Obviously 9V exceeds that and could damage the arduino.
}

void loop() {
  /*uncomment the proceding code block to test minimum and 
    maximum motor driver output voltage*/
  /*for(int i = 0; i <= 400; i+=50){
    if(i > 400){
      i= 400;
    }
  motors.setSpeeds(i,i);
  Serial.print("Speed:");
  Serial.print(i);
  Serial.print("\n");
  delay(3000);
  }*/

  /*uncomment the proceding code block to what function value
    yields an output voltage of 6V{Through repeated uses, the
    range was narrowed to 260-270, in increments of 2*/
  for(int i = 260; i <= 270; i+=2){
    if(i > 270){
      i= 270;
    }
  motors.setSpeeds(i,i);
  Serial.print("Speed:");
  Serial.print(i);
  Serial.print("\n");
  delay(3000);
  }
}
