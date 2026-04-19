#pragma once

class PID {
public:
    PID(float kp, float ki, float kd, float outMin, float outMax);

    // Compute next output given current error and timestep (s)
    float compute(float error, float dt);

    void  setGains(float kp, float ki, float kd);
    void  reset();

private:
    float _kp, _ki, _kd;
    float _integral;
    float _prevError;
    float _outMin, _outMax;
};
