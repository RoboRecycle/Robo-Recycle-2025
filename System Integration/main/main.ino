#include <Arduino.h>
#include "DCmotor.h"
#include "STEPPERmotor.h"
#include "Encoder.h"
#include "Loadcell.h"

#define LOADCELL_THRESHOLD 5.0        
#define LOADCELL_ENGAGEMENT_DEADBAND 10.0  

#define SCREW_DISPOSAL_X 0
#define SCREW_DISPOSAL_Y 0
#define SCREW_DISPOSAL_Z 0

#define STEPPER_INCREMENT 0.1

float load_cell_reading = 0.0;

float motor_speed = 0.0;



float targetX = 0.0;
float targetY = 0.0;
float targetZ = 0.0;



/**
 * @brief Arduino setup function. Initializes all subsystems.
 */
void setup() {
  Serial.println("System Homed and Ready");
  Serial.begin(115200);
  Serial.println("R2 Robot Starting...");

  //DCmotor_Init();
  STEPPERmotor_Init();
  // Encoder_Init();
  // Loadcell_Init();
}

/**
 * @brief Arduino main loop. Application logic goes here.
 */

void loop() {

  Stepper_HomeAll();
  delay(2000);
  //Loadcell_Tare();
  Serial.println("System Homed and Ready");
  // -------------------------
  // Wait for target coordinates: 
  // in form: X Y
  // eg: 100.0 50.5
  // -------------------------
  
  Serial.println("Moving to screw");
  
  // float rpm = Loadcell_Read();
  // while (true) {
  //   rpm = Loadcell_Read();
    
  //   Serial.println(rpm);
  // }

  Stepper_MoveTo(220, 190.5, 200);
  delay(2000);
  // Motor_Forward(20);
  // float rpm = Encoder_GetSpeedRPM();
  // while (true) {
  //    rpm = Encoder_GetSpeedRPM();
  //    Encoder_Update();
  //    Serial.println(rpm);
    
  // }
  
  //Stepper_MoveTo(220, 190.5, 209);
  //Stepper_MoveTo(220, 190.5, 150);
  /**
  // -------------------------
  // Lower Z until load cell threshold is met
  // -------------------------
  unsigned long startTime = millis();
  const unsigned long TIMEOUT_MS = 3000;

  load_cell_reading = Loadcell_Read();
  while (load_cell_reading < LOADCELL_THRESHOLD) {
    // check timeout
    if (millis() - startTime > TIMEOUT_MS) {
      Serial.println("Timeout: screw not engaged");
      Motor_Brake();
      break;            
    }

    Motor_Forward(motor_speed);
    Stepper_MoveTo(targetX, targetY, targetZ - STEPPER_INCREMENT);
    Encoder_Update();

    float rpm = Encoder_GetSpeedRPM();
    int dir = Encoder_GetDirection();
    

    load_cell_reading = Loadcell_Read();
  }

  // -------------------------
  // Raise Z while engagement exceeded (unscrewing phase)
  // -------------------------
  while (load_cell_reading > LOADCELL_THRESHOLD) {
    Motor_Backward(50);  // unscrewing
    Stepper_MoveTo(targetX, targetY, targetZ + STEPPER_INCREMENT);
    load_cell_reading = Loadcell_Read();
  }

  // -------------------------
  // Stop DC motor
  // -------------------------
  Motor_Brake();

  // -------------------------
  // Move gantry to disposal location
  // -------------------------
  Serial.println("Moving screw to disposal area...");
  Stepper_MoveTo(SCREW_DISPOSAL_X, SCREW_DISPOSAL_Y, SCREW_DISPOSAL_Z);
  delay(5000); // Wait for 1 second (1000 milliseconds)
  **/
}
