#include "BasicStepperDriver.h"

// Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
#define MOTOR_STEPS 200
#define RPM 250

// Since microstepping is set externally, make sure this matches the selected mode
// If it doesn't, the motor will move at a different RPM than chosen
// 1=full step, 2=half step etc.
#define MICROSTEPS 1

#define DIR_LEFT A7
#define STEP_LEFT A6
#define ENABLE_LEFT A2
BasicStepperDriver leftStepper(MOTOR_STEPS, DIR_LEFT, STEP_LEFT, ENABLE_LEFT);

#define DIR_RIGHT 33
#define STEP_RIGHT 32
#define ENABLE_RIGHT 31
BasicStepperDriver rightStepper(MOTOR_STEPS, DIR_RIGHT, STEP_RIGHT, ENABLE_RIGHT);

#define DIR_UP 48
#define STEP_UP 46
#define ENABLE_UP A8
BasicStepperDriver upStepper(MOTOR_STEPS, DIR_UP, STEP_UP, ENABLE_UP);

#define DIR_DOWN 34
#define STEP_DOWN 36
#define ENABLE_DOWN 30
BasicStepperDriver downStepper(MOTOR_STEPS, DIR_DOWN, STEP_DOWN, ENABLE_DOWN);

#define DIR_FRONT A1
#define STEP_FRONT A0
#define ENABLE_FRONT 38
BasicStepperDriver frontStepper(MOTOR_STEPS, DIR_FRONT, STEP_FRONT, ENABLE_FRONT);

#define DIR_BACK 28
#define STEP_BACK 26
#define ENABLE_BACK 24
BasicStepperDriver backStepper(MOTOR_STEPS, DIR_BACK, STEP_BACK, ENABLE_BACK);

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(10);
  delay(500);

  leftStepper.begin(RPM, MICROSTEPS);
  rightStepper.begin(RPM, MICROSTEPS);
  upStepper.begin(RPM, MICROSTEPS);
  downStepper.begin(RPM, MICROSTEPS);
  frontStepper.begin(RPM, MICROSTEPS);
  backStepper.begin(RPM, MICROSTEPS);
  
  leftStepper.disable();
  rightStepper.disable();
  upStepper.disable();
  downStepper.disable();
  frontStepper.disable();
  backStepper.disable();

}

void loop() {
  if (Serial.available() > 0) {
    int wait = Serial.parseInt();
    int rpm = Serial.parseInt();
    leftStepper.setRPM(rpm);
    rightStepper.setRPM(rpm);
    upStepper.setRPM(rpm);
    downStepper.setRPM(rpm);
    frontStepper.setRPM(rpm);
    backStepper.setRPM(rpm);
    
    String command = Serial.readStringUntil('\n');
    leftStepper.enable();
    rightStepper.enable();
    upStepper.enable();
    downStepper.enable();
    frontStepper.enable();
    backStepper.enable();
    delay(100);

    for (int i = 0; i < command.length(); i++) {
      delay(wait);
      switch (command[i]) {
        case 'L':
          if (command[i + 1] == '2') {
            leftStepper.rotate(180);
            i++;
          } else if (command[i + 1] == '\'') {
            leftStepper.rotate(-90);
            i++;
          } else {
            leftStepper.rotate(90);
          }
          break;
        case 'R':
          if (command[i + 1] == '2') {
            rightStepper.rotate(180);
            i++;
          } else if (command[i + 1] == '\'') {
            rightStepper.rotate(-90);
            i++;
          } else {
            rightStepper.rotate(90);
          }
          break;
        case 'U':
          if (command[i + 1] == '2') {
            upStepper.rotate(180);
            i++;
          } else if (command[i + 1] == '\'') {
            upStepper.rotate(-90);
            i++;
          } else {
            upStepper.rotate(90);
          }
          break;
        case 'D':
          if (command[i + 1] == '2') {
            downStepper.rotate(180);
            i++;
          } else if (command[i + 1] == '\'') {
            downStepper.rotate(-90);
            i++;
          } else {
            downStepper.rotate(90);
          }
          break;
        case 'F':
          if (command[i + 1] == '2') {
            frontStepper.rotate(180);
            i++;
          } else if (command[i + 1] == '\'') {
            frontStepper.rotate(-90);
            i++;
          } else {
            frontStepper.rotate(90);
          }
          break;
        case 'B':
          if (command[i + 1] == '2') {
            backStepper.rotate(180);
            i++;
          } else if (command[i + 1] == '\'') {
            backStepper.rotate(-90);
            i++;
          } else {
            backStepper.rotate(90);
          }
          break;
      }
    }

    delay(500);
    
    leftStepper.disable();
    rightStepper.disable();
    upStepper.disable();
    downStepper.disable();
    frontStepper.disable();
    backStepper.disable();
  }
}
