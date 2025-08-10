# EZ Control - Advanced NodeMCU Controller Setup Guide

## Overview
This guide covers the setup of the **EZ Control NodeMCU Controller** - a powerful, flexible controller that provides both **car control** and **GPIO control** simultaneously. The system features real-time WebSocket communication, WiFi/hotspot dual-mode operation, and control over all 10 available pins.

## Key Features

### 🚗 **Car Control System**
- **8-direction movement** with diagonal support
- **Speed control** via PWM (0-1023)
- **Real-time control** via WebSocket
- **HTTP API fallback** for compatibility

### 🔌 **GPIO Control System**
- **ALL 10 pins available** (D0-D8, A0)
- **Individual pin control** or batch operations
- **Real-time state monitoring**
- **Toggle and reset functionality**

### 🌐 **Communication & Connectivity**
- **WebSocket server** on port 81 (real-time)
- **HTTP server** on port 80 (fallback)
- **WiFi client mode** (primary)
- **Hotspot mode** (fallback when WiFi fails)
- **OTA updates** support
- **Flutter app discovery**

## Components Required

### Hardware
1. **NodeMCU ESP8266** - Main controller
2. **L298N Motor Driver Module** - Motor control
3. **2x DC Motors** - For car movement
4. **Car Chassis** - With wheels
5. **Battery Pack** - 12V recommended
6. **Jumper Wires** - For connections
7. **Breadboard** - For testing (optional)
8. **LED** - Status indicator (optional)

### Software
1. **Arduino IDE** with ESP8266 board support
2. **Required Libraries**:
   - ESP8266WiFi
   - ESP8266WebServer
   - WebSocketsServer
   - ArduinoJson (version 6.x)
   - ESP8266HTTPUpdateServer

## Pin Configuration

### Motor Control Pins (Also Available for GPIO)
| NodeMCU Pin | GPIO | L298N Pin | Function | GPIO Available |
|-------------|------|-----------|----------|----------------|
| D1          | GPIO5| IN1       | Right Motor Forward | ✅ Yes |
| D2          | GPIO4| IN2       | Right Motor Backward | ✅ Yes |
| D3          | GPIO0| IN3       | Left Motor Forward | ✅ Yes |
| D4          | GPIO2| IN4       | Left Motor Backward | ✅ Yes |
| D5          | GPIO14| ENA/ENB  | Motor Speed Control (PWM) | ✅ Yes |

### Additional GPIO Control Pins
| NodeMCU Pin | GPIO | Function | GPIO Available |
|-------------|------|----------|----------------|
| D0          | GPIO16| General Purpose | ✅ Yes |
| D6          | GPIO12| General Purpose | ✅ Yes |
| D7          | GPIO13| General Purpose | ✅ Yes |
| D8          | GPIO15| General Purpose | ✅ Yes |
| A0          | A0   | Analog Input/Output | ✅ Yes |

**Total Available Pins: 10 (D0-D8, A0)**

## Wiring Diagram

```
NodeMCU ESP8266          L298N Motor Driver
┌─────────────┐         ┌─────────────────┐
│             │         │                 │
│ D1 ──────── │────────▶│ IN1             │
│ D2 ──────── │────────▶│ IN2             │
│ D3 ──────── │────────▶│ IN3             │
│ D4 ──────── │────────▶│ IN4             │
│ D5 ──────── │────────▶│ ENA/ENB         │
│             │         │                 │
│ 3.3V ────── │────────▶│ VCC (Logic)     │
│ GND ─────── │────────▶│ GND             │
└─────────────┘         └─────────────────┘
                                │
                                ▼
                        ┌─────────────────┐
                        │   DC Motors     │
                        │                 │
                        │ Left    Right   │
                        │ Motor   Motor   │
                        └─────────────────┘

Additional GPIO Pins Available:
D0, D6, D7, D8, A0 - Connect to any devices you want to control
```

## Power Supply

### L298N Power
- **VCC (Logic)**: 3.3V from NodeMCU
- **VCC (Motor)**: 12V from battery pack
- **GND**: Common ground

