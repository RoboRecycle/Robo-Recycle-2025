#include "DCmotor.h"
#include "STEPPERmotor.h"
#include "Encoder.h"
#include "Loadcell.h"

/**
 * @brief Arduino setup function. Initializes all subsystems.
 */
void setup() {
  DCmotor_Init();
  STEPPERmotor_Init();
  Encoder_Init();
  Loadcell_Init();
}

/**
 * @brief Arduino main loop. Application logic goes here.
 */
void loop() {
  // TODO: implement main logic
}
