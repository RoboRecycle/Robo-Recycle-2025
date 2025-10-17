#include "STEPPERmotor.h"

// === CONFIG ===
#define MOTOR_INTERFACE_TYPE AccelStepper::DRIVER
#define STEPS_PER_MM_XY 9.09  // 200 steps / 22 mm
#define STEPS_PER_MM_Z 20     // 500 steps / 25 mm

// === X AXIS ===
#define X_STEP_PIN 2
#define X_DIR_PIN 5
#define X_ENDSTOP_PIN 10
AccelStepper stepperX(MOTOR_INTERFACE_TYPE, X_STEP_PIN, X_DIR_PIN);

// === Y AXIS ===
#define Y_STEP_PIN 3
#define Y_DIR_PIN 6
#define Y_ENDSTOP_PIN 9
AccelStepper stepperY(MOTOR_INTERFACE_TYPE, Y_STEP_PIN, Y_DIR_PIN);

// === Z AXIS ===
#define Z_STEP_PIN 4
#define Z_DIR_PIN 7
#define Z_ENDSTOP_PIN 11
AccelStepper stepperZ(MOTOR_INTERFACE_TYPE, Z_STEP_PIN, Z_DIR_PIN);

void STEPPERmotor_Init() {
  pinMode(X_ENDSTOP_PIN, INPUT_PULLUP);
  pinMode(Y_ENDSTOP_PIN, INPUT_PULLUP);
  pinMode(Z_ENDSTOP_PIN, INPUT_PULLUP);

  stepperX.setMaxSpeed(100000);
  stepperX.setAcceleration(5000);

  stepperY.setMaxSpeed(100000);
  stepperY.setAcceleration(5000);

  stepperZ.setMaxSpeed(30000);
  stepperZ.setAcceleration(2000);
}

// === HOMING FUNCTION TEMPLATE ===
void homeAxis(AccelStepper &stepper, int endstopPin, const char* axisName) {
  const int homingSpeed = 200;

  stepper.setMaxSpeed(homingSpeed);
  stepper.setAcceleration(50);
  if (strcmp(axisName, "Z") == 0) {
    stepper.moveTo(-100000);  // Z direction is opposite
  } else {
    stepper.moveTo(100000);  // Move far in negative direction
  }
  
  Serial.print("Homing ");
  Serial.println(axisName);

  while (digitalRead(endstopPin) != LOW) {
    stepper.run();
  }

  stepper.stop();

  stepper.setCurrentPosition(0);
  Serial.print("Homed ");
  Serial.println(axisName);
}

void Stepper_HomeX() {
  homeAxis(stepperX, X_ENDSTOP_PIN, "X");
}

void Stepper_HomeY() {
  homeAxis(stepperY, Y_ENDSTOP_PIN, "Y");
}

void Stepper_HomeZ() {
  homeAxis(stepperZ, Z_ENDSTOP_PIN, "Z");
}

void Stepper_HomeAll() {
  homeAxis(stepperX, X_ENDSTOP_PIN, "X");
  homeAxis(stepperY, Y_ENDSTOP_PIN, "Y");
  homeAxis(stepperZ, Z_ENDSTOP_PIN, "Z");
}

// Currently should do each individually, could be made to all move at once later

void Stepper_MoveTo(float x_mm, float y_mm, float z_mm) {
  long x_steps = x_mm * STEPS_PER_MM_XY;
  long y_steps = y_mm * STEPS_PER_MM_XY;
  long z_steps = z_mm * STEPS_PER_MM_Z;
  stepperX.moveTo(-x_steps);
  stepperY.moveTo(-y_steps);
  stepperZ.moveTo(z_steps);
  stepperX.runToPosition();
  stepperY.runToPosition();
  stepperZ.runToPosition();
}

