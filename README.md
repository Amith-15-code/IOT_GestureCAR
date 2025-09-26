# IoT Gesture Control Car

**A simple, open-source IoT car controlled by hand gestures.** This repository contains the hardware wiring, firmware, and server/client code to control a small robotic car using gestures (IMU-based glove or camera-based gesture detection) and communicate over MQTT / WebSocket for remote telemetry.

---

## Table of Contents

* [Overview](#overview)
* [Features](#features)
* [Hardware](#hardware)
* [Wiring](#wiring)
* [Software Components](#software-components)
* [Installation](#installation)
* [Usage](#usage)
* [Gesture Mapping](#gesture-mapping)
* [Calibration & Tuning](#calibration--tuning)
* [Troubleshooting](#troubleshooting)
* [Contributing](#contributing)
* [License](#license)

---

## Overview

This project demonstrates a gesture-controlled IoT car. It supports two common gesture input methods:

1. **IMU-based glove** — small MPU6050 / ADXL335 / BNO055 attached to a hand or glove that sends orientation/acceleration to an ESP32/Arduino, which translates gestures to commands.
2. **Camera-based gestures** — use a Raspberry Pi or PC running OpenCV / MediaPipe to detect hand poses, then send commands over MQTT / WebSocket to the car.

The car itself is controlled by a microcontroller (ESP32 / NodeMCU / Arduino) connected to a motor driver (L298N / TB6612 / DRV8833) and communicates with a broker or server for remote control and telemetry.

---

## Features

* Forward / Backward / Left / Right movement
* Speed control (variable PWM)
* Stop / Emergency stop
* Telemetry: battery voltage, connection status
* Support for IMU-based glove and camera-based gesture detection
* OTA updates (ESP32) and simple web UI (optional)

---

## Hardware

**Recommended parts**

* Chassis with 2 DC motors (or 4-wheel)
* Motor driver (L298N / TB6612 / DRV8833)
* ESP32 (recommended) or ESP8266 / Arduino Uno (with Wi-Fi module)
* MPU6050 or BNO055 (for IMU glove)
* Battery pack (7.4V Li-ion or 6xAA with regulator)
* Optional: HC-SR04 (ultrasonic) for obstacle detection
* Wires, soldering supplies, breadboard, jumper cables

---

## Wiring

> The wiring depends on your motor driver and microcontroller. Below is a generic example for an ESP32 + L298N:

```
ESP32        L298N
------       -----
GND   <----> GND
VIN (5V) <-> VCC (use regulator or separate supply)
D18   <----> IN1
D19   <----> IN2
D21   <----> ENA (PWM for motor A)
D22   <----> IN3
D23   <----> IN4
D5    <----> ENB (PWM for motor B)

MPU6050 I2C:
SDA -> GPIO21
SCL -> GPIO22
VCC -> 3.3V
GND -> GND
```

Adjust pins to your board and update `settings.h` (or `config.h`) in the firmware.

---

## Software Components

* `/firmware/esp32/` — ESP32 firmware (controls motors, reads IMU, connects to Wi‑Fi, publishes/subscribes MQTT topics)
* `/server/` — optional Node.js server or MQTT bridge (for web UI & logging)
* `/client/opencv/` — example Python app for camera-based gesture detection (MediaPipe/OpenCV)
* `/webui/` — optional web interface to manually control and visualize telemetry

---
<img width="868" alt="Screenshot" src="https://drive.google.com/uc?export=view&id=1TSoYFf06lU9OYvBIkJ88B4bGNWEEzglY">
## Installation

### 1. Flash ESP32 firmware

1. Install [PlatformIO](https://platformio.org/) or Arduino IDE with ESP32 support.
2. Edit `firmware/esp32/src/config.h` and set Wi‑Fi SSID/PASSWORD, MQTT broker, and GPIO pins.
3. Build & upload:

PlatformIO example:

```bash
cd firmware/esp32
pio run --target upload
```

Arduino CLI example:

```bash
arduino-cli compile --fqbn esp32:esp32:esp32dev .
arduino-cli upload -p /dev/ttyUSB0 --fqbn esp32:esp32:esp32dev .
```

### 2. Run MQTT broker / server (optional)

You can use a public broker (test.mosquitto.org) or run a local Mosquitto broker:

```bash
# Debian/Ubuntu
sudo apt update && sudo apt install -y mosquitto
sudo systemctl enable --now mosquitto
```

Server example (Node.js) in `/server/`:

```bash
cd server
npm install
node server.js
```

### 3. Camera-based gesture client (optional)

Install Python dependencies (MediaPipe + OpenCV):

```bash
python3 -m venv venv
source venv/bin/activate
pip install opencv-python mediapipe paho-mqtt
python client.py --broker 192.168.1.10
```

---

## Usage

1. Power the car and ensure the microcontroller connects to Wi-Fi and the MQTT broker.
2. Start the gesture client (IMU glove firmware will stream IMU telemetry; camera client will detect hands).
3. Observe MQTT topics (e.g. `car/commands` and `car/telemetry`).
4. Commands format (JSON):

```json
{
  "action": "move",
  "direction": "forward",
  "speed": 150
}
```

The firmware listens on `car/commands` and publishes telemetry to `car/telemetry`.

---

## Gesture Mapping

Default mappings included (change in `firmware/config.h` or in the gesture client):

* **Fist** → Stop
* **Open palm** → Move forward
* **Tilt left** → Turn left
* **Tilt right** → Turn right
* **Thumb up** → Increase speed
* **Thumb down** → Decrease speed

You can customize these mappings in `/client/opencv/gestures.py` or in the IMU gesture recognizer code.

---

## Calibration & Tuning

* IMU: Calibrate gyroscope/accelerometer offsets in the IMU setup routine. Keep your glove steady for a few seconds during calibration.
* Speed/PWM: Tune the maximum PWM value to match your motors and battery to avoid overheating.
* Camera: Adjust detection confidence thresholds in the MediaPipe pipeline.

---

## Troubleshooting

* **Motors don't spin**: Check motor power supply and enable pins. Measure voltage at motor driver.
* **MQTT connection fails**: Verify Wi‑Fi credentials and broker address. Check broker logs.
* **Gesture not recognized**: Recalibrate IMU or increase camera detection confidence.

---

## Contributing

Contributions welcome! Please open issues or PRs. Suggested improvements:

* Add support for Bluetooth control
* Improve gesture recognition (ML-based)
* Add smartphone app interface

---

## Repository Structure

```
/
├─ firmware/
│  └─ esp32/            # ESP32 firmware
├─ client/
│  └─ opencv/           # Camera-based gesture client
├─ server/              # Optional Node.js server & web UI
├─ docs/                # Diagrams & photos
└─ README.md            # <-- you are here
```

---

## License

This project is MIT licensed — see `LICENSE` file.

---

If you'd like, I can:

* Add a ready-to-flash `platformio.ini` and example `config.h` with pin definitions, or
* Generate a simple `client.py` using MediaPipe that publishes MQTT commands, or
* Create wiring diagrams (text-based or SVG) for your exact hardware.

Pick one and I'll add it to t
