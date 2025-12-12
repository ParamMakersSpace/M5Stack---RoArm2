# M5Stack IMU Control for RoArm2 Robotic Arm

Control the **RoArm2 robotic arm** using the **M5Stack Core IMU** by mapping hand orientation and motion to robotic arm movement in real time.

This project demonstrates **intuitive 6-DOF-style control**, where:
- Tilt controls X, Y, and Z motion
- Rotation controls end-effector orientation
- Communication is done wirelessly over WiFi using HTTP commands

---

## 🧠 Project Concept

The **M5Stack Core** acts as a motion controller:
- Accelerometer → Cartesian movement (X, Y, Z)
- Gyroscope → Tool rotation (T axis)

The **RoArm2** runs as a WiFi Access Point, and the M5Stack sends HTTP JSON commands directly to the robot.

---

## 🧩 Hardware Used

- **M5Stack Core (ESP32-based)**
- **RoArm2 Robotic Arm**
- Built-in IMU (accelerometer + gyroscope)
- WiFi (RoArm2 AP mode)

---

## 🛠️ Software & Libraries

- Arduino IDE
- M5Unified Library
- ESP32 Board Package

### Required Libraries
Install these from Arduino Library Manager:
- `M5Unified`
- `WiFi` (ESP32 core)
- `HTTPClient`

---

## 📡 Network Configuration

- **RoArm2 SSID:** `RoArm-M2`
- **Password:** `12345678`
- **Robot IP:** `192.168.4.1`

⚠️ Ensure your RoArm2 is powered ON before starting the M5Stack.

---

## 🎮 Control Mapping

| M5Stack IMU | Robot Motion |
|------------|--------------|
| Tilt X-axis | Y-axis movement |
| Tilt Y-axis | X-axis movement |
| Vertical acceleration | Z-axis movement |
| Gyro Z | Tool rotation (T) |

---

## 🔐 Safety Constraints

To prevent damage, motion is constrained:

```cpp
posX = constrain(posX, 100, 200);
posY = constrain(posY, 100, 200);
posZ = constrain(posZ, 60, 100);
posT = constrain(posT, 0.5, 2.5);
