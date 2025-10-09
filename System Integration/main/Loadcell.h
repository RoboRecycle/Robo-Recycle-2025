#ifndef LOADCELL_H
#define LOADCELL_H

#include <Arduino.h>
#include "HX711.h"

// HX711 instance
HX711 loadcell;

// === CONFIG ===
#define LOADCELL_DOUT_PIN 2   // Data pin
#define LOADCELL_SCK_PIN 3    // Clock pin
#define LOADCELL_CALIBRATION_FACTOR 1.0   // To be calibrated
#define LOADCELL_READING_NUMBER 1

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