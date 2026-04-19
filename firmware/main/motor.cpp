#include "motor.h"
#include "config.h"
#include <ESP32Servo.h>

static Servo motors[3];
static const uint8_t MOTOR_PINS[3] = {
    MOTOR1_PWM_PIN, MOTOR2_PWM_PIN, MOTOR3_PWM_PIN
};

void motor_init() {
    for (int i = 0; i < 3; i++) {
        motors[i].attach(MOTOR_PINS[i], MOTOR_PWM_MIN, MOTOR_PWM_MAX);
        motors[i].writeMicroseconds(MOTOR_PWM_IDLE);
    }
    Serial.println("[MOTOR] ESC arming delay...");
    delay(2000);   // Give ESCs time to arm
    Serial.println("[MOTOR] Motors ready.");
}

void motor_set(uint8_t idx, float speed) {
    if (idx >= 3) return;
    speed = constrain(speed, -1.0f, 1.0f);
    int pwm = (int)(MOTOR_PWM_IDLE + speed * (MOTOR_PWM_MAX - MOTOR_PWM_IDLE));
    motors[idx].writeMicroseconds(pwm);
}

void motor_stop_all() {
    for (int i = 0; i < 3; i++)
        motors[i].writeMicroseconds(MOTOR_PWM_IDLE);
}
