# 🌊 AquaRegen — Floating Natural Water Treatment System

> **MFB2102 Engineering Team Project II | Jan '26 | ETP Group 19**

![SDG 6](https://img.shields.io/badge/SDG%206-Clean%20Water%20%26%20Sanitation-00ADEF?style=flat-square)
![SDG 11](https://img.shields.io/badge/SDG%2011-Sustainable%20Cities-F89D2A?style=flat-square)
![Platform](https://img.shields.io/badge/Platform-ESP32-red?style=flat-square)
![Status](https://img.shields.io/badge/Status-Prototype-brightgreen?style=flat-square)
![Cost](https://img.shields.io/badge/Build%20Cost-RM%20416.33-blue?style=flat-square)

---

## 📋 Table of Contents

- [Project Overview](#-project-overview)
- [Hardware](#-hardware)
- [How to Set Up the Blynk App in Arduino IDE](#-how-to-set-up-the-blynk-app-in-arduino-ide)
- [Lab Results](#-lab-results)
- [Cost Analysis](#-cost-analysis)
- [Team Members](#-team-members)

---

## 🌿 Project Overview

**AquaRegen** is a floating natural water treatment system designed to combat algae blooms and eutrophication in small to medium-scale lakes — without the use of harsh chemicals.

### Problem

V7 Lake (Universiti Petronas) has been experiencing persistent algae blooms caused by:
- Untreated resident wastewater
- Improper waste discharge from nearby cafes
- Fertilizer leakage

**84%** of surveyed users agree that algae growth negatively affects their quality of life, and **76%** identified untreated wastewater as the primary cause.

### Solution

AquaRegen addresses this through **3 core principles**:

| # | Principle | Description |
|---|-----------|-------------|
| 1 | 🛟 **Floating Platform** | Polystyrene + polyethene board structure for mobility, covering a larger surface area of the lake |
| 2 | 🌱 **Natural Water Treatment** | Water hyacinths absorb phosphorus and nitrogen; an onboard pump circulates water to raise dissolved oxygen levels |
| 3 | 🕹️ **Manual Steering System** | ESP32-controlled water pumps act as thrusters, preventing the platform from colliding with lake banks |

### Target Market

> Fish farms · Recreational lakes · Campus lakes · Industrial ponds

### Future Improvements

- 📱 Mobile app integration (Blynk / custom app)
- 📊 Real-time water quality data tracking
- ☀️ Solar power integration
- 🤖 Enhanced autonomous mobility

---

## 🔧 Hardware

### Component List

#### 🧱 Structure / Main Body

| Component | Qty | Unit Price (RM) | Total (RM) |
|-----------|-----|----------------|------------|
| Polystyrene Board | 1 | 5.90 | 5.90 |
| Polyethene Board | 1 | 10.80 | 10.80 |
| PVC Pipe / Foot | 8 | 1.60 | 12.80 |
| PVC Elbow | 4 | 1.60 | 6.40 |

#### 💧 Water Treatment System

| Component | Qty | Unit Price (RM) | Total (RM) |
|-----------|-----|----------------|------------|
| Water Filter | 1 | 16.71 | 16.71 |
| Main Water Pump (12V) | 1 | 31.24 | 31.24 |
| Plant Pots | 4 | 1.30 | 5.20 |

#### 🔋 Power Source

| Component | Qty | Unit Price (RM) | Total (RM) |
|-----------|-----|----------------|------------|
| LiPo Battery 1100mAh 11.1V (logic) | 1 | 39.90 | 39.90 |
| LiPo Battery 5200mAh 11.1V (main) | 1 | 129.90 | 129.90 |

#### 🚤 Steering System

| Component | Qty | Unit Price (RM) | Total (RM) |
|-----------|-----|----------------|------------|
| Marine Water Pump (12V bilge) | 2 | 22.56 | 45.12 |
| Propeller | 2 | 3.20 | 6.40 |

#### ⚡ Electronics

| Component | Qty | Unit Price (RM) | Total (RM) |
|-----------|-----|----------------|------------|
| ESP32 Dev Board + USB | 1 | 32.90 | 32.90 |
| LM2596 Buck Converter (step-down) | 1 | 14.50 | 14.50 |
| BTS7960 H-Bridge Motor Driver | 2 | 15.63 | 31.26 |
| 2.5mm Cable | 2 | 3.80 | 7.60 |
| 0.3mm Cable | 2 | 0.85 | 1.70 |
| 20A Blade Fuse Holder | 1 | 5.00 | 5.00 |
| 12V Rocker Switch | 1 | 3.10 | 3.10 |

#### 🔩 Miscellaneous

| Component | Qty | Unit Price (RM) | Total (RM) |
|-----------|-----|----------------|------------|
| Silicone Sealant | 1 | 8.50 | 8.50 |
| Zip Ties | 1 | 1.40 | 1.40 |

> **💰 Total Build Cost: RM 416.33**

---

### Circuit Architecture

The electrical system is split into two isolated power rails:

```
┌─────────────────────────────────────────────────────────┐
│              BRAIN POWER SYSTEM (Logic)                 │
│  Battery 2 (11.1V 1100mAh LiPo)                        │
│       └─► LM2596 Buck Converter (→ 5V)                 │
│               └─► ESP32 Dev Board                      │
│                     ├─ GPIO 25 (Enable)                 │
│                     ├─ GPIO 26 (Forward)  ──► BTS7960   │
│                     ├─ GPIO 33 (Reverse)  Right Driver  │
│                     ├─ GPIO 13 (Enable)                 │
│                     ├─ GPIO 14 (Forward)  ──► BTS7960   │
│                     └─ GPIO 27 (Reverse)  Left Driver   │
└─────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────┐
│           MUSCLE POWER SYSTEM (High Current)            │
│  Battery 1 (11.1V 5200mAh LiPo)                        │
│       └─► 20A Blade Fuse                               │
│               └─► 12V Rocker Switch                    │
│                     ├─► Central Algae Pump (Auto ON)   │
│                     ├─► BTS7960 Right Driver           │
│                     │       └─► Right Steering Pump    │
│                     └─► BTS7960 Left Driver            │
│                             └─► Left Steering Pump     │
└─────────────────────────────────────────────────────────┘

⚠️  CRITICAL: Both GND rails must be tied together (common ground)
```

### Pin Mapping (ESP32)

| GPIO Pin | Function | Connected To |
|----------|----------|--------------|
| 25 | Enable (Right) | BTS7960 Right Driver |
| 26 | Forward (Right) | BTS7960 Right Driver |
| 33 | Reverse (Right) | BTS7960 Right Driver |
| 13 | Enable (Left) | BTS7960 Left Driver |
| 14 | Forward (Left) | BTS7960 Left Driver |
| 27 | Reverse (Left) | BTS7960 Left Driver |

---

## 📱 How to Set Up the Blynk App in Arduino IDE

This guide walks you through connecting AquaRegen's ESP32 to the **Blynk IoT platform** for wireless control.

### Step 1 — Install the Arduino IDE

Download and install the [Arduino IDE](https://www.arduino.cc/en/software) (version 2.x recommended).

---

### Step 2 — Add ESP32 Board Support

1. Open Arduino IDE → go to **File > Preferences**
2. In the *Additional Board Manager URLs* field, paste:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
3. Click **OK**
4. Go to **Tools > Board > Boards Manager**
5. Search for `esp32` and install **"esp32 by Espressif Systems"**
6. Select your board: **Tools > Board > ESP32 Arduino > ESP32 Dev Module**

---

### Step 3 — Install the Blynk Library

1. Go to **Sketch > Include Library > Manage Libraries**
2. Search for `Blynk`
3. Install **"Blynk by Volodymyr Shymanskyy"** (version 1.3.x or later)

---

### Step 4 — Create a Blynk Account & Project

1. Download the **Blynk IoT** app on your phone ([Android](https://play.google.com/store/apps/details?id=cloud.blynk) / [iOS](https://apps.apple.com/app/blynk-iot/id1559317868))
2. Sign up at [blynk.cloud](https://blynk.cloud)
3. Click **+ New Template** and fill in:
   - **Name**: `AquaRegen`
   - **Hardware**: `ESP32`
   - **Connection Type**: `WiFi`
4. Go to **Datastreams** and create Virtual Pins:

| Virtual Pin | Name | Data Type | Purpose |
|-------------|------|-----------|---------|
| V0 | Forward | Integer (0/1) | Move forward |
| V1 | Reverse | Integer (0/1) | Move backward |
| V2 | Turn Left | Integer (0/1) | Left pump only |
| V3 | Turn Right | Integer (0/1) | Right pump only |
| V4 | Pump Toggle | Integer (0/1) | Main algae pump |

5. Go to **Web Dashboard** or **Mobile Dashboard** and add **Button** widgets mapped to each virtual pin.
6. Note down your **Template ID**, **Template Name**, and **Auth Token** from the template settings.

---

### Step 5 — Write the Firmware

Create a new sketch in Arduino IDE and use the following as your starting template:

```cpp
// AquaRegen — ESP32 Blynk Firmware
// ETP Group 19 | MFB2102 Jan '26

#define BLYNK_TEMPLATE_ID   "YOUR_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "AquaRegen"
#define BLYNK_AUTH_TOKEN    "YOUR_AUTH_TOKEN"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// --- WiFi Credentials ---
char ssid[] = "YOUR_WIFI_SSID";
char pass[] = "YOUR_WIFI_PASSWORD";

// --- Pin Definitions ---
// Right BTS7960 Driver
#define RIGHT_EN  25
#define RIGHT_FWD 26
#define RIGHT_REV 33

// Left BTS7960 Driver
#define LEFT_EN   13
#define LEFT_FWD  14
#define LEFT_REV  27

// --- Blynk Virtual Pin Handlers ---

BLYNK_WRITE(V0) {  // Forward
  int val = param.asInt();
  digitalWrite(RIGHT_EN, HIGH);
  digitalWrite(LEFT_EN, HIGH);
  digitalWrite(RIGHT_FWD, val);
  digitalWrite(LEFT_FWD, val);
  digitalWrite(RIGHT_REV, LOW);
  digitalWrite(LEFT_REV, LOW);
}

BLYNK_WRITE(V1) {  // Reverse
  int val = param.asInt();
  digitalWrite(RIGHT_EN, HIGH);
  digitalWrite(LEFT_EN, HIGH);
  digitalWrite(RIGHT_REV, val);
  digitalWrite(LEFT_REV, val);
  digitalWrite(RIGHT_FWD, LOW);
  digitalWrite(LEFT_FWD, LOW);
}

BLYNK_WRITE(V2) {  // Turn Left (left pump only)
  int val = param.asInt();
  digitalWrite(LEFT_EN, HIGH);
  digitalWrite(RIGHT_EN, LOW);
  digitalWrite(LEFT_FWD, val);
  digitalWrite(LEFT_REV, LOW);
}

BLYNK_WRITE(V3) {  // Turn Right (right pump only)
  int val = param.asInt();
  digitalWrite(RIGHT_EN, HIGH);
  digitalWrite(LEFT_EN, LOW);
  digitalWrite(RIGHT_FWD, val);
  digitalWrite(RIGHT_REV, LOW);
}

void setup() {
  Serial.begin(115200);

  // Set all motor pins as output
  pinMode(RIGHT_EN, OUTPUT);  pinMode(RIGHT_FWD, OUTPUT);  pinMode(RIGHT_REV, OUTPUT);
  pinMode(LEFT_EN, OUTPUT);   pinMode(LEFT_FWD, OUTPUT);   pinMode(LEFT_REV, OUTPUT);

  // All motors off initially
  digitalWrite(RIGHT_EN, LOW); digitalWrite(LEFT_EN, LOW);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  Serial.println("AquaRegen Online ✅");
}

void loop() {
  Blynk.run();
}
```

> ⚠️ Replace `YOUR_TEMPLATE_ID`, `YOUR_AUTH_TOKEN`, `YOUR_WIFI_SSID`, and `YOUR_WIFI_PASSWORD` with your actual values.

---

### Step 6 — Upload & Test

1. Connect your ESP32 to your PC via USB
2. Select the correct **Port**: **Tools > Port > COMx** (Windows) or `/dev/ttyUSB0` (Linux/Mac)
3. Click the **Upload** button (→)
4. Open **Serial Monitor** at **115200 baud** and confirm:
   ```
   AquaRegen Online ✅
   ```
5. Open the Blynk app, tap the buttons, and verify the motors respond

---

### Troubleshooting

| Issue | Fix |
|-------|-----|
| ESP32 not detected | Install CP210x or CH340 USB driver |
| Blynk not connecting | Double-check WiFi credentials and Auth Token |
| Motors not responding | Verify common GND between brain and muscle rails |
| Upload fails | Hold the `BOOT` button on ESP32 while uploading |

---

## 🧪 Lab Results

Testing was conducted using lake water samples over a 24-hour period with the AquaRegen prototype active.

| Parameter | At 0 Hours | After 24 Hours | Improvement |
|-----------|-----------|----------------|-------------|
| Turbidity | 19.9 NTU | 16.2 NTU | ✅ −18.6% |
| Ammonia–Nitrogen Content | 1.4 mg/L | 0.4 mg/L | ✅ −71.4% |
| Phosphate Content | 0.49 mg/L | 0.43 mg/L | ✅ −12.2% |

> The most significant result was the **71.4% reduction in ammonia-nitrogen**, a key nutrient driving algae blooms.

---

## 💰 Cost Analysis

| Category | Total (RM) |
|----------|------------|
| Structure / Main Body | 35.90 |
| Water Treatment System | 53.15 |
| Power Source | 169.80 |
| Steering System | 51.52 |
| Electronic System | 96.06 |
| Miscellaneous | 9.90 |
| **Grand Total** | **RM 416.33** |

**Estimated market value: RM 650 – RM 750 → ROI ≈ 56%**

---

## 👥 Team Members

| Name | Role |
|------|------|
| Muhammad Iqbaal bin Zahid | Team Member |
| Emir Azimil Akbar bin Mohd Fauzi | Team Member |
| Muhammad Hafizuddin bin Mahasan | Team Member |
| Vanessa Valery Tan Yinn Yinn | Team Member |
| Nor Izzaty Imanina binti Nazri | Team Member |

---

<div align="center">

# License

This project is developed for academic purposes at Universiti Teknologi PETRONAS (UTP). All rights reserved by ETP Group 19.

**ETP Group 19 · MFB2102 · January 2026**

*Achieving SDG 6 (Clean Water & Sanitation) and SDG 11 (Sustainable Cities & Communities)*

</div>
