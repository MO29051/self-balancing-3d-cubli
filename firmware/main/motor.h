#pragma once
#include <Arduino.h>

// Initialize all three motor PWM channels
void motor_init();

// Set motor speed: value in range [-1.0, 1.0]
// Negative = reverse, 0 = idle, positive = forward
void motor_set(uint8_t motorIndex, float speed);

// Emergency stop — sets all motors to idle
void motor_stop_all();
