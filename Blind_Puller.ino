/*-----( Import needed libraries )-----*/
#include <AccelStepper.h>

/*-----( Declare Constants and Pin Numbers )-----*/
#define FULLSTEP 4
#define HALFSTEP 8
// motor pins
#define motorPin1  4     // Blue   - 28BYJ48 pin 1
#define motorPin2  5     // Pink   - 28BYJ48 pin 2
#define motorPin3  6     // Yellow - 28BYJ48 pin 3
#define motorPin4  7     // Orange - 28BYJ48 pin 4
// Red    - 28BYJ48 pin 5 (VCC)

const int ledPin = 13;
const int prNumReadings = 15; /* number of readings to smooth analog input across*/
const int tempNumReadings = 15; /* number of readings to smooth analog input across*/
const int prPin = A0;
const int tempPin = A1;
const float tempTarget = 20.0;

/*-----( Declare objects )-----*/
// NOTE: The sequence 1-3-2-4 is required for proper sequencing of 28BYJ48
AccelStepper stepper1(HALFSTEP, motorPin1, motorPin3, motorPin2, motorPin4);

/*-----( Declare Variables )-----*/

int prSensorValue;
int prSensorLow = 1023;
int prSensorHigh = 0;
int prReadings [prNumReadings];
int prReadIndex = 0;
int prTotal = 0;
int prAverage = 0;

int tempSensorValue;
float tempVoltage = 0.0;
float temperature = 0.0;
float tempReadings [prNumReadings];
int tempReadIndex = 0;
float tempTotal = 0;
float tempAverage = 0;


void setup() { /****** SETUP: RUNS ONCE ******/

  //  pinMode(ledPin, OUTPUT);
  //  digitalWrite(ledPin, HIGH);             /*turns on on-board led when photoresistor is calibrating*/
  //
  //  while(millis()<5000){                   /*calibrates photoresistor by finding the max and min value over 5 seconds*/
  //    prSensorValue = analogRead(prPin);
  //    if(prSensorValue > prSensorHigh){
  //      prSensorHigh = prSensorValue;
  //    }
  //    if(prSensorValue < prSensorLow){
  //      prSensorLow = prSensorValue;
  //    }
  //  }
  //   digitalWrite(ledPin, LOW);           /*turns off on-board led when photoresistor is calibrated*/
  Serial.begin(9600);                  /*open a serial port*/
}

void loop() { /****** LOOP: RUNS CONSTANTLY ******/

  prSensorValue = analogRead(prPin);
  if (prSensorValue > prSensorHigh) {
    prSensorHigh = prSensorValue;
  }
  if (prSensorValue < prSensorLow) {
    prSensorLow = prSensorValue;
  }
  prSensorValue = analogRead(prPin);
  prSensorValue = 100 * (prSensorValue - prSensorLow) / (prSensorHigh - prSensorLow); /*normalize values*/

  prTotal = prTotal - prReadings[prReadIndex];
  prReadings[prReadIndex] = prSensorValue;
  prTotal = prTotal + prReadings[prReadIndex];
  prReadIndex = prReadIndex + 1;

  if (prReadIndex >= prNumReadings) {
    prReadIndex = 0;
  }
  prAverage = prTotal / prNumReadings;

  tempSensorValue = analogRead(tempPin);
  tempVoltage = (tempSensorValue / 1024.0) * 5.0;
  temperature = (tempVoltage - 0.5) * 100;

  tempTotal = tempTotal - tempReadings[tempReadIndex];
  tempReadings[tempReadIndex] = temperature;
  tempTotal = tempTotal + tempReadings[tempReadIndex];
  tempReadIndex = tempReadIndex + 1;

  if (tempReadIndex >= tempNumReadings) {
    tempReadIndex = 0;
  }
  tempAverage = tempTotal / tempNumReadings;

  Serial.print(prAverage);
  Serial.print("\t");
  Serial.println(tempAverage);


  //delay(1);

}
