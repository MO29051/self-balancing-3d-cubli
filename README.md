# Self-Balancing 3D Cubli (Reaction-Wheel Cube)

## 📌 Overview

This project presents the design and implementation of a self-balancing 3D Cubli, a cube capable of stabilizing itself on an edge or corner using a reaction wheel.

The system behaves as a 3D inverted pendulum and is stabilized through feedback control using real-time sensor data and motor actuation.

---

## 🎯 Objectives

* Stabilize an unstable 3D system
* Implement real-time feedback control
* Demonstrate reaction-wheel based balancing

---

## ⚙️ Working Principle

The system uses a reaction wheel to generate torque. By accelerating or decelerating the wheel, an opposite torque is applied to the cube, allowing control of its orientation.

This follows Newton’s third law:
Torque applied to the wheel produces an equal and opposite torque on the cube.

---

## 🚀 Key Features

* Reaction-wheel stabilization
* IMU-based orientation sensing
* Real-time control loop
* Balancing on edge

---

## ⚙️ System Architecture

IMU → Arduino → Control Algorithm → Motor Driver → Reaction Wheel → Cube Motion

---

## 🔩 Hardware Components

* Arduino Uno
* IMU sensor (MPU6050)
* DC/BLDC motor with flywheel
* Motor driver
* 3D-printed cube frame

---

## 💻 Software & Tools

* Arduino IDE
* MATLAB / Simulink (for modeling)
* Serial communication for debugging

---

## 📊 Results

* Successful edge balancing
* Stable response under small disturbances
* Improved performance after tuning control gains

---

## 📷 Preview

(Add images here)

---

## 🎥 Demo

(Add video link)

---

## 📘 Documentation

* System Modeling → docs/system-modeling.md
* Control Design → docs/control-design.md
* Implementation → docs/implementation.md
* Results → docs/results.md

---

## 🧠 Engineering Highlights

* Stabilized nonlinear unstable system
* Designed real-time feedback controller
* Integrated sensing, control, and actuation

---

## 🔮 Future Improvements

* Corner balancing
* State-space control optimization
* Improved disturbance rejection
