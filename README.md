# 🚗 EZ Control - Advanced NodeMCU Controller

<div align="center">

![EZ Control Logo](https://img.shields.io/badge/EZ%20Control-Advanced%20Controller-blue?style=for-the-badge&logo=arduino)
![Version](https://img.shields.io/badge/Version-4.0.0-green?style=for-the-badge)
![License](https://img.shields.io/badge/License-MIT-yellow?style=for-the-badge)

**Advanced NodeMCU controller with car control, GPIO control, WebSocket support, and dual-mode WiFi operation**

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

EZ Control is an **advanced NodeMCU controller** that provides both **car control** and **GPIO control** simultaneously. The system features real-time WebSocket communication, WiFi/hotspot dual-mode operation, control over all 10 available pins, and seamless integration with mobile apps and web interfaces.

### 🌟 Key Highlights

- **🚗 Car Control**: 8-direction movement with diagonal support and speed control
- **🔌 GPIO Control**: ALL 10 pins (D0-D8, A0) available for general purpose use
- **🌐 WebSocket Support**: Real-time communication for instant response
- **📡 Dual WiFi Mode**: WiFi client + hotspot fallback
- **🔄 OTA Updates**: Over-the-air firmware updates
- **📱 Flutter Integration**: Built-in device discovery for mobile apps

---

## 🚀 Features

### 🎮 Car Control Features
- ✅ **8-Direction Movement**: Complete directional control including diagonals
- ✅ **Speed Control**: Real-time PWM speed adjustment (0-1023)
- ✅ **Motor Management**: Individual motor control via GPIO
- ✅ **Visual Feedback**: LED status indicators
- ✅ **Error Recovery**: Robust error handling and logging

### 🔌 GPIO Control Features
- ✅ **10 Available Pins**: D0, D1, D2, D3, D4, D5, D6, D7, D8, A0
- ✅ **Individual Control**: Set each pin HIGH/LOW independently
- ✅ **Batch Operations**: Control multiple pins simultaneously
- ✅ **Toggle Function**: Toggle pin states with single command
- ✅ **Reset Function**: Reset all pins to LOW state
- ✅ **Real-time Monitoring**: Live pin state updates

### 🌐 Connectivity Features
- ✅ **WebSocket Server**: Real-time communication on port 81
- ✅ **HTTP API**: RESTful endpoints on port 80
- ✅ **WiFi Auto-Connect**: Automatic network connection
- ✅ **Hotspot Mode**: Fallback network when WiFi fails
- ✅ **OTA Updates**: Firmware updates without physical connection
- ✅ **Device Discovery**: Automatic device detection for Flutter apps

### 📱 Integration Features
- ✅ **Mobile App Support**: Flutter app integration
- ✅ **Web App Support**: React web interface
- ✅ **Device Discovery**: Automatic device detection
- ✅ **Cross-Platform**: Works on Android, iOS, and Web
- ✅ **Real-time Status**: Live updates via WebSocket

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
  - `WebSocketsServer` by Markus Sattler
  - `ArduinoJson` (v6.x)
  - `ESP8266HTTPUpdateServer` (built-in)

---

## 🔧 Setup Guide

### Step 1: Hardware Assembly

#### Pin Configuration

| NodeMCU Pin | GPIO | L298N Pin | Function | GPIO Available |
|-------------|------|-----------|----------|----------------|
| **D1 (GPIO5)** | GPIO5 | **IN1** | Right Motor Forward | ✅ Yes |
| **D2 (GPIO4)** | GPIO4 | **IN2** | Right Motor Backward | ✅ Yes |
| **D3 (GPIO0)** | GPIO0 | **IN3** | Left Motor Forward | ✅ Yes |
| **D4 (GPIO2)** | GPIO2 | **IN4** | Left Motor Backward | ✅ Yes |
| **D5 (GPIO14)** | GPIO14 | **ENA/ENB** | Motor Speed Control | ✅ Yes |

#### Additional GPIO Pins
| NodeMCU Pin | GPIO | Function | GPIO Available |
|-------------|------|----------|----------------|
| **D0** | GPIO16 | General Purpose | ✅ Yes |
| **D6** | GPIO12 | General Purpose | ✅ Yes |
| **D7** | GPIO13 | General Purpose | ✅ Yes |
| **D8** | GPIO15 | General Purpose | ✅ Yes |
| **A0** | A0 | Analog Input/Output | ✅ Yes |

**Total Available Pins: 10 (D0-D8, A0)**

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
   - **WebSocketsServer** by Markus Sattler
   - **ArduinoJson** by Benoit Blanchon (v6.x)

---

## 🔌 Circuit Diagram

```
┌─────────────────────────────────────────────────────────────────┐
│                    EZ Control Advanced Controller               │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  NodeMCU ESP8266                    L298N Motor Driver          │
│  ┌─────────────────┐                ┌─────────────────────────┐ │
│  │                 │                │                         │ │
│  │ D1 ──────────── │────────────────▶│ IN1                    │ │
│  │ D2 ──────────── │────────────────▶│ IN2                    │ │
│  │ D3 ──────────── │────────────────▶│ IN3                    │ │
│  │ D4 ──────────── │────────────────▶│ IN4                    │ │
│  │ D5 ──────────── │────────────────▶│ ENA/ENB                │ │
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
│                                                                 │
│  Additional GPIO Pins Available:                                │
│  D0, D6, D7, D8, A0 - Connect to any devices you want to control │
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
   // WiFi credentials
   const char* ssid = "IOT";           // Replace with your WiFi name
   const char* password = "11223344";   // Replace with your WiFi password

   // Hotspot configuration
   const char* hotspot_ssid = "Ez Control Device";
   const char* hotspot_password = "11223344";
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
EZ Control NodeMCU Starting...
All 10 pins (D0-D8, A0) available for GPIO control!
WiFi connected successfully!
IP Address: 192.168.1.100
WebSocket server started on port 81
HTTP server started on port 80
EZ Control NodeMCU Ready!
```

---

## 📱 Mobile App & Website

### 🌐 Web Application

**Live Demo**: [https://ezcontrol.pappuraj.com](https://ezcontrol.pappuraj.com)

#### Features:
- 🎮 **8-Direction Control**: Joystick and button controls
- 🔌 **GPIO Control**: Control all 10 pins individually
- 🔍 **Auto Discovery**: Automatic device scanning
- 📊 **Real-time Status**: Live updates via WebSocket
- 🎨 **Modern UI**: Beautiful responsive design
- 📱 **Mobile Friendly**: Works on all devices

#### How to Connect:
1. **Open the website**: [https://ezcontrol.pappuraj.com](https://ezcontrol.pappuraj.com)
2. **Wait for auto-scan** or click "Start Scan"
3. **Click on your device** in the discovered list
4. **Start controlling** your car and GPIO pins!

### 📱 Mobile App (Flutter)

#### Download Instructions:
1. **Android**: Available on Google Play Store
2. **iOS**: Available on App Store
3. **Direct Download**: [Download APK](link-to-apk)

#### App Features:
- 🎮 **Touch Controls**: Intuitive touch interface
- 🔌 **GPIO Control**: Control all pins from mobile
- 🔍 **Device Discovery**: Automatic NodeMCU detection
- 📊 **Status Dashboard**: Real-time monitoring
- ⚙️ **Settings**: Customizable controls
- 🌙 **Dark Mode**: Eye-friendly interface

#### Connection Process:
1. **Install the app** on your device
2. **Open the app** and grant permissions
3. **Wait for device discovery** or scan manually
4. **Tap on your NodeMCU device**
5. **Start controlling** your car and GPIO pins!

---

## 🎮 Usage Instructions

### Basic Controls

#### 8-Direction Car Movement
| Direction | Left Motor | Right Motor | Description |
|-----------|------------|-------------|-------------|
| **Forward** | Forward | Forward | Straight forward |
| **Backward** | Backward | Backward | Straight backward |
| **Left** | Backward | Forward | Turn left |
| **Right** | Forward | Backward | Turn right |
| **Left-Forward** | Forward | Forward | Diagonal left-forward (right motor slower) |
| **Right-Forward** | Forward | Forward | Diagonal right-forward (left motor slower) |
| **Left-Backward** | Backward | Backward | Diagonal left-backward (right motor slower) |
| **Right-Backward** | Backward | Backward | Diagonal right-backward (left motor slower) |

#### Speed Control
- **Range**: 0-1023 (PWM values)
- **Default**: 512 (50% speed)
- **Presets**: 25%, 50%, 75%, 100%

### GPIO Control

#### Individual Pin Control
- **Set HIGH**: `{"command": "gpio_control", "pin": "D6", "state": 1}`
- **Set LOW**: `{"command": "gpio_control", "pin": "D6", "state": 0}`

#### Batch Operations
Control multiple pins simultaneously:
```json
{
  "command": "gpio_batch",
  "pins": {
    "D0": 1,
    "D6": 0,
    "D7": 1,
    "A0": 1
  }
}
```

#### Special Functions
- **Toggle**: `{"command": "gpio_control", "pin": "D6", "action": "toggle"}`
- **Reset All**: `{"command": "gpio_control", "action": "reset"}`

### Control Methods

#### 1. WebSocket (Recommended)
- **Port**: 81
- **URL**: `ws://[DEVICE_IP]:81`
- **Real-time**: Instant response
- **JSON Commands**: Structured control

#### 2. HTTP API (Fallback)
- **Port**: 80
- **URL**: `http://[DEVICE_IP]/`
- **REST Endpoints**: Standard HTTP methods
- **Response**: Text/JSON

#### 3. Web Interface
- **Joystick Mode**: Drag joystick for direction
- **Button Mode**: Click and hold buttons
- **GPIO Panel**: Control all 10 pins
- **Speed Slider**: Adjust speed in real-time

#### 4. Mobile App
- **Touch Joystick**: Intuitive touch control
- **GPIO Buttons**: Individual pin control
- **Gesture Controls**: Swipe for direction
- **Voice Commands**: Voice control (future)

---

## 🔍 API Documentation

### WebSocket Commands

#### Car Control
```json
// Basic movement
{"command": "car_control", "action": "forward"}
{"command": "car_control", "action": "backward"}
{"command": "car_control", "action": "left"}
{"command": "car_control", "action": "right"}
{"command": "car_control", "action": "stop"}

// Diagonal movement
{"command": "car_control", "action": "lf"}
{"command": "car_control", "action": "rf"}
{"command": "car_control", "action": "lb"}
{"command": "car_control", "action": "rb"}

// Speed control
{"command": "car_control", "action": "speed", "speed": 512}
```

#### GPIO Control
```json
// Single pin control
{"command": "gpio_control", "pin": "D6", "state": 1}

// Batch pin control
{"command": "gpio_batch", "pins": {"D0": 1, "D6": 0, "D7": 1}}

// Get status
{"command": "get_status"}
```

### HTTP Endpoints

#### Car Control
| Endpoint | Method | Description | Parameters |
|----------|--------|-------------|------------|
| `/car/forward` | POST | Move forward | None |
| `/car/backward` | POST | Move backward | None |
| `/car/left` | POST | Turn left | None |
| `/car/right` | POST | Turn right | None |
| `/car/stop` | POST | Stop car | None |
| `/car/speed` | POST | Set speed | `speed` (0-1023) |
| `/car/lf` | POST | Left-forward diagonal | None |
| `/car/rf` | POST | Right-forward diagonal | None |
| `/car/lb` | POST | Left-backward diagonal | None |
| `/car/rb` | POST | Right-backward diagonal | None |

#### GPIO Control
| Endpoint | Method | Description | Parameters |
|----------|--------|-------------|------------|
| `/pin` | POST | Set pin state | `pin`, `state` |
| `/pins/batch` | POST | Set multiple pins | JSON body |
| `/pins` | GET | Get all pin states | None |
| `/pin` | GET | Get individual pin state | `pin` |
| `/toggle` | POST | Toggle pin state | `pin` |
| `/reset` | POST | Reset all pins to LOW | None |

#### System Endpoints
| Endpoint | Method | Description | Response |
|----------|--------|-------------|----------|
| `/status` | GET | Get system status | JSON |
| `/discovery` | GET | Device discovery info | JSON |
| `/` | GET | Web interface | HTML |
| `/update` | POST | OTA firmware update | Text |

### Status Response Format
```json
{
  "type": "status",
  "car_enabled": true,
  "current_speed": 512,
  "current_direction": 1,
  "wifi_mode": "wifi_client",
  "device_ip": "192.168.1.100",
  "gpio_states": [0, 1, 0, 1, 0, 0, 1, 0, 0, 0]
}
```

### Discovery Response Format
```json
{
  "device": "nodemcu",
  "name": "EZ Control NodeMCU",
  "version": "1.0.0",
  "ip": "192.168.1.100",
  "mode": "wifi_client",
  "ssid": "YourWiFi",
  "status": "connected",
  "mac": "AA:BB:CC:DD:EE:FF",
  "rssi": -45,
  "availablePins": ["D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7", "D8", "A0"]
}
```

---

## 🛠️ Troubleshooting

### Common Issues & Solutions

#### 1. WiFi Connection Failed
**Symptoms**: Device shows "Not Connected"
**Solutions**:
- ✅ Verify WiFi SSID and password
- ✅ Device will automatically start hotspot mode
- ✅ Connect to "Ez Control Device" hotspot
- ✅ Check device IP (should be 192.168.4.1)

#### 2. WebSocket Connection Failed
**Symptoms**: Real-time control not working
**Solutions**:
- ✅ Check if port 81 is accessible
- ✅ Try HTTP API as fallback
- ✅ Verify firewall settings
- ✅ Check device IP address

#### 3. GPIO Pins Not Responding
**Symptoms**: GPIO control not working
**Solutions**:
- ✅ Check pin connections
- ✅ Verify pin names (D0, D1, D2, etc.)
- ✅ Use `/pins` endpoint to check current states
- ✅ Test individual pins via HTTP API

#### 4. Car Movement Issues
**Symptoms**: Car doesn't move or moves incorrectly
**Solutions**:
- ✅ Check motor connections
- ✅ Verify L298N power supply
- ✅ Test individual motors via GPIO control
- ✅ Check motor mounting direction

#### 5. Hotspot Not Working
**Symptoms**: Can't connect to device hotspot
**Solutions**:
- ✅ Check if WiFi credentials are correct
- ✅ Verify hotspot SSID: "Ez Control Device"
- ✅ Password: "11223344"
- ✅ Check device IP (should be 192.168.4.1)

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
// "WiFi connected successfully!"
// "IP Address: 192.168.1.xxx"
// "WebSocket server started on port 81"
```

#### 3. WebSocket Test
```javascript
// Test WebSocket connection
const ws = new WebSocket('ws://192.168.1.100:81');
ws.onopen = () => console.log('Connected!');
ws.send('{"command": "get_status"}');
```

#### 4. API Test
```bash
# Test basic connectivity
curl http://192.168.1.100/status

# Test car control
curl -X POST http://192.168.1.100/car/forward

# Test GPIO control
curl -X POST "http://192.168.1.100/pin?pin=D6&state=1"
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
- 📱 Phone: +8801832755401
- 💼 LinkedIn: [pappuraj](https://linkedin.com/in/pappuraj)

---

<div align="center">

**⭐ If this project helped you, please give it a star! ⭐**


</div>

---

**Version**: 4.0.0  
**Last Updated**: December 2024  
**Compatible with**: NodeMCU ESP8266, Arduino IDE 2.0+  
**Features**: WebSocket, GPIO Control, Hotspot Mode, OTA Updates 
