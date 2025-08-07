# EZ Control - Car Control System Setup Guide

## Overview
This guide will help you set up the EZ Control car system using NodeMCU and L298N motor driver to control a car with 8-direction movement.

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
   - ArduinoJson (version 6.x)
   - ESP8266mDNS

## Pin Configuration

### NodeMCU to L298N Connections
| NodeMCU Pin | L298N Pin | Function |
|-------------|-----------|----------|
| D1 (GPIO5)  | IN1       | Left Motor Forward |
| D2 (GPIO4)  | IN2       | Left Motor Backward |
| D3 (GPIO0)  | IN3       | Right Motor Forward |
| D4 (GPIO2)  | IN4       | Right Motor Backward |
| D5 (GPIO14) | ENA       | Motor Speed Control (PWM) |
| D6 (GPIO12) | -         | Status LED (optional) |

### L298N to Motors
| L298N Pin | Motor Connection |
|-----------|------------------|
| OUT1      | Left Motor (+) |
| OUT2      | Left Motor (-) |
| OUT3      | Right Motor (+) |
| OUT4      | Right Motor (-) |

## Wiring Diagram

```
NodeMCU ESP8266          L298N Motor Driver
┌─────────────┐         ┌─────────────────┐
│             │         │                 │
│ D1 ──────── │────────▶│ IN1             │
│ D2 ──────── │────────▶│ IN2             │
│ D3 ──────── │────────▶│ IN3             │
│ D4 ──────── │────────▶│ IN4             │
│ D5 ──────── │────────▶│ ENA             │
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
   - **ArduinoJson** by Benoit Blanchon
   - **ESP8266mDNS** (built-in)

### 2. Configure WiFi Settings
Edit `car_controller_final.ino`:
```cpp
const char* ssid = "YourWiFiSSID";        // Your WiFi name
const char* password = "YourWiFiPassword"; // Your WiFi password
```

### 3. Upload Code
1. Select **NodeMCU 1.0** board in Arduino IDE
2. Set upload speed to **115200**
3. Upload the `car_controller_final.ino` file

## Testing

### 1. Serial Monitor
Open Serial Monitor (115200 baud) to see:
- WiFi connection status
- IP address
- Car control commands
- Error messages

### 2. Web Interface
1. Note the IP address from Serial Monitor
2. Open browser and go to `http://[IP_ADDRESS]/`
3. You should see the car controller status page

### 3. API Testing
Test car control using these endpoints:
```
POST http://[IP_ADDRESS]/car_forward
POST http://[IP_ADDRESS]/car_backward
POST http://[IP_ADDRESS]/car_left
POST http://[IP_ADDRESS]/car_right
POST http://[IP_ADDRESS]/car_stop
POST http://[IP_ADDRESS]/car_speed?speed=512
```

## 8-Direction Control

The system supports 8 directions:

| Direction | Left Motor | Right Motor | Description |
|-----------|------------|-------------|-------------|
| Forward   | Forward    | Forward     | Straight forward |
| Backward  | Backward   | Backward    | Straight backward |
| Left      | Stop       | Forward     | Turn left |
| Right     | Forward    | Stop        | Turn right |
| LF        | Forward    | Forward     | Left-forward diagonal |
| RF        | Forward    | Forward     | Right-forward diagonal |
| LB        | Backward   | Backward    | Left-backward diagonal |
| RB        | Backward   | Backward    | Right-backward diagonal |

## Troubleshooting

### Common Issues

1. **Motors not moving**
   - Check power supply to L298N
   - Verify motor connections
   - Check if motors are properly mounted

2. **WiFi connection failed**
   - Verify SSID and password
   - Check WiFi signal strength
   - NodeMCU will start in AP mode if WiFi fails

3. **Car moving in wrong direction**
   - Swap motor wires
   - Check pin connections
   - Verify motor mounting direction

4. **Speed control not working**
   - Check D5 (PWM) connection
   - Verify motor driver ENA connection
   - Test with different speed values

### LED Status Indicators
- **Steady ON**: System ready
- **Blinking**: Car is moving
- **OFF**: System error or not powered

## Safety Notes

1. **Always disconnect power** before making wiring changes
2. **Use appropriate battery capacity** for your motors
3. **Test on a stable surface** before outdoor use
4. **Keep wires organized** to prevent short circuits
5. **Monitor battery voltage** to prevent over-discharge

## Integration with Flutter App

The NodeMCU is designed to work seamlessly with the EZ Control Flutter app:

1. **Device Discovery**: App will automatically find the car controller
2. **8-Direction Control**: Full support for all movement directions
3. **Speed Control**: Real-time speed adjustment
4. **Status Monitoring**: Live car status updates
5. **Error Handling**: Automatic reconnection and error recovery

## Version Information
- **Hardware Version**: 3.1.0
- **Software Version**: 3.1.0
- **Compatible App Version**: 3.1.0+

## Support
For issues or questions:
1. Check Serial Monitor for error messages
2. Verify all connections match the wiring diagram
3. Test individual components before full assembly
4. Ensure proper power supply and grounding

---
**Author**: Pappuraj Bhottacharjee  
**Last Updated**: August 2024 