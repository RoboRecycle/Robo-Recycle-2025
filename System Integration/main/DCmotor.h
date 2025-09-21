#ifndef DCMOTOR_H
#define DCMOTOR_H

#include <Arduino.h>

/**
 * @brief Initialize the DC motor driver pins.
 */
void DCmotor_Init();

/**
 * @brief Drive motor forward at a given speed.
 * @param speed PWM value (0–255).
 */
void Motor_Forward(int speed);

/**
 * @brief Drive motor backward at a given speed.
 * @param speed PWM value (0–255).
 */
void Motor_Backward(int speed);

/**
 * @brief Stop motor (brake).
 */
void Motor_Brake();

#endif