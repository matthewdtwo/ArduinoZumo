#include <Arduino.h>
#include <Wire.h>
#include <ZumoShield.h>


#define TONE_DURATION 250
#define MAX_FORWARD_SPEED 400
#define MAX_TURN_SPEED 1000
#define MAX_REVERSE_SPEED -400
#define TURN_DURATION 600
#define REVERSE_DURATION 100
#define QTR_THRESHOLD 300   
#define NUM_SENSORS 6


ZumoBuzzer buzzer;
Pushbutton button(ZUMO_BUTTON);
// ZumoIMU imu;
ZumoMotors motors;
ZumoReflectanceSensorArray sensors(QTR_NO_EMITTER_PIN);


bool attack = false;

void setup() {
  buzzer.playFrequency(1000, 250, 15);
}

void startTone() {
 if(button.isPressed()) {
  button.waitForRelease();
  for(int i=0;i<5;i++) {
    buzzer.playFrequency(1000, TONE_DURATION, 10);
    delay(1000);
  }


  buzzer.playFrequency(3000, TONE_DURATION/3, 10);
  delay(150);
  buzzer.playFrequency(3000, TONE_DURATION/3, 10);
 
  attack = true;
 } 

}

bool checkForEdge() {
  unsigned int sensor_values[6];
  int avgSensors = 0;
  sensors.read(sensor_values);
  // sum sensor values ??
  for(int i=0;i<NUM_SENSORS; i++) {
    avgSensors = sensor_values[i];
  }
  avgSensors = avgSensors / NUM_SENSORS;
  if(avgSensors <= QTR_THRESHOLD) {
    return true;
  } else {
    return false;
  }

}

void loop() {
  startTone();

  if(attack) {
    if(checkForEdge()) {
      // oh snap, go back!
      motors.setSpeeds(MAX_REVERSE_SPEED, MAX_REVERSE_SPEED);
      delay(REVERSE_DURATION);
      motors.setSpeeds(-MAX_TURN_SPEED, MAX_TURN_SPEED);
      delay(TURN_DURATION);

    } else {
      motors.setSpeeds(MAX_FORWARD_SPEED, MAX_FORWARD_SPEED);
    }
  }

}