#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>

/**
 * @brief Initialize encoder pins and variables.
 */
void Encoder_Init();

/**
 * @brief Update encoder readings (position, speed, direction).
 */
void Encoder_Update();

/**
 * @brief Get current encoder position (counts).
 * @return Position in encoder counts.
 */
long Encoder_GetPosition();

/**
 * @brief Get current encoder speed in RPM.
 * @return Speed in RPM.
 */
float Encoder_GetSpeedRPM();

/**
 * @brief Get current encoder direction.
 * @return +1 forward, -1 backward, 0 stopped.
 */
int Encoder_GetDirection();

#endif