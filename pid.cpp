#pragma once
#include <Arduino.h>

struct Angles {
    float roll;
    float pitch;
    float yaw;
};

struct RawIMU {
    float ax, ay, az;   // Accelerometer (g)
    float gx, gy, gz;   // Gyroscope (deg/s)
};

// Initialize I²C and MPU-6050; blocks during calibration
void imu_init();

// Read raw sensor values
RawIMU imu_read_raw();

// Update the complementary filter and return estimated angles
// Call this every control loop tick; dt in seconds
Angles imu_update(float dt);
