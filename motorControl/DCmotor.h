#ifndef DCMOTOR_H
#define DCMOTOR_H

// Pin definitions
const int IN1 = 5;
const int IN2 = 4;
const int ENA = 6;

// Motor function declarations
void Motor1_Forward(int Speed);
void Motor1_Backward(int Speed);
void Motor1_Brake();

#endif