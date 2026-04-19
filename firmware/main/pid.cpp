#include "pid.h"
#include <Arduino.h>

PID::PID(float kp, float ki, float kd, float outMin, float outMax)
    : _kp(kp), _ki(ki), _kd(kd),
      _integral(0), _prevError(0),
      _outMin(outMin), _outMax(outMax) {}

float PID::compute(float error, float dt) {
    _integral += error * dt;

    // Anti-windup clamp
    _integral = constrain(_integral, _outMin / _ki, _outMax / _ki);

    float derivative = (error - _prevError) / dt;
    _prevError = error;

    float output = _kp * error + _ki * _integral + _kd * derivative;
    return constrain(output, _outMin, _outMax);
}

void PID::setGains(float kp, float ki, float kd) {
    _kp = kp;  _ki = ki;  _kd = kd;
}

void PID::reset() {
    _integral  = 0;
    _prevError = 0;
}
