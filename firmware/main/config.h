#pragma once

// ─── Pin Definitions ──────────────────────────────────────────────
#define SDA_PIN         21
#define SCL_PIN         22

#define MOTOR1_PWM_PIN  25    // Roll axis
#define MOTOR2_PWM_PIN  26    // Pitch axis
#define MOTOR3_PWM_PIN  27    // Yaw axis

// ─── IMU Settings ─────────────────────────────────────────────────
#define IMU_CALIBRATION_SAMPLES  500
#define COMPLEMENTARY_ALPHA      0.98f   // Gyro trust weight (0–1)
#define CONTROL_LOOP_HZ          1000    // Hz

// ─── PID Gains ────────────────────────────────────────────────────
// Tune these per-axis. Start with Kp, then add Kd.
#define ROLL_KP     1.5f
#define ROLL_KI     0.01f
#define ROLL_KD     0.05f

#define PITCH_KP    1.5f
#define PITCH_KI    0.01f
#define PITCH_KD    0.05f

#define YAW_KP      1.0f
#define YAW_KI      0.005f
#define YAW_KD      0.03f

// ─── Setpoints (target angles in degrees) ─────────────────────────
#define ROLL_SETPOINT   0.0f
#define PITCH_SETPOINT  0.0f
#define YAW_SETPOINT    0.0f

// ─── Motor Limits ─────────────────────────────────────────────────
#define MOTOR_PWM_MIN   1000   // µs (stopped)
#define MOTOR_PWM_MAX   2000   // µs (full speed)
#define MOTOR_PWM_IDLE  1500   // µs (neutral)

// ─── Safety ───────────────────────────────────────────────────────
#define MAX_TILT_ANGLE  45.0f  // Degrees — cut motors if exceeded
