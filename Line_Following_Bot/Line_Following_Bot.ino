/****************************************************************
 * AUTHOR: Palmer Clegg
 * DATE: 12/22/2018
 * DESCRIPTION: 
 * UPDATED: 04/08/2019
 ***************************************************************/
#include <QTRSensors.h>
#include <DRV8835MotorShield.h>

DRV8835MotorShield motors;
QTRSensors sensors;
const uint8_t sensorCount = 8;
const uint8_t sensorPins[sensorCount] = {A2, A3, A4, A5, 2, 3, 4, 5};
uint16_t sensorValues[sensorCount];

double time;

void setup() {
  //The following code configures the QTRRC infrared sensors
  sensors.setTypeRC();
  sensors.setSensorPins(sensorPins, sensorCount);
  sensors.setEmitterPin(A1);
  sensors.setTimeout(QTRRCDefaultTimeout);//QTRRCDefaultTimeout is defined as 2500us in QTRSensors.h
  
  pinMode(A0, OUTPUT);//Powers the Vcc pin of the QTRRC sensors
  digitalWrite(A0, HIGH);
  
  delay(4000);//Gives the user some time to power the linebot and place it on the line
 
  motors.flipM1(false); //Ensures that the two motors both rotate in the same direction
  motors.flipM2(true);
  
  Serial.begin(9400);
  Serial.println("BEGINNING CALIBRATION");
  
  /* The following is the calibration routine.  The routine takes
   * 5 seconds.  4 seconds of sweeping, 1 sec of delay after
   * sweeping. */ 
  for(int i = 0; i < 200; i++){
    sensors.calibrate();
    if(i == 0 || i == 40*3){//turn right after 0 seconds. Turn right again after 4 seconds
      Serial.println("  TURNING RIGHT");
      motors.setM1Speed(-60);//turn right
      motors.setM2Speed(60);
    }
    else if(i == 40*1){// turn left after 1 second
      Serial.println("  TURNING LEFT");
      motors.setM1Speed(60);//turn left
      motors.setM2Speed(-60);
    }
    else if(i == 40*4){//stop at 5 seconds.  Should be recentered on the line
      Serial.println("  RECENTERED");
      motors.setM1Speed(0);//stop turning
      motors.setM2Speed(0);
    }
  }  
  //printCalibration(sensors);

}

void printValues(unsigned int position){
  for(int i = 0; i < sensorCount; i++){
    Serial.print(sensorValues[i]);
    Serial.print('\t');
  }
  Serial.println(position);
}

//This is a debug function.  It prints the ...
void printCalibration(QTRSensors sensors){
  for(int i = 0; i < sensorCount; i++){
    Serial.print(sensors.calibrationOn.minimum[i]);
    Serial.print("\t");
  }
  Serial.println();
  for(int i = 0; i < sensorCount; i++){
    Serial.print(sensors.calibrationOn.maximum[i]);
    Serial.print("\t");
  }
  delay(2000);
}

  int baseSpeed = 290;
  int M1Speed;
  int M2Speed;
  int maxSpeed = 400;
  int minSpeed = 0;
  int error;
  int lastError = 0;
  double lastTime = 0;
  int totalError = 0;
  unsigned int position;
  unsigned int lastPosition = 3500;
  int controlValue;

  /* These PID values were found by some intuition and trial and
   * error.  The maximum error possible is 3500, and the maximum
   * input valued to set M1/M2Speed() is 400.  The error is
   * multiplied by kp and the product is added/subtracted from 
   * the baseSpeed value.  Therefore, the error will need to be 
   * significantly downscaled. .........*/
  double kp = .055;
  double kd = 28;
  double ki = 0;
  
void loop() {
  position = sensors.readLineBlack(sensorValues);
  Serial.print(position);
  Serial.print(' ');
  error = 3500 - position;
  Serial.print(error);
  Serial.print(' ');
  totalError = totalError + error;
  time = millis();
  controlValue = kp*error + ki*totalError + kd*((position-lastPosition)/((time-lastTime)*1000));
  Serial.print(kp*error);
  Serial.print(' ');
  Serial.print(kd*((position-lastPosition)/((time-lastTime)*1000)));
  Serial.print(' ');
  Serial.print(controlValue);
  Serial.print(' ');
  lastPosition = position;
  lastTime = time;

  M1Speed = baseSpeed - controlValue;
  M2Speed = baseSpeed + controlValue;

  /* The following if/else statements ensure that M1&M2Speed do 
   * not exceed the acceptable range of input values passed into 
   * setM1/M2Speed() */ 
  if(M1Speed > maxSpeed){
    M1Speed = maxSpeed;
  }
  else if(M1Speed < minSpeed){
    M1Speed = minSpeed;
  }
  if(M2Speed > maxSpeed){
    M2Speed = maxSpeed;
  }
  else if(M2Speed < minSpeed){
    M2Speed = minSpeed;
  }
  Serial.print(M1Speed);
  Serial.print(' ');
  Serial.print(M2Speed);
  Serial.println();
  motors.setM1Speed(M1Speed);
  motors.setM2Speed(M2Speed);
  
}