### Battery Configuration
- **Recommended**: 12V battery pack
- **Alternative**: 6x AA batteries (9V)
- **Minimum**: 6V for basic operation

## Software Setup

### 1. Install Required Libraries
In Arduino IDE:
1. Go to **Tools** → **Manage Libraries**
2. Search and install:
   - **ESP8266WiFi** (built-in)
   - **ESP8266WebServer** (built-in)
   - **WebSocketsServer** by Markus Sattler
   - **ArduinoJson** by Benoit Blanchon
   - **ESP8266HTTPUpdateServer** (built-in)

### 2. Configure WiFi Settings
Edit `Ez Control NodeMCU Code.ino`:
```cpp
const char* ssid = "IOT";           // Your WiFi name
const char* password = "11223344";   // Your WiFi password

// Hotspot fallback settings
const char* hotspot_ssid = "Ez Control Device";
const char* hotspot_password = "11223344";
```

### 3. Upload Code
1. Select **NodeMCU 1.0** board in Arduino IDE
2. Set upload speed to **115200**
3. Upload the `Ez Control NodeMCU Code.ino` file

## Operation Modes

### WiFi Client Mode (Primary)
- Connects to your WiFi network
- Device gets IP from your router
- Access via `http://[ROUTER_IP]/`

### Hotspot Mode (Fallback)
- Activates when WiFi connection fails
- Creates WiFi network: "Ez Control Device"
- Password: "11223344"
- Device IP: 192.168.4.1
- Access via `http://192.168.4.1/`

## Communication Protocols

### WebSocket (Recommended - Real-time)
- **Port**: 81
- **URL**: `ws://[DEVICE_IP]:81`
- **Commands**: JSON format
- **Real-time**: Instant response

### HTTP API (Fallback)
- **Port**: 80
- **URL**: `http://[DEVICE_IP]/`
- **Commands**: REST endpoints
- **Response**: Text/JSON

## Car Control Commands

### WebSocket Commands
```json
// Basic movement
{"command": "car_control", "action": "forward"}
{"command": "car_control", "action": "backward"}
{"command": "car_control", "action": "left"}
{"command": "car_control", "action": "right"}
{"command": "car_control", "action": "stop"}

// Diagonal movement
{"command": "car_control", "action": "lf"}    // Left-forward
{"command": "car_control", "action": "rf"}    // Right-forward
{"command": "car_control", "action": "lb"}    // Left-backward
{"command": "car_control", "action": "rb"}    // Right-backward

// Speed control
{"command": "car_control", "action": "speed", "speed": 512}
```

### HTTP Endpoints
```
POST /car/forward
POST /car/backward
POST /car/left
POST /car/right
POST /car/stop
POST /car/lf
POST /car/rf
POST /car/lb
POST /car/rb
POST /car/speed?speed=512
```

## GPIO Control Commands

### WebSocket Commands
```json
// Single pin control
{"command": "gpio_control", "pin": "D6", "state": 1}

// Batch pin control
{"command": "gpio_batch", "pins": {"D0": 1, "D6": 0, "D7": 1}}

// Get status
{"command": "get_status"}
```

### HTTP Endpoints
```
POST /pin?pin=D6&state=1
POST /pins/batch (JSON body)
GET  /pins
GET  /pin?pin=D6
POST /toggle?pin=D6
POST /reset
```

## 8-Direction Car Movement

| Direction | Left Motor | Right Motor | Description |
|-----------|------------|-------------|-------------|
| Forward   | Forward    | Forward     | Straight forward |
| Backward  | Backward   | Backward    | Straight backward |
| Left      | Backward   | Forward     | Turn left |
| Right     | Forward    | Backward    | Turn right |
| LF        | Forward    | Forward     | Left-forward diagonal (right motor slower) |
| RF        | Forward    | Forward     | Right-forward diagonal (left motor slower) |
| LB        | Backward   | Backward    | Left-backward diagonal (right motor slower) |
| RB        | Backward   | Backward    | Right-backward diagonal (left motor slower) |

## Testing

