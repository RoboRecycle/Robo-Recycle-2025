#include "Loadcell.h"

void Loadcell_Init() {
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  
  // // Wait for the chip to be ready
  // while (!scale.is_ready()) {
  //   Serial.println("Waiting for load cell to be ready...");
  //   delay(100);
  // }
  
  // Set the calibration factor
  scale.set_scale(LOADCELL_CALIBRATION_FACTOR);
  
  // Tare the scale to zero
  scale.tare();
  
  Serial.println("Load cell initialized and tared");
}

float Loadcell_Read() {
  return scale.get_units(LOADCELL_READING_NUMBER);  // Get average of LOADCELL_READING_NUMBER of readings, returns calibrated value
}

void Loadcell_Tare() {
  scale.tare();
}
