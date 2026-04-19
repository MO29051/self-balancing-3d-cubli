# ⬛ Self-Balancing 3D Cubli (Reaction-Wheel Cube)

<div align="center">

![Platform](https://img.shields.io/badge/Platform-ESP32-blue?style=for-the-badge&logo=espressif)
![IMU](https://img.shields.io/badge/IMU-MPU--6050-orange?style=for-the-badge)
![Control](https://img.shields.io/badge/Control-PID-green?style=for-the-badge)
![License](https://img.shields.io/badge/License-MIT-purple?style=for-the-badge)
![Status](https://img.shields.io/badge/Status-Active-brightgreen?style=for-the-badge)

**A reaction-wheel-based self-balancing cube inspired by [ETH Zurich's Cubli](https://idsc.ethz.ch/research-dandrea/research-projects/archive/cubli.html)**

[Overview](#-overview) • [Hardware](#-hardware) • [Software](#-software) • [Getting Started](#-getting-started) • [Results](#-results) • [References](#-references)

</div>

---

## 📌 Overview

The **Cubli** is a 15×15×15 cm cube capable of balancing on its corner and edges using three **reaction wheels** — one per axis. This project replicates that concept using off-the-shelf components, custom 3D-printed parts, and a hand-soldered PCB.

Key achievements:
- ✅ Real-time attitude estimation via **MPU-6050** (gyroscope + accelerometer)
- ✅ **PID control loop** running on **ESP32** at ~1 kHz
- ✅ Custom **50×70 mm prototype PCB** with integrated power management
- ✅ **Nidec 24H brushless motors** as reaction wheels
- ✅ Fully self-contained **LiPo-powered** embedded system
- ✅ Custom **3D-printed enclosure** for all three axes

> 🎥 Inspired by: [Cubli Control System – YouTube](https://www.youtube.com/watch?v=AJQZFHJzwt4)

---

## 🔩 Hardware

### Components

| Component | Model | Purpose |
|-----------|-------|---------|
| Microcontroller | ESP32 (WROOM-32) | Main compute & control loop |
| IMU | MPU-6050 | Gyroscope + Accelerometer (I²C) |
| Motors | Nidec 24H | Brushless DC reaction wheels |
| Motor Driver | Custom/ESC | PWM speed control |
| Battery | LiPo (7.4V, 2S) | Power supply |
| PCB | 50×70mm prototype | Custom hand-soldered board |
| Enclosure | PLA 3D Print | Custom-designed cube frame |

### System Architecture

```
┌─────────────────────────────────────────────────────┐
│                    ESP32 Core                        │
│                                                      │
│   ┌──────────┐    PID    ┌───────────────────────┐  │
│   │ MPU-6050 │──────────▶│  Control Loop (~1kHz) │  │
│   │  (I²C)   │  angles   │   Roll / Pitch / Yaw  │  │
│   └──────────┘           └──────────┬────────────┘  │
│                                     │ PWM            │
│                          ┌──────────▼────────────┐  │
│                          │   Motor Drivers (x3)  │  │
│                          └──────────┬────────────┘  │
│                                     │               │
└─────────────────────────────────────┼───────────────┘
                                      │
                         ┌────────────▼────────────┐
                         │   Nidec 24H Motors (x3)  │
                         │   Reaction Wheels         │
                         └──────────────────────────┘
```

### PCB Design

The 50×70mm hand-soldered prototype PCB integrates:
- ESP32 module headers
- MPU-6050 breakout interface
- 3× motor driver signal routing
- LiPo battery connector + voltage regulation
- Status LEDs & reset/boot buttons

### 3D Printed Parts

All mechanical parts are printed in **PLA** with the following specs:
- Layer height: 0.2mm
- Infill: 30%
- Supports: Yes (for motor mounts)

STL files are located in [`/hardware/3d-models/`](hardware/3d-models/).

---

## 💻 Software

### Firmware Architecture

```
firmware/
├── main/
│   ├── main.cpp            # Entry point, task scheduler
│   ├── imu.cpp / .h        # MPU-6050 driver & complementary filter
│   ├── pid.cpp / .h        # PID controller (3-axis)
│   ├── motor.cpp / .h      # Motor PWM control
│   └── config.h            # Tuning parameters & pin definitions
└── lib/
    └── ...                 # External libraries
```

### Control Loop

The control system uses a **Complementary Filter** to fuse gyroscope and accelerometer data from the MPU-6050, estimating roll, pitch, and yaw angles. These are fed into three independent **PID controllers**, one per axis, which command the reaction wheel motors.

```
θ_estimated = α × (θ_prev + ω × dt) + (1 - α) × θ_accel
                    └─── Gyro ────┘       └── Accel ──┘

error = θ_setpoint - θ_estimated
output = Kp × error + Ki × ∫error dt + Kd × dError/dt
```

### PID Tuning Parameters

| Axis | Kp | Ki | Kd |
|------|----|----|----|
| Roll | `TBD` | `TBD` | `TBD` |
| Pitch | `TBD` | `TBD` | `TBD` |
| Yaw | `TBD` | `TBD` | `TBD` |

> ⚠️ These values are hardware-specific. Start with Kp and increase until oscillation, then add Kd to dampen.

---

## 🚀 Getting Started

### Prerequisites

- [PlatformIO](https://platformio.org/) or Arduino IDE with ESP32 support
- ESP32 board definitions installed
- USB-to-Serial adapter (or onboard USB on your ESP32 variant)

### Wiring

```
ESP32 GPIO   →   Component
─────────────────────────────
GPIO 21 (SDA) → MPU-6050 SDA
GPIO 22 (SCL) → MPU-6050 SCL
GPIO 3.3V     → MPU-6050 VCC
GND           → MPU-6050 GND

GPIO 25       → Motor Driver 1 (PWM)
GPIO 26       → Motor Driver 2 (PWM)
GPIO 27       → Motor Driver 3 (PWM)
```

### Flashing the Firmware

```bash
# Clone the repository
git clone https://github.com/YOUR_USERNAME/cubli-esp32.git
cd cubli-esp32

# Open in PlatformIO
pio run --target upload

# Monitor serial output
pio device monitor --baud 115200
```

### Calibration

1. Place the cube on a flat surface before powering on.
2. The MPU-6050 will auto-calibrate during the first 3 seconds (keep it still).
3. Adjust `Kp`, `Ki`, `Kd` in `config.h` for your specific build.

---

## 📊 Results

| Metric | Value |
|--------|-------|
| Balancing axis | Corner (1-axis stable, WIP) |
| Control loop rate | ~1 kHz |
| Settling time | ~500ms |
| Battery life | ~20 min (active balancing) |
| Total build cost | ~$40 USD |

---

## 📁 Repository Structure

```
cubli-esp32/
├── firmware/               # ESP32 source code (PlatformIO)
│   ├── main/
│   └── lib/
├── hardware/
│   ├── 3d-models/          # STL files for printing
│   ├── pcb/                # PCB schematics & layout
│   └── wiring-diagram/     # Connection diagrams
├── docs/
│   ├── report.pdf          # Project report (if any)
│   └── images/             # Photos & videos
├── platformio.ini          # PlatformIO config
└── README.md
```

---

## 🔗 References

- 📄 [ETH Zurich Cubli Project](https://idsc.ethz.ch/research-dandrea/research-projects/archive/cubli.html)
- 🎥 [Cubli Control System – YouTube](https://www.youtube.com/watch?v=AJQZFHJzwt4)
- 📦 [MPU-6050 Datasheet](https://invensense.tdk.com/products/motion-tracking/6-axis/mpu-6050/)
- 📦 [ESP32 Technical Reference](https://www.espressif.com/en/support/documents/technical-documents)
- 📐 [PID Control Theory – Wikipedia](https://en.wikipedia.org/wiki/Proportional%E2%80%93integral%E2%80%93derivative_controller)

---

## 👤 Author

**Your Name**
- GitHub: [@YOUR_USERNAME](https://github.com/YOUR_USERNAME)
- LinkedIn: [your-linkedin](https://linkedin.com/in/your-linkedin)

---

## 📄 License

This project is licensed under the MIT License — see the [LICENSE](LICENSE) file for details.

---

<div align="center">
⭐ Star this repo if you found it helpful!
</div>
