# ⬛ 3D Cubli — Self-Balancing Reaction-Wheel Cube

<!-- ============================================================
  📸 IMAGE PLACEHOLDER #1 — DEMO GIF (MOST IMPORTANT)
  Record a 5–8 second video of the cube balancing on its edge.
  Convert to GIF at ezgif.com/video-to-gif (keep under 5MB).
  Save the file as:  docs/images/demo.gif
  Then uncomment the line below:
  ![Cubli Balancing Demo](docs/images/demo.gif)
============================================================ -->

<div align="center">

![Platform](https://img.shields.io/badge/Platform-ESP32-blue?style=for-the-badge&logo=espressif)
![IMU](https://img.shields.io/badge/IMU-MPU--6050-orange?style=for-the-badge)
![Control](https://img.shields.io/badge/Control-PID-green?style=for-the-badge)
![Motor](https://img.shields.io/badge/Motor-A2212--1400KV-red?style=for-the-badge)
![University](https://img.shields.io/badge/MSA_University-MSE464-purple?style=for-the-badge)
![License](https://img.shields.io/badge/License-MIT-lightgrey?style=for-the-badge)

**A reaction-wheel-based self-balancing cube**  

[Overview](#-overview) · [Math Model](#-mathematical-model) · [Hardware](#-hardware) · [Software](#-software) · [Wiring](#-wiring) · [Gallery](#-gallery) · [Results](#-results) · [Team](#-team)

</div>

---

## 📌 Overview

The **3D Cubli** is a self-balancing cube that uses a **reaction wheel** to maintain its upright position on an edge. When the cube begins to tilt, the brushless motor accelerates a flywheel in the opposite direction, generating a counter-torque that brings the cube back to equilibrium — with no external support.


**Key highlights:**
- ✅ Real-time attitude estimation via **MPU-6050** (I²C, fused gyro + accelerometer)
- ✅ **PID control loop** on **ESP32** running at ~100 Hz
- ✅ **A2212-1400KV** brushless motor + 30A ESC as reaction wheel
- ✅ **11.1V 3S1P LiPo** battery — fully self-contained system
- ✅ Custom **3D-printed PLA/ABS** cube enclosure
- ✅ Derived from first principles: EOM → Transfer Function → State Space → PID

> 🎥 Inspired by: [Cubli Control System – YouTube](https://www.youtube.com/watch?v=AJQZFHJzwt4)  
> 📄 ETH Zurich paper: [The Cubli: A cube that can jump up and balance](https://www.semanticscholar.org/paper/The-Cubli:-A-cube-that-can-jump-up-and-balance-Gajamohan-Merz/ff6d108d625f6025876f251585b97658f77d24ca/figure/0)

<!-- ============================================================
  🎥 VIDEO PLACEHOLDER #1 — YOUTUBE DEMO THUMBNAIL
  Upload your demo video to YouTube (can be unlisted).
  Replace YOUR_VIDEO_ID with the ID from your YouTube URL.
  Example: https://www.youtube.com/watch?v=AJQZFHJzwt4
                                                   ^^^^^^^^^^^
  Then uncomment the three lines below:

  [![Cubli Demo Video](https://img.youtube.com/vi/YOUR_VIDEO_ID/maxresdefault.jpg)](https://www.youtube.com/watch?v=YOUR_VIDEO_ID)
  > 🎬 Click the thumbnail above to watch the full demo
============================================================ -->

---

## 📐 Mathematical Model

### System Parameters

| Symbol | Description |
|--------|-------------|
| `m` | Total mass of the Cubli |
| `l` | Distance from pivot to centre of mass (after tilt) |
| `J_c` | Moment of inertia of cube body around pivot |
| `J_w` | Moment of inertia of reaction wheel |
| `J_eff` | Effective inertia = `J_c + J_w` |
| `θ` | Tilt angle from vertical |
| `τ_w` | Torque applied to reaction wheel |
| `ω_w` | Angular velocity of reaction wheel |

<!-- ============================================================
  📸 IMAGE PLACEHOLDER #2 — FREE BODY DIAGRAM
  Draw or scan your free body diagram showing the cube on its
  pivot, forces mg, reaction wheel torque τ_w, and angle θ.
  Save as: docs/images/free_body_diagram.png
  Then uncomment:
  ![Free Body Diagram](docs/images/free_body_diagram.png)
  
  You can also use the scan from Cubli_Task1.pdf page 1.
  Export it as a PNG and place it here.
============================================================ -->

### Equation of Motion

Using Newton's Second Law, for small tilt angles (sin θ ≈ θ):

```
(J_c + J_w) · θ̈ - mgl · θ = τ_w

  →   J_eff · θ̈  -  mgl · θ  =  τ_w
```

Where **J_eff = J_c + J_w** is the effective inertia of the system.

### Transfer Function

Taking the Laplace Transform:

```
J_eff · s²Θ(s)  -  mgl · Θ(s)  =  Tw(s)

        Θ(s)              1
G(s) = ──────  =  ─────────────────────
        Tw(s)      J_eff · s²  -  mgl
```

<!-- ============================================================
  📸 IMAGE PLACEHOLDER #3 — BODE / ROOT LOCUS / STEP RESPONSE PLOTS
  Export the Bode diagram, root locus, Nyquist, and step response
  plots from MATLAB or Python (control library).
  Save them as:
    docs/images/bode_diagram.png
    docs/images/root_locus.png
    docs/images/nyquist.png
    docs/images/step_response.png
  Then add them like:
  
  ### Bode Diagram
  ![Bode Diagram](docs/images/bode_diagram.png)

  ### Root Locus
  ![Root Locus](docs/images/root_locus.png)

  ### Step Response
  ![Step Response](docs/images/step_response.png)
============================================================ -->

### State Space Model

Letting **x₁ = θ** (tilt angle) and **x₂ = θ̇** (angular velocity):

```
⎡ ẋ₁ ⎤   ⎡      0          1  ⎤   ⎡ x₁ ⎤   ⎡      0      ⎤
⎢    ⎥ = ⎢                   ⎥ · ⎢    ⎥ + ⎢             ⎥ · τ_w
⎣ ẋ₂ ⎦   ⎣  mgl / J_eff     0  ⎦   ⎣ x₂ ⎦   ⎣  1 / J_eff  ⎦

y  =  [ 1   0 ] · [ x₁   x₂ ]ᵀ
```

### System Classification

| Property | Value |
|----------|-------|
| Type | SISO (Single Input, Single Output) |
| Order | 2nd order |
| Category | Mechanical system |
| Open-loop stability | **Unstable** — pole in right-half plane |
| Controller needed | Closed-loop PID |

---

## 🔩 Hardware

### Bill of Materials

| Component | Model | Specs | Role |
|-----------|-------|-------|------|
| Microcontroller | **ESP32 WROOM-32** | Dual-core 240 MHz, Wi-Fi, BT | Main CPU + control loop |
| IMU | **MPU-6050** | 3-axis gyro + 3-axis accel, I²C | Attitude estimation |
| Motor | **A2212-1400KV** | Brushless DC, 1400 KV | Reaction wheel actuator |
| Motor Controller | **ESC 30A** | PWM input, 11.1V compatible | Motor speed control |
| Battery | **3S1P LiPo** | 11.1V, 500 mAh | Main power supply |
| Frame | **3D Printed** | PLA / ABS | Structural enclosure |

> ⚠️ **Motor substitution note:** The Nidec 24H motors used in the original reference design were unavailable locally and import-restricted. We substituted with **A2212-1400KV** brushless motors, which provide comparable KV rating and torque output when paired with 30A ESCs.

<!-- ============================================================
  📸 IMAGE PLACEHOLDER #4 — COMPONENTS PHOTO
  Lay out all your components (ESP32, MPU-6050, motor, ESC,
  battery, 3D frame parts) on a flat surface and photograph them.
  Save as: docs/images/components.jpg
  Then uncomment:
  ![Components](docs/images/components.jpg)
============================================================ -->

### System Architecture

```
┌──────────────────────────────────────────────────────────────┐
│                          ESP32                               │
│                                                              │
│   ┌────────────┐  I²C    ┌──────────────────────────────┐   │
│   │  MPU-6050  │────────▶│  Sensor Fusion (comp filter) │   │
│   │  100 Hz    │  angle  │  +  PID Control Loop         │   │
│   └────────────┘         └───────────────┬──────────────┘   │
│                                          │ PWM (1000–2000µs) │
│                                 ┌────────▼────────┐          │
│                                 │    ESC  30A     │          │
└─────────────────────────────────┴────────┬────────┘          
                                            │ 3-phase
                                   ┌────────▼────────┐
                                   │  A2212-1400KV   │
                                   │  Reaction Wheel │
                                   └─────────────────┘
```

---

## 💻 Software

### Firmware Structure

```
firmware/
└── main/
    └── main.cpp     ← Full PID control loop (single file)
```

### Control Loop Logic

```
Every 10ms (~100Hz):
  1. mpu.update()              → Read fused IMU data
  2. angle = mpu.getAngleX()  → Get estimated tilt (degrees)
  3. error = setAngle - angle  → Compute deviation from target
  4. PID computation:
       integral   += error × dt
       derivative  = (error - prevError) / dt
       output      = Kp×e + Ki×∫e + Kd×ė
  5. pwm = constrain(1000 + output, 1000, 2000)
  6. esc.writeMicroseconds(pwm) → Command motor
  7. Serial.print(angle, pwm)  → Debug output
```

### PID Tuning Parameters

| Parameter | Value | Notes |
|-----------|-------|-------|
| `Kp` | **7.0** | Start here — increase until oscillation |
| `Ki` | **0.0** | Disabled — avoids integral windup during tuning |
| `Kd` | **1.0** | Add after Kp to dampen oscillations |
| `setAngle` | **20.0°** | Equilibrium tilt angle — adjust to your build |
| Loop rate | **~100 Hz** | 10ms delay per cycle |

### Full Source Code

```cpp
#include <Wire.h>
#include <ESP32Servo.h>
#include <MPU6050_light.h>

// ── Hardware ─────────────────────────────────────────────────────
Servo esc;
const int escPin = 13;       // PWM-capable GPIO pin on ESP32

// ── Setpoint ─────────────────────────────────────────────────────
float setAngle = 20.0;       // Target upright angle (degrees)
                             // Tune this to your physical build

// ── PID Gains ────────────────────────────────────────────────────
float Kp = 7.0;
float Ki = 0.0;
float Kd = 1.0;

float pidOutput     = 0;
float previousError = 0;
float integral      = 0;

// ── IMU ──────────────────────────────────────────────────────────
MPU6050 mpu(Wire);

void setup() {
  Serial.begin(115200);
  Wire.begin();

  // Arm ESC at minimum throttle first
  esc.attach(escPin, 1000, 2000);
  esc.writeMicroseconds(1000);

  // Initialize MPU-6050
  byte status = mpu.begin();
  Serial.println(status == 0 ? "MPU initialized!" : "MPU failed!");
  delay(1000);

  mpu.calcOffsets();   // Auto-calibrate — keep cube perfectly still!
  Serial.println("Calibration complete. Running...");
}

void loop() {
  mpu.update();
  float angle = mpu.getAngleX();   // Pitch angle for 2D Cubli

  // PID computation
  float error      = setAngle - angle;
  integral        += error * 0.01;
  float derivative = (error - previousError) / 0.01;
  pidOutput        = Kp * error + Ki * integral + Kd * derivative;
  previousError    = error;

  // Map PID output → ESC pulse width [1000–2000 µs]
  int pwm = constrain((int)(1000 + pidOutput), 1000, 2000);
  esc.writeMicroseconds(pwm);

  // Serial telemetry
  Serial.print("Angle: ");    Serial.print(angle);
  Serial.print(" | Error: "); Serial.print(error);
  Serial.print(" | PWM: ");   Serial.println(pwm);

  delay(10);   // 100 Hz loop
}
```

<!-- ============================================================
  📸 IMAGE PLACEHOLDER #5 — SERIAL MONITOR SCREENSHOT
  Run your cube and take a screenshot of the serial monitor
  showing the Angle / Error / PWM telemetry output.
  Save as: docs/images/serial_monitor.png
  Then uncomment:

  ### Serial Monitor Output
  ![Serial Monitor](docs/images/serial_monitor.png)
============================================================ -->

---

## 🔌 Wiring

<!-- ============================================================
  📸 IMAGE PLACEHOLDER #6 — WIRING DIAGRAM
  Create your wiring diagram using one of these free tools:
    • Fritzing   → fritzing.org
    • draw.io    → draw.io
    • Wokwi      → wokwi.com (ESP32 simulator)
  Export as PNG and save to: hardware/wiring/wiring_diagram.png
  Then uncomment:
  ![Wiring Diagram](hardware/wiring/wiring_diagram.png)
============================================================ -->

### Connection Table

```
MPU-6050                  ESP32 (WROOM-32)
────────────────────────────────────────────────
SDA              →    GPIO 21
SCL              →    GPIO 22
VCC              →    3.3V
GND              →    GND (common rail)

ESC Signal       →    GPIO 13   (PWM output)
ESC Power  +     →    LiPo Battery  +  (11.1V)
ESC Power  –     →    LiPo Battery  –  (GND)

A2212 Motor      →    ESC (3 phase wires)
ESP32 VIN        →    5V buck converter output
All GNDs         →    Common Ground Rail
```

### Power Architecture

```
LiPo 11.1V (3S1P, 500mAh)
         │
         ├──────────────────────→  ESC (direct 11.1V)
         │                               └──→ A2212 Motor
         │
         └──→ 5V Buck Converter
                     └──→ ESP32 VIN
                               └──→ 3.3V LDO (on-board)
                                         └──→ MPU-6050 VCC
```

> ⚠️ **Important:** Tie **all grounds** (battery, ESC, ESP32, MPU-6050) to a **single common ground rail**. Floating references cause noisy IMU readings and erratic control behavior.

---

## 🖼️ Gallery

<!-- ============================================================
  📸 IMAGE PLACEHOLDER #7 — ASSEMBLED CUBE
  Photograph the fully assembled cube on a clean background.
  Good lighting makes a huge difference.
  Save as: docs/images/assembled.jpg
  Then replace the comment below with:
  ![Assembled Cube](docs/images/assembled.jpg)
============================================================ -->

<!-- ============================================================
  📸 IMAGE PLACEHOLDER #8 — INTERNAL ELECTRONICS
  Open the cube and photograph the internals:
  motor mount, wiring, ESC, battery placement.
  Save as: docs/images/internals.jpg
  Then replace the comment below with:
  ![Internals](docs/images/internals.jpg)
============================================================ -->

<!-- ============================================================
  📸 IMAGE PLACEHOLDER #9 — PCB / WIRING CLOSE-UP
  Close-up photo of your hand-soldered board or breadboard wiring.
  Save as: docs/images/pcb.jpg
  Then replace the comment below with:
  ![PCB Close-up](docs/images/pcb.jpg)
============================================================ -->

<!-- ============================================================
  📸 IMAGE PLACEHOLDER #10 — 3D PRINTED PARTS (before assembly)
  Photograph all your 3D-printed parts laid out before assembly.
  Save as: docs/images/3d_parts.jpg
  Then replace the comment below with:
  ![3D Printed Parts](docs/images/3d_parts.jpg)
============================================================ -->

> 📂 All images go in `docs/images/` — see the [image guide](docs/images/README.md) for full instructions.

---

## ⚠️ Challenges & Solutions

| Challenge | Root Cause | Solution |
|-----------|------------|----------|
| Nidec 24H motor unavailable | Import-restricted locally | Substituted **A2212-1400KV** — similar KV, compatible with 30A ESC |
| Motor stiction / slow response | ESC dead-band + low Kd | Tuned PID, selected fast-response ESC |
| IMU drift over time | Gyro bias accumulates | `mpu.calcOffsets()` on startup; complementary filter in library |
| Power noise affecting IMU | Shared power rail with motors | 100µF decoupling capacitor on 5V rail; common GND |
| `setAngle` mismatch | Physical CoM not at 0° | Measured equilibrium manually, set `setAngle = 20.0` |

---

## 📊 Results

| Metric | Value |
|--------|-------|
| Control loop rate | ~100 Hz (10ms delay) |
| Kp / Ki / Kd | 7.0 / 0.0 / 1.0 |
| Equilibrium angle | 20° |
| Battery life | ~20 min (active balancing) |
| Motor | A2212-1400KV + 30A ESC |
| System order | 2nd order, SISO |
| Open-loop stability | Unstable (requires closed-loop PID) |

<!-- ============================================================
  📊 RESULTS PLACEHOLDER — MATLAB / PYTHON PLOTS
  If you ran simulations in MATLAB or Python, export:
    • Step response plot
    • Bode diagram
    • Root locus
  Save in docs/images/ and add them here like:
  ![Step Response](docs/images/step_response.png)
============================================================ -->

---

## 📁 Repository Structure

```
cubli-esp32/
├── README.md                        ← You are here
├── LICENSE                          ← MIT License
├── .gitignore                       ← Ignores build artifacts
├── platformio.ini                   ← PlatformIO build config
│
├── firmware/
│   └── main/
│       └── main.cpp                 ← Full PID control firmware
│
├── hardware/
│   ├── 3d-models/
│   │   ├── README.md                ← Print settings guide
│   │   └── *.stl                    ✅ 3D models complete — drop STL files here
│   ├── pcb/
│   │   ├── README.md                ← BOM + schematic notes
│   │   └── schematic.png            ← [ADD YOUR SCHEMATIC IMAGE HERE]
│   └── wiring/
│       ├── README.md                ← Wiring notes
│       └── wiring_diagram.png       ← [ADD YOUR WIRING DIAGRAM HERE]
│
└── docs/
    ├── Cubli_Task1.pdf              ← Handwritten EOM derivation
    ├── report_auto.docx             ← Full project report
    ├── cubili.pptx                  ← Project presentation slides
    └── images/
        ├── README.md                ← Image guide
        ├── demo.gif                 ← [ADD BALANCING DEMO GIF HERE]
        ├── assembled.jpg            ← [ADD ASSEMBLED CUBE PHOTO]
        ├── internals.jpg            ← [ADD INTERNALS PHOTO]
        ├── pcb.jpg                  ← [ADD PCB CLOSE-UP PHOTO]
        ├── serial_monitor.png       ← [ADD SERIAL MONITOR SCREENSHOT]
        └── components.jpg           ← [ADD COMPONENTS PHOTO]
```

---

## 🚀 Getting Started

### Prerequisites

- [PlatformIO](https://platformio.org/) (VS Code extension recommended)
- ESP32 board support installed via PlatformIO

### Flash the Firmware

```bash
# Clone the repository
git clone https://github.com/YOUR_USERNAME/cubli-esp32.git
cd cubli-esp32

# Upload to ESP32
pio run --target upload

# Open serial monitor
pio device monitor --baud 115200
```

### Calibration Procedure

1. Place the cube on a **flat surface**, completely still.
2. Power on — wait 3 seconds while `mpu.calcOffsets()` runs.
3. Watch the serial monitor — confirm angle reads ~0° on flat.
4. Balance the cube manually on its edge and read the angle.
5. Set that value as `setAngle` in `main.cpp`.
6. Re-upload and test.

### PID Tuning Guide

```
Step 1: Set Ki = 0, Kd = 0. Increase Kp until cube starts to oscillate.
Step 2: Back off Kp by ~20%.
Step 3: Increase Kd slowly until oscillations are damped.
Step 4: Only add Ki if there is steady-state offset — use very small values.
```

---

## 🔗 References

- 📹 [Cubli Control System – YouTube](https://www.youtube.com/watch?v=AJQZFHJzwt4)
- 📄 [The Cubli: A cube that can jump up and balance – Semantic Scholar](https://www.semanticscholar.org/paper/The-Cubli:-A-cube-that-can-jump-up-and-balance-Gajamohan-Merz/ff6d108d625f6025876f251585b97658f77d24ca/figure/0)
- 📄 [Balancing Cube – Willem Pennings (2024)](https://willempennings.nl/balancing-cube/)
- 📐 [Nonlinear Dynamics and Control of a Cube Robot – MDPI Mathematics, 2020](https://www.mdpi.com/2227-7390/8/10/1840)
- 📦 [MPU6050_light Arduino Library](https://github.com/rfetick/MPU6050_light)
- 📦 [ESP32Servo Library](https://github.com/madhephaestus/ESP32Servo)

---

## 📄 License

This project is licensed under the MIT License — see [LICENSE](LICENSE) for details.

---

<div align="center">
⭐ If you found this helpful, please star the repo!
</div>
