#ifndef STEPPERMOTOR_H
#define STEPPERMOTOR_H

#include <Arduino.h>

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

/**
 * @brief Initialize stepper motor drivers.
 */
void STEPPERmotor_Init();

/**
 * @brief Home X axis using endstop.
 */
void Stepper_HomeX();

/**
 * @brief Home Y axis using endstop.
 */
void Stepper_HomeY();

/**
 * @brief Home Z axis using endstop.
 */
void Stepper_HomeZ();

/**
 * @brief Home all axes.
 */
void Stepper_HomeAll();

/**
 * @brief Move steppers to specified coordinates in mm.
 * @param x_mm Target X position (mm).
 * @param y_mm Target Y position (mm).
 * @param z_mm Target Z position (mm).
 */
void Stepper_MoveTo(float x_mm, float y_mm, float z_mm);

#endif