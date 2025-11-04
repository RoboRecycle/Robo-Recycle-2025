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

// Serial command buffer
String inputString = "";
bool stringComplete = false;

void setup() {
  Serial.begin(115200);
  DCmotor_Init();
  STEPPERmotor_Init();
  Encoder_Init();
  Loadcell_Init();

  Stepper_HomeAll();
  Serial.println(F("System Homed and Ready"));
  Serial.println(F("R2 Robot Starting..."));
  Serial.println(F("Type HELP for commands"));

  inputString.reserve(64);  // prevent memory fragmentation
}


void printHelp() {
  Serial.println(F("\n=== R2 ROBOT COMMANDS ==="));
  Serial.println(F("HOME                → Home all steppers"));
  Serial.println(F("GOTO X Y Z          → Move gantry"));
  Serial.println(F("DRILL deg [spd]     → Rotate drill (spd=0-255)"));
  Serial.println(F("UNSCREW X Y         → Auto unscrew at X,Y"));
  Serial.println(F("BRAKE               → Stop drill"));
  Serial.println(F("RPM                 → Show drill speed"));
  Serial.println(F("POS                 → Show encoder position"));
  Serial.println(F("LOAD                → Read load cell"));
  Serial.println(F("HELP                → This menu"));
  Serial.println(F("==========================\n"));
}

void loop() {
  // --- Serial command parser ---
  if (stringComplete) {
    processCommand(inputString);
    inputString = "";
    stringComplete = false;
  }

  // Optional: add background tasks here (e.g. auto-unscrew, monitoring)
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\n' || inChar == '\r') {
      if (inputString.length() > 0) {
        stringComplete = true;
      }
    } else if (inputString.length() < 60) {
      inputString += inChar;
    }
  }
}


/**
 * @brief Fully automated unscrew routine at given X,Y
 * @param x Target X coordinate
 * @param y Target Y coordinate
 * @return true if screw removed, false on failure/timeout
 */
bool autoUnscrew(float x, float y) {
  const float Z_START       = 140;   // safe high Z
  const float Z_STEP_ENGAGE = 0.5;     // mm per step
  const float Z_STEP_UNSCREW = 0.2;
  const float Z_MAX_DROP    = 50.0;    // max Z travel
  const float ENGAGE_DRILL_DEGREES = 180;   // 2 full turns per attempt
  const float UNSCREW_DRILL_DEGREES = 60;  
  const int   DRILL_SPEED  = 180;
  const float ENGAGE_LOAD  = -300000.000;     // > this = screw engaged
  const float Z_UNSCREW_THRESHOLD = -300000.000; // keep moving down until this threshold acheived
  const float DRILL_STOP_THRESHOLD = -300000.000;
  const unsigned long TIMEOUT = 100000; // 15 sec max

  unsigned long startTime = millis();
  float currentZ = Z_START;

  // --- 1. Move to X,Y at safe Z ---
  Serial.println(F("Moving to XY..."));
  Stepper_MoveTo(x, y, Z_START);
  delay(2000);

  float load = Loadcell_Read();
  Serial.print(F("Initial load: ")); Serial.println(load, 3);

  // --- 2. Lower Z until engagement ---
  Serial.println(F("Lowering Z to engage screw..."));
  while (load > ENGAGE_LOAD && (millis() - startTime) < TIMEOUT) { // may need to change this if load is positive
    currentZ += Z_STEP_ENGAGE;
    // if (currentZ < Z_START - Z_MAX_DROP) {
    //   Serial.println(F("Z limit reached."));
    //   return false;
    // }
    Stepper_MoveTo(x, y, currentZ);
    load = Loadcell_Read();
    Serial.print(F("Z=")); Serial.print(currentZ, 1);
    Serial.print(F(" Load=")); Serial.println(load, 3);
  }

  // if (load < ENGAGE_LOAD) {
  //   Serial.println(F("Failed to engage screw."));
  //   return false;
  // }

  Serial.println(F("Screw touching"));

  // --- 3. Check engagement
  bool drillOk = Motor_RotateDegrees(-ENGAGE_DRILL_DEGREES, DRILL_SPEED, 8000);
  if (!drillOk) {
    Serial.println(F("Drill timeout."));
    return false;
  } 

  drillOk = Motor_RotateDegrees(10, DRILL_SPEED, 8000);
  if (!drillOk) {
    Serial.println(F("Drill timeout."));
    return false;
  } 

  float initialDrillLoad = Loadcell_Read();

  if (initialDrillLoad < load) {
    Serial.println(F("Screw engaged"));
  } else {
    // potentially put while loop in here that does steps to engage if it misses, ie try again but rotate drill 20 degrees
    float secondaryDrillLoad = Loadcell_Read();
    bool drillOk = Motor_RotateDegrees(-ENGAGE_DRILL_DEGREES, DRILL_SPEED, 8000);
    if (!drillOk) {
      Serial.println(F("Drill timeout."));
      return false;
    }
    if (secondaryDrillLoad > initialDrillLoad) {
      Serial.println(F("Screw engaged"));
    }
  }
  

  // --- 4. Unscrew loop: drill + raise Z ---
  bool unscrewed = false;
  while (!unscrewed) {
    load = Loadcell_Read();
    while (load < Z_UNSCREW_THRESHOLD) {
      currentZ -= Z_STEP_UNSCREW;
      Stepper_MoveTo(x, y, currentZ);
      load = Loadcell_Read();
    }

    int count = 0;
    while (load > DRILL_STOP_THRESHOLD) {
      count ++;
      // Drill (rotate backward)
      bool drillOk = Motor_RotateDegrees(-UNSCREW_DRILL_DEGREES, DRILL_SPEED, 8000);
      if (!drillOk) {
        Serial.println(F("Drill timeout."));
        return false;
      }
      float prevLoad = load;
      load = Loadcell_Read();
      Serial.println(load);
      // if (load > prevLoad) {
      //   unscrewed = true;
      // }
      if (count > 10) {
        unscrewed = true;
        break;
      }
    }
  }

  Stepper_MoveTo(x, y, Z_START - 20);
  Motor_Brake();

  return true;
}

void processCommand(String cmd) {
  cmd.trim();
  cmd.toUpperCase();

  Serial.print(F("> "));
  Serial.println(cmd);

  /* --------------------------------------------------- */
  /*  HELP                                               */
  /* --------------------------------------------------- */
  if (cmd == "HELP") {
    printHelp();
    return;
  }

  /* --------------------------------------------------- */
  /*  HOME                                               */
  /* --------------------------------------------------- */
  if (cmd == "HOME") {
    Serial.println(F("Homing all steppers..."));
    Stepper_HomeAll();
    Serial.println(F("Homed."));
    return;
  }

  /* --------------------------------------------------- */
  /*  GOTO X Y Z                                         */
  /* --------------------------------------------------- */
  if (cmd.startsWith("GOTO ")) {
  String args = cmd.substring(5);
  args.trim();

  float x, y, z;
  int firstSpace = args.indexOf(' ');
  int secondSpace = args.indexOf(' ', firstSpace + 1);

  if (firstSpace > 0 && secondSpace > 0) {
    x = args.substring(0, firstSpace).toFloat();
    y = args.substring(firstSpace + 1, secondSpace).toFloat();
    z = args.substring(secondSpace + 1).toFloat();

    Serial.print(F("Moving to X=")); Serial.print(x, 2);
    Serial.print(F(" Y=")); Serial.print(y, 2);
    Serial.print(F(" Z=")); Serial.println(z, 2);

    Stepper_MoveTo(x, y, z);
    Serial.println(F("Move complete."));
  } else {
    Serial.println(F("Error: GOTO X Y Z  (use spaces, no commas)"));
  }
  return;
}

  /* --------------------------------------------------- */
  /*  DRILL degrees [speed]                              */
  /* --------------------------------------------------- */
  if (cmd.startsWith("DRILL ")) {
    String args = cmd.substring(6); // everything after "DRILL "
    args.trim();

    float deg = 0.0;
    int speed = 180; // default PWM speed

    int spaceIndex = args.indexOf(' ');
    if (spaceIndex >= 0) {
        // There is a second value for speed
        String degStr = args.substring(0, spaceIndex);
        String speedStr = args.substring(spaceIndex + 1);
        deg = degStr.toFloat();
        speed = speedStr.toInt();
    } else {
        // Only degrees given
        deg = args.toFloat();
    }

    // Clamp speed
    if (speed < 0)   speed = 0;
    if (speed > 255) speed = 255;

    Serial.print(F("Rotating "));
    Serial.print(deg, 2);
    Serial.print(F(" degrees at PWM "));
    Serial.println(speed);

    bool ok = Motor_RotateDegrees(deg, speed, 10000);
    Serial.println(ok ? F("Done.") : F("TIMEOUT!"));
    return;
}

  /* --------------------------------------------------- */
  /*  BRAKE / RPM / POS / LOAD                          */
  /* --------------------------------------------------- */
  if (cmd == "BRAKE") {
    Motor_Brake();
    Serial.println(F("Drill stopped."));
    return;
  }
  if (cmd == "RPM") {
    float rpm = Encoder_GetSpeedRPM();
    Serial.print(F("Drill RPM: "));
    Serial.println(rpm, 1);
    return;
  }
  if (cmd == "POS") {
    long pos = Encoder_GetPosition();
    float deg = pos * 360.0f / 1440.0f;
    Serial.print(F("Encoder: "));
    Serial.print(pos);
    Serial.print(F(" pulses ("));
    Serial.print(deg, 2);
    Serial.println(F(" degrees)"));
    return;
  }
  if (cmd == "LOAD") {
    float load = Loadcell_Read();
    Serial.print(F("Load cell: "));
    Serial.println(load, 3);
    return;
  }

  /* --------------------------------------------------- */
  /*  UNSCREW X Y  –  Auto unscrew routine               */
  /* --------------------------------------------------- */
  if (cmd.startsWith("UNSCREW ")) {
    String args = cmd.substring(8);
    args.trim();

    float x = 0.0;
    float y = 0.0;

    int spaceIndex = args.indexOf(' ');
    if (spaceIndex >= 0) {
        // Split into X and Y parts
        String xStr = args.substring(0, spaceIndex);
        String yStr = args.substring(spaceIndex + 1);
        xStr.trim();
        yStr.trim();
        x = xStr.toFloat();
        y = yStr.toFloat();

        Serial.print(F("Starting UNSCREW at X="));
        Serial.print(x, 2);
        Serial.print(F(" Y="));
        Serial.println(y, 2);

        bool success = autoUnscrew(x, y);
        Serial.println(success ? F("UNSCREW complete.") : F("UNSCREW failed."));
    } else {
        Serial.println(F("Error: UNSCREW X Y"));
    }

    return;
  }

  /* --------------------------------------------------- */
  /*  Unknown                                            */
  /* --------------------------------------------------- */
  Serial.println(F("Unknown command. Type HELP."));
}


