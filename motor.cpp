#include "imu.h"
#include "config.h"
#include <Wire.h>
#include <MPU6050.h>

static MPU6050 mpu;
static Angles  _angles = {0, 0, 0};

// Gyro offset calibration
static float gx_offset = 0, gy_offset = 0, gz_offset = 0;

void imu_init() {
    Wire.begin(SDA_PIN, SCL_PIN);
    mpu.initialize();

    Serial.println("[IMU] Calibrating — keep cube still...");
    long gxSum = 0, gySum = 0, gzSum = 0;
    for (int i = 0; i < IMU_CALIBRATION_SAMPLES; i++) {
        int16_t ax, ay, az, gx, gy, gz;
        mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
        gxSum += gx;  gySum += gy;  gzSum += gz;
        delay(2);
    }
    gx_offset = gxSum / (float)IMU_CALIBRATION_SAMPLES / 131.0f;
    gy_offset = gySum / (float)IMU_CALIBRATION_SAMPLES / 131.0f;
    gz_offset = gzSum / (float)IMU_CALIBRATION_SAMPLES / 131.0f;
    Serial.printf("[IMU] Offsets: gx=%.3f  gy=%.3f  gz=%.3f\n",
                  gx_offset, gy_offset, gz_offset);
}

RawIMU imu_read_raw() {
    int16_t ax16, ay16, az16, gx16, gy16, gz16;
    mpu.getMotion6(&ax16, &ay16, &az16, &gx16, &gy16, &gz16);

    RawIMU r;
    r.ax = ax16 / 16384.0f;        // ±2g scale
    r.ay = ay16 / 16384.0f;
    r.az = az16 / 16384.0f;
    r.gx = gx16 / 131.0f - gx_offset;   // ±250°/s scale
    r.gy = gy16 / 131.0f - gy_offset;
    r.gz = gz16 / 131.0f - gz_offset;
    return r;
}

Angles imu_update(float dt) {
    RawIMU r = imu_read_raw();

    // Accelerometer angle estimates
    float roll_acc  = atan2f(r.ay, r.az) * RAD_TO_DEG;
    float pitch_acc = atan2f(-r.ax, sqrtf(r.ay*r.ay + r.az*r.az)) * RAD_TO_DEG;

    // Complementary filter fusion
    _angles.roll  = COMPLEMENTARY_ALPHA * (_angles.roll  + r.gx * dt)
                  + (1.0f - COMPLEMENTARY_ALPHA) * roll_acc;
    _angles.pitch = COMPLEMENTARY_ALPHA * (_angles.pitch + r.gy * dt)
                  + (1.0f - COMPLEMENTARY_ALPHA) * pitch_acc;
    _angles.yaw  += r.gz * dt;   // Yaw relies on gyro only (no mag)

    return _angles;
}