### 1. Serial Monitor
Open Serial Monitor (115200 baud) to see:
- WiFi connection status
- IP address (WiFi or Hotspot)
- WebSocket connections
- Car control commands
- GPIO operations
- Error messages

### 2. Web Interface
1. Note the IP address from Serial Monitor
2. Open browser and go to `http://[IP_ADDRESS]/`
3. You should see the controller status page with pin information

### 3. WebSocket Testing
Use a WebSocket client (like browser console or Postman):
```javascript
// Connect to WebSocket
const ws = new WebSocket('ws://[IP_ADDRESS]:81');

// Send car control command
ws.send('{"command": "car_control", "action": "forward"}');

// Send GPIO command
ws.send('{"command": "gpio_control", "pin": "D6", "state": 1}');
```

### 4. API Testing
Test using curl or Postman:
```bash
# Car control
curl -X POST "http://[IP_ADDRESS]/car/forward"
curl -X POST "http://[IP_ADDRESS]/car/speed?speed=512"

# GPIO control
curl -X POST "http://[IP_ADDRESS]/pin?pin=D6&state=1"
curl -X GET "http://[IP_ADDRESS]/pins"
```

## Flutter App Integration

The controller includes a discovery endpoint for the EZ Control Flutter app:

```
GET /discovery
```

Returns device information including:
- Device type and name
- Current IP address
- WiFi mode (client/hotspot)
- Available pins
- Pin descriptions

## OTA Updates

The controller supports Over-The-Air updates:
- **URL**: `http://[IP_ADDRESS]/update`
- **Method**: POST with firmware file
- **Use**: Update code without physical connection

## Troubleshooting

### Common Issues

1. **WiFi connection failed**
   - Check SSID and password
   - Device will automatically start hotspot mode
   - Connect to "Ez Control Device" hotspot

2. **WebSocket connection failed**
   - Check if port 81 is accessible
   - Try HTTP API as fallback
   - Verify firewall settings

3. **GPIO pins not responding**
   - Check pin connections
   - Verify pin names (D0, D1, D2, etc.)
   - Use `/pins` endpoint to check current states

4. **Car movement issues**
   - Check motor connections
   - Verify L298N power supply
   - Test individual motors via GPIO control

5. **Hotspot not working**
   - Check if WiFi credentials are correct
   - Verify hotspot SSID and password
   - Check device IP (should be 192.168.4.1)

### Status Monitoring
- **WebSocket**: Real-time status updates
- **HTTP**: `/status` endpoint for current state
- **Serial**: Continuous status logging

## Safety Notes

1. **Always disconnect power** before making wiring changes
2. **Use appropriate battery capacity** for your motors
3. **Test on a stable surface** before outdoor use
4. **Keep wires organized** to prevent short circuits
5. **Monitor battery voltage** to prevent over-discharge
6. **GPIO pins can control high-voltage devices** - use appropriate safety measures

## Advanced Features

### GPIO Pin Mapping
- **D0**: GPIO16 - General purpose
- **D1**: GPIO5 - Motor control + GPIO
- **D2**: GPIO4 - Motor control + GPIO
- **D3**: GPIO0 - Motor control + GPIO
- **D4**: GPIO2 - Motor control + GPIO
- **D5**: GPIO14 - Motor control + GPIO
- **D6**: GPIO12 - General purpose
- **D7**: GPIO13 - General purpose
- **D8**: GPIO15 - General purpose
- **A0**: Analog input/output

### Batch Operations
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

### Real-time Status
WebSocket provides continuous status updates:
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

## Version Information
- **Hardware Version**: 4.0.0
- **Software Version**: 4.0.0
- **Compatible App Version**: 4.0.0+
- **Features**: WebSocket, GPIO Control, Hotspot Mode, OTA Updates

## Support
For issues or questions:
1. Check Serial Monitor for error messages
2. Verify all connections match the wiring diagram
3. Test individual components before full assembly
4. Ensure proper power supply and grounding
5. Use WebSocket for real-time control
6. Check device discovery endpoint for troubleshooting

---
**Author**: Pappuraj Bhottacharjee  
**Email**: me@pappuraj.com  
**Phone**: +8801832755401  
**Last Updated**: December 2024 