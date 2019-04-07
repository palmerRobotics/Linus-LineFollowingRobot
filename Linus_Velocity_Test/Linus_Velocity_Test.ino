/***************************************************************
 *                     LINUS VELOCITY TEST                     *
 * This code commands the line following robot to drive forward  
 * at the rated motor voltage of 6V.  The linebot will travel 
 * for 5 seconds and then stop.  The distance travelled will be
 * measured and then velocity in ft/sec will be calculated.  
 **************************************************************/

#include <DRV8835MotorShield.h>

DRV8835MotorShield motors;
int maxSpeedAt6V = 284;

void setup() {
  //pinMode(11, OUTPUT);
  Serial.begin(9600);
  Serial.print("Initializing\n");
  motors.flipM1(false); //M1 forward
  motors.flipM2(true); //M2 forward
  delay(4000);
  Serial.print("forward drive\n");
  motors.setSpeeds(maxSpeedAt6V, maxSpeedAt6V);
  delay(5000);
  motors.setSpeeds(0,0);
  Serial.print("End of Test\n");
}

void loop() {
  //do nothing
}
