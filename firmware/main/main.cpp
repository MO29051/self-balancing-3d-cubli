#include <Arduino.h>
#include "config.h"
#include "imu.h"
#include "pid.h"
#include "motor.h"

// ── PID Controllers (one per axis) ────────────────────────────────
PID pidRoll (ROLL_KP,  ROLL_KI,  ROLL_KD,  -1.0f, 1.0f);
PID pidPitch(PITCH_KP, PITCH_KI, PITCH_KD, -1.0f, 1.0f);
PID pidYaw  (YAW_KP,   YAW_KI,   YAW_KD,   -1.0f, 1.0f);

// ── Timing ────────────────────────────────────────────────────────
static unsigned long lastMicros = 0;
static const unsigned long DT_US = 1000000UL / CONTROL_LOOP_HZ;

void setup() {
    Serial.begin(115200);
    Serial.println("=== Cubli ESP32 Booting ===");

    imu_init();
    motor_init();

    lastMicros = micros();
    Serial.println("=== Control loop started ===");
}

void loop() {
    unsigned long now = micros();
    if (now - lastMicros < DT_US) return;   // Pace the loop precisely

    float dt = (now - lastMicros) / 1e6f;
    lastMicros = now;

    // 1. Estimate angles
    Angles angles = imu_update(dt);

    // 2. Safety cut — cube has fallen over
    if (fabsf(angles.roll)  > MAX_TILT_ANGLE ||
        fabsf(angles.pitch) > MAX_TILT_ANGLE) {
        motor_stop_all();
        pidRoll.reset();  pidPitch.reset();  pidYaw.reset();
        Serial.println("[SAFETY] Tilt limit exceeded. Motors stopped.");
        return;
    }

    // 3. Compute PID outputs
    float rollOut  = pidRoll .compute(ROLL_SETPOINT  - angles.roll,  dt);
    float pitchOut = pidPitch.compute(PITCH_SETPOINT - angles.pitch, dt);
    float yawOut   = pidYaw  .compute(YAW_SETPOINT   - angles.yaw,   dt);

    // 4. Drive motors
    motor_set(0, rollOut);
    motor_set(1, pitchOut);
    motor_set(2, yawOut);

    // 5. Serial telemetry (reduce rate to not flood)
    static uint16_t logCounter = 0;
    if (++logCounter >= 50) {   // Print every ~50ms at 1kHz
        logCounter = 0;
        Serial.printf("R:%.2f P:%.2f Y:%.2f | mR:%.2f mP:%.2f mY:%.2f\n",
                      angles.roll, angles.pitch, angles.yaw,
                      rollOut, pitchOut, yawOut);
    }
}
