#include <Thread.h>
#include <ThreadController.h>
#include <Wire.h>
#include <HMC5883L.h>
#include "MPU6050.h"

MPU6050 accelgyro;
HMC5883L compass;
int m1 = 4;//changed to same pins on arduino ROMEO
int m2 = 2;
int e1 = 5;
int e2 = 6;
int m3 = 7;
int m4 = 3;
int led = 13;
int compHeading;
int targetHeading;
float declinationAngle = (21.0 + (18.0 / 60.0)) / (180 / M_PI);// You can find your declination on: http://magnetic-declination.com/
ThreadController main = ThreadController();
Thread sensors = Thread();
Thread output = Thread();
int failsafe = 0; //variable to be incremented each 100ms, if it reaches 100, it stops the robot

void setup() {
  Wire.begin();
  accelgyro.setI2CMasterModeEnabled(false);
  accelgyro.setI2CBypassEnabled(true) ;
  accelgyro.setSleepEnabled(false);
  Serial.begin(9600);
  pinMode( m1 , OUTPUT);
  pinMode( m2 , OUTPUT);
  pinMode( m3 , OUTPUT);
  pinMode( m4 , OUTPUT);
  pinMode( e1 , OUTPUT);
  pinMode( e2 , OUTPUT);
  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);
  while (!compass.begin())
  {
    Serial.println("Could not find a valid HMC5883L sensor, check wiring!");
    delay(500);
  }
  compass.setRange(HMC5883L_RANGE_1_3GA);
  compass.setMeasurementMode(HMC5883L_CONTINOUS);
  compass.setDataRate(HMC5883L_DATARATE_30HZ);
  compass.setSamples(HMC5883L_SAMPLES_8);
  compass.setOffset(0, 0);
  sensors.onRun(sensorRefresh);
  sensors.setInterval(50);
  main.add(&sensors);
  output.onRun(calculate);
  output.setInterval(100);
  main.add(&output);


  digitalWrite(led, LOW);
}

void loop() {
  main.run();
  while (Serial.available() > 0) {
    int command = Serial.parseInt();
    switch (command) {
      case 0://stop all
        roll(0, 0);
        break;
      case 1:
        //something
        break;
      default:
        roll(0, 0);
        output.enabled = false;
        sensors.enabled = false;
    }

  }
}

void sensorRefresh() {
  Vector norm = compass.readNormalize();
  float heading = atan2(norm.YAxis, norm.XAxis);
  heading -= declinationAngle;
  heading = heading * 180 / M_PI;
  if (heading < 0)
  {
    heading += 360;
  }

  if (heading > 360)
  {
    heading -= 360;
  }
  compHeading = int(heading);
}

void calculate() {
  failsafe += 1;
  roll(0, 0);
}

void roll(int rm, int lm) {
  if (rm >= 0) {
    analogWrite(e1, rm);
    digitalWrite(m1, 1);
    digitalWrite(m2, 0);
  }
  else {
    rm = -rm;
    analogWrite(e1, rm);
    digitalWrite(m1, 0);
    digitalWrite(m2, 1);
  }
  if (lm >= 0) {
    analogWrite(e2, lm);
    digitalWrite(m3, 1);
    digitalWrite(m4, 0);
  }
  else {
    lm = -lm;
    analogWrite(e2, lm);
    digitalWrite(m3, 0);
    digitalWrite(m4, 1);
  }
}

