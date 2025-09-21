#ifndef LOADCELL_H
#define LOADCELL_H

#include <Arduino.h>

/**
 * @brief Initialize the load cell hardware and amplifier (e.g., HX711)
 */
void Loadcell_Init();

/**
 * @brief Read current load cell value
 * @return Raw or calibrated value
 */
float Loadcell_Read();

/**
 * @brief Set current load cell reading as zero (tare)
 */
void Loadcell_Tare();

#endif