# 🚗 EZ Control - NodeMCU Car Control System

<div align="center">

![EZ Control Logo](https://img.shields.io/badge/EZ%20Control-Car%20System-blue?style=for-the-badge&logo=arduino)
![Version](https://img.shields.io/badge/Version-5.0.0-green?style=for-the-badge)
![License](https://img.shields.io/badge/License-MIT-yellow?style=for-the-badge)

**Complete NodeMCU-based load & car control system with 8-direction movement, WiFi connectivity, and mobile app integration**

[🌐 Live Demo](https://ezcontrol.pappuraj.com) • [📱 Mobile App](#mobile-app) • [🔧 Setup Guide](#setup-guide)

</div>

---

## 📋 Table of Contents

- [🎯 Overview](#-overview)
- [🚀 Features](#-features)
- [📦 Components Required](#-components-required)
- [🔧 Setup Guide](#-setup-guide)
- [🔌 Circuit Diagram](#-circuit-diagram)
- [💻 Code Upload Process](#-code-upload-process)
- [📱 Mobile App & Website](#-mobile-app--website)
- [🎮 Usage Instructions](#-usage-instructions)
- [🔍 API Documentation](#-api-documentation)
- [🛠️ Troubleshooting](#️-troubleshooting)

---

## 🎯 Overview

EZ Control is a comprehensive car control system that transforms your NodeMCU into a powerful WiFi-enabled car controller. The system supports 8-direction movement, real-time speed control, and seamless integration with both mobile apps and web interfaces.

### 🌟 Key Highlights

- **8-Direction Control**: Forward, Backward, Left, Right, and 4 diagonal movements
- **WiFi Connectivity**: Automatic WiFi connection with Access Point fallback
- **Real-time Control**: Instant response with visual feedback
- **Mobile Integration**: Works with dedicated Flutter app and web interface
- **Speed Control**: PWM-based speed adjustment (0-1023)
- **Status Monitoring**: Real-time car status and pin state monitoring

---

## 🚀 Features

### 🎮 Car Control Features
- ✅ **8-Direction Movement**: Complete directional control
- ✅ **Speed Control**: Real-time PWM speed adjustment
- ✅ **Command Timeout**: Automatic stop after inactivity
- ✅ **Visual Feedback**: LED status indicators
- ✅ **Error Recovery**: Robust error handling and logging

### 🌐 Connectivity Features
- ✅ **WiFi Auto-Connect**: Automatic network connection
- ✅ **Access Point Mode**: Fallback hotspot for direct connection
- ✅ **mDNS Support**: Network discovery enhancement
- ✅ **RESTful API**: JSON-based communication
- ✅ **Web Interface**: Built-in status page

### 📱 Integration Features
- ✅ **Mobile App Support**: Flutter app integration
- ✅ **Web App Support**: React web interface
- ✅ **Device Discovery**: Automatic device detection
- ✅ **Cross-Platform**: Works on Android, iOS, and Web

---

## 📦 Components Required

### 🔧 Hardware Components

| Component | Quantity | Description | Price Range |
|-----------|----------|-------------|-------------|
| **NodeMCU ESP8266** | 1 | Main controller board | $3-8 |
| **L298N Motor Driver** | 1 | Dual H-bridge motor controller | $2-5 |
| **DC Motors** | 2 | 12V DC motors with wheels | $5-15 |
| **Car Chassis** | 1 | Car body with wheel mounts | $8-20 |
| **12V Battery Pack** | 1 | Power supply (6-12V) | $10-25 |
| **Jumper Wires** | 10+ | Male-to-male and male-to-female | $2-5 |
| **Breadboard** | 1 | For testing (optional) | $3-8 |
| **LED** | 1 | Status indicator (optional) | $0.5-2 |

**Total Estimated Cost: $33-88**

### 💻 Software Requirements

- **Arduino IDE** (2.0+) with ESP8266 board support
- **Required Libraries**:
  - `ESP8266WiFi` (built-in)
  - `ESP8266WebServer` (built-in)
  - `ArduinoJson` (v6.x)
  - `ESP8266mDNS` (built-in)

---

## 🔧 Setup Guide

### Step 1: Hardware Assembly

#### Pin Configuration

| NodeMCU Pin | L298N Pin | Function | Wire Color |
|-------------|-----------|----------|------------|
| **D1 (GPIO5)** | **IN1** | Right Motor Forward | Red |
| **D2 (GPIO4)** | **IN2** | Right Motor Backward | Blue |
| **D3 (GPIO0)** | **IN3** | Left Motor Forward | Green |
| **D4 (GPIO2)** | **IN4** | Left Motor Backward | Yellow |
| **D5 (GPIO14)** | **ENA** | Motor Speed Control | Orange |
| **3.3V** | **VCC (Logic)** | Logic Power | White |
| **GND** | **GND** | Common Ground | Black |

#### Motor Connections

| L298N Pin | Motor Connection | Description |
|-----------|------------------|-------------|
| **OUT1** | Left Motor (+) | Left motor positive |
| **OUT2** | Left Motor (-) | Left motor negative |
| **OUT3** | Right Motor (+) | Right motor positive |
| **OUT4** | Right Motor (-) | Right motor negative |

### Step 2: Power Supply Setup

```
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   12V Battery   │    │   L298N Driver  │    │   NodeMCU       │
│                 │    │                 │    │                 │
│  + ──────────── │───▶│ VCC (Motor)     │    │ 3.3V ──────────▶│
│                 │    │                 │    │                 │
│  - ──────────── │───▶│ GND             │───▶│ GND             │
└─────────────────┘    └─────────────────┘    └─────────────────┘
```

### Step 3: Software Installation

#### 3.1 Install Arduino IDE
1. Download [Arduino IDE 2.0+](https://www.arduino.cc/en/software)
2. Install and launch Arduino IDE

#### 3.2 Add ESP8266 Board Support
1. Open Arduino IDE
2. Go to **File** → **Preferences**
3. Add this URL to "Additional Board Manager URLs":
   ```
   https://arduino.esp8266.com/stable/package_esp8266com_index.json
   ```
4. Go to **Tools** → **Board** → **Boards Manager**
5. Search for "ESP8266" and install "ESP8266 by ESP8266 Community"

#### 3.3 Install Required Libraries
1. Go to **Tools** → **Manage Libraries**
2. Search and install:
   - **ArduinoJson** by Benoit Blanchon (v6.x)

---

## 🔌 Circuit Diagram

```
┌─────────────────────────────────────────────────────────────────┐
│                        EZ Control Car System                    │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  NodeMCU ESP8266                    L298N Motor Driver          │
│  ┌─────────────────┐                ┌─────────────────────────┐ │
│  │                 │                │                         │ │
│  │ D1 ──────────── │────────────────▶│ IN1                    │ │
│  │ D2 ──────────── │────────────────▶│ IN2                    │ │
│  │ D3 ──────────── │────────────────▶│ IN3                    │ │
│  │ D4 ──────────── │────────────────▶│ IN4                    │ │
│  │ D5 ──────────── │────────────────▶│ ENA                    │ │
│  │                 │                │                         │ │
│  │ 3.3V ────────── │────────────────▶│ VCC (Logic)            │ │
│  │ GND ─────────── │────────────────▶│ GND                    │ │
│  └─────────────────┘                └─────────────────────────┘ │
│           │                                   │                 │
│           │                                   ▼                 │
│           │                        ┌─────────────────────────┐ │
│           │                        │     12V Battery Pack    │ │
│           │                        │                         │ │
│           │                        │  + ──────────────────────▶│
│           │                        │  - ──────────────────────▶│
│           │                        └─────────────────────────┘ │
│           │                                   │                 │
│           │                                   ▼                 │
│           │                        ┌─────────────────────────┐ │
│           │                        │      DC Motors          │ │
│           │                        │                         │ │
│           │                        │  Left        Right      │ │
│           │                        │  Motor       Motor      │ │
│           │                        └─────────────────────────┘ │
│           │                                   │                 │
│           │                                   ▼                 │
│           │                        ┌─────────────────────────┐ │
│           │                        │      Car Chassis        │ │
│           │                        │                         │ │
│           │                        │  ┌─────┐    ┌─────┐     │ │
│           │                        │  │     │    │     │     │ │
│           │                        │  │  🚗  │    │  🚗  │     │ │
│           │                        │  │     │    │     │     │ │
│           │                        │  └─────┘    └─────┘     │ │
│           │                        └─────────────────────────┘ │
└─────────────────────────────────────────────────────────────────┘
```

---

## 💻 Code Upload Process

### Step 1: Configure Arduino IDE

1. **Select Board**:
   - Go to **Tools** → **Board** → **ESP8266 Boards** → **NodeMCU 1.0**

2. **Configure Settings**:
   ```
   Upload Speed: 115200
   CPU Frequency: 80 MHz
   Flash Frequency: 40 MHz
   Flash Mode: DIO
   Flash Size: 4MB (FS:2MB OTA:~1019KB)
   Debug port: Disabled
   Debug Level: None
   Reset Method: nodemcu
   Crystal Frequency: 26 MHz
   Port: [Select your COM port]
   ```

### Step 2: Configure WiFi Settings

1. Open `Ez Control NodeMCU Code.ino`
2. Locate the WiFi configuration section:
   ```cpp
   // WiFi Configuration
   const char* ssid = "YOUR_WIFI_NAME";        // Change this
   const char* password = "YOUR_WIFI_PASSWORD"; // Change this
   ```
3. Replace with your WiFi credentials

### Step 3: Upload Code

1. **Connect NodeMCU** to your computer via USB
2. **Select the correct port** in Arduino IDE
3. **Click Upload** (→ button) or press `Ctrl+U`
4. **Wait for upload** to complete
5. **Open Serial Monitor** (115200 baud) to see status

### Step 4: Verify Upload

Check Serial Monitor for:
```
=== EZ Control Flexible Controller ===
Version: 5.0.0
Connecting to WiFi...
WiFi connected!
IP address: 192.168.1.100
mDNS responder started
HTTP server started
```

---

## 📱 Mobile App & Website

### 🌐 Web Application

**Live Demo**: [https://ezcontrol.pappuraj.com](https://ezcontrol.pappuraj.com)

#### Features:
- 🎮 **8-Direction Control**: Joystick and button controls
- 🔍 **Auto Discovery**: Automatic device scanning
- 📊 **Real-time Status**: Live car status monitoring
- 🎨 **Modern UI**: Beautiful responsive design
- 📱 **Mobile Friendly**: Works on all devices

#### How to Connect:
1. **Open the website**: [https://ezcontrol.pappuraj.com](https://ezcontrol.pappuraj.com)
2. **Wait for auto-scan** or click "Start Scan"
3. **Click on your device** in the discovered list
4. **Start controlling** your car!

### 📱 Mobile App (Flutter)

#### Download Instructions:
1. **Android**: Available on Google Play Store
2. **iOS**: Available on App Store
3. **Direct Download**: [Download APK](link-to-apk)

#### App Features:
- 🎮 **Touch Controls**: Intuitive touch interface
- 🔍 **Device Discovery**: Automatic NodeMCU detection
- 📊 **Status Dashboard**: Real-time monitoring
- ⚙️ **Settings**: Customizable controls
- 🌙 **Dark Mode**: Eye-friendly interface

#### Connection Process:
1. **Install the app** on your device
2. **Open the app** and grant permissions
3. **Wait for device discovery** or scan manually
4. **Tap on your NodeMCU device**
5. **Start controlling** your car!

---

## 🎮 Usage Instructions

### Basic Controls

#### 8-Direction Movement
| Direction | Left Motor | Right Motor | Description |
|-----------|------------|-------------|-------------|
| **Forward** | Forward | Forward | Straight forward |
| **Backward** | Backward | Backward | Straight backward |
| **Left** | Stop | Forward | Turn left |
| **Right** | Forward | Stop | Turn right |
| **Left-Forward** | Forward | Forward | Diagonal left-forward |
| **Right-Forward** | Forward | Forward | Diagonal right-forward |
| **Left-Backward** | Backward | Backward | Diagonal left-backward |
| **Right-Backward** | Backward | Backward | Diagonal right-backward |

#### Speed Control
- **Range**: 0-1023 (PWM values)
- **Default**: 512 (50% speed)
- **Presets**: 25%, 50%, 75%, 100%

### Control Methods

#### 1. Web Interface
- **Joystick Mode**: Drag joystick for direction
- **Button Mode**: Click and hold buttons
- **Speed Slider**: Adjust speed in real-time

#### 2. Mobile App
- **Touch Joystick**: Intuitive touch control
- **Gesture Controls**: Swipe for direction
- **Voice Commands**: Voice control (future)

#### 3. API Commands

For instance the IP of your NodeMCU is 192.168.1.100
```bash
# Basic movements
curl -X POST http://192.168.1.100/car_forward
curl -X POST http://192.168.1.100/car_backward
curl -X POST http://192.168.1.100/car_left
curl -X POST http://192.168.1.100/car_right
curl -X POST http://192.168.1.100/car_stop

# Speed control
curl -X POST "http://192.168.1.100/car_speed?speed=512"

# Diagonal movements
curl -X POST http://192.168.1.100/car_left_forward
curl -X POST http://192.168.1.100/car_right_forward
curl -X POST http://192.168.1.100/car_left_backward
curl -X POST http://192.168.1.100/car_right_backward
```

---

## 🔍 API Documentation

### Car Control Endpoints

| Endpoint | Method | Description | Parameters |
|----------|--------|-------------|------------|
| `/car_forward` | POST | Move forward | None |
| `/car_backward` | POST | Move backward | None |
| `/car_left` | POST | Turn left | None |
| `/car_right` | POST | Turn right | None |
| `/car_stop` | POST | Stop car | None |
| `/car_speed` | POST | Set speed | `speed` (0-1023) |
| `/car_left_forward` | POST | Left-forward diagonal | None |
| `/car_right_forward` | POST | Right-forward diagonal | None |
| `/car_left_backward` | POST | Left-backward diagonal | None |
| `/car_right_backward` | POST | Right-backward diagonal | None |

### Status Endpoints

| Endpoint | Method | Description | Response |
|----------|--------|-------------|----------|
| `/status` | GET | Get car status | JSON |
| `/discover` | GET | Device discovery | JSON |
| `/` | GET | Web interface | HTML |

### Status Response Format
```json
{
  "device": {
    "name": "EZ Control Device",
    "version": "5.0.0",
    "mac": "AA:BB:CC:DD:EE:FF",
    "ip": "192.168.1.100"
  },
  "car": {
    "direction": "STOP",
    "speed": 512,
    "isMoving": false,
    "lastCommand": "2024-08-07T10:30:00Z"
  },
  "wifi": {
    "ssid": "YourWiFi",
    "signal": -45,
    "isConnected": true
  }
}
```

---

## 🛠️ Troubleshooting

### Common Issues & Solutions

#### 1. Motors Not Moving
**Symptoms**: Car doesn't respond to commands
**Solutions**:
- ✅ Check 12V power supply to L298N
- ✅ Verify motor wire connections
- ✅ Test motors individually with 9V battery
- ✅ Check if motors are properly mounted

#### 2. WiFi Connection Failed
**Symptoms**: Device shows "Not Connected"
**Solutions**:
- ✅ Verify WiFi SSID and password
- ✅ Check WiFi signal strength
- ✅ Ensure NodeMCU is within range
- ✅ Try Access Point mode as fallback

#### 3. Wrong Movement Direction
**Symptoms**: Car moves opposite to commands
**Solutions**:
- ✅ Swap motor wires (OUT1/OUT2 or OUT3/OUT4)
- ✅ Check motor mounting direction
- ✅ Verify pin connections match diagram

#### 4. Speed Control Issues
**Symptoms**: Speed doesn't change or motors stutter
**Solutions**:
- ✅ Check D5 (PWM) connection to ENA
- ✅ Verify motor driver ENA connection
- ✅ Test with different speed values
- ✅ Check power supply stability

#### 5. Device Not Found
**Symptoms**: App/website can't discover device
**Solutions**:
- ✅ Ensure devices are on same network
- ✅ Check firewall settings
- ✅ Try manual IP entry
- ✅ Verify mDNS is working

### Testing Steps

#### 1. Hardware Test
```cpp
// Upload motor_test.ino first
void setup() {
  // Test each motor individually
  digitalWrite(D1, HIGH); delay(1000); digitalWrite(D1, LOW);
  digitalWrite(D2, HIGH); delay(1000); digitalWrite(D2, LOW);
  digitalWrite(D3, HIGH); delay(1000); digitalWrite(D3, LOW);
  digitalWrite(D4, HIGH); delay(1000); digitalWrite(D4, LOW);
}
```

#### 2. WiFi Test
```cpp
// Check Serial Monitor for:
// "WiFi connected!"
// "IP address: 192.168.1.xxx"
```

#### 3. API Test
```bash
# Test basic connectivity
curl http://192.168.1.100/status

# Test car control
curl -X POST http://192.168.1.100/car_forward
```

### LED Status Indicators

| LED State | Meaning | Action |
|-----------|---------|--------|
| **Steady ON** | System ready | Ready to use |
| **Blinking** | Car moving | Normal operation |
| **Fast Blink** | WiFi connecting | Wait for connection |
| **OFF** | System error | Check power/connections |

---

## 👨‍💻 Author

**Pappuraj Bhottacharjee**

- 🌐 Website: [https://pappuraj.com](https://pappuraj.com)
- 📧 Email: me@pappuraj.com
- 💼 LinkedIn: [pappuraj](https://linkedin.com/in/pappuraj)

---

<div align="center">

**⭐ If this project helped you, please give it a star! ⭐**


</div>

---

**Version**: 5.0.0  
**Last Updated**: August 2024  
**Compatible with**: NodeMCU ESP8266, Arduino IDE 2.0+ 
