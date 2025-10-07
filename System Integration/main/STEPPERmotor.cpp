#include "STEPPERmotor.h"

void STEPPERmotor_Init() {
  pinMode(X_ENDSTOP_PIN, INPUT_PULLUP);
  pinMode(Y_ENDSTOP_PIN, INPUT_PULLUP);
  pinMode(Z_ENDSTOP_PIN, INPUT_PULLUP);

  stepperX.setMaxSpeed(1000);
  stepperX.setAcceleration(200);

  stepperY.setMaxSpeed(1000);
  stepperY.setAcceleration(200);

  stepperZ.setMaxSpeed(1000);
  stepperZ.setAcceleration(200);
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