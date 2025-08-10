/*
 * EZ Control - Flexible NodeMCU Controller with WebSocket Support
 * Car Control and GPIO Control work simultaneously
 * 
 * Car Control: Uses motor pins (D1-D5) but these can also be controlled via GPIO
 * GPIO Control: Can control ALL pins (D0-D8, A0) - Total 10 pins
 * 
 * Pin Configuration:
 * - D1: Right Motor Forward (IN1) - Also available for GPIO
 * - D2: Right Motor Backward (IN2) - Also available for GPIO
 * - D3: Left Motor Forward (IN3) - Also available for GPIO
 * - D4: Left Motor Backward (IN4) - Also available for GPIO
 * - D5: Motor Speed Control (ENA/ENB) - Also available for GPIO
 * - D0, D6-D8, A0: Available for GPIO control
 * 
 * Total Available Pins: 10 (D0-D8, A0)
 * 
 * Movement Commands:
 * - forward: Both motors forward at full speed
 * - backward: Both motors backward at full speed
 * - left: Right motor forward, left motor backward (turn left)
 * - right: Left motor forward, right motor backward (turn right)
 * - lf: Left-forward diagonal (both motors forward, right motor slower)
 * - rf: Right-forward diagonal (both motors forward, left motor slower)
 * - lb: Left-backward diagonal (both motors backward, right motor slower)
 * - rb: Right-backward diagonal (both motors backward, left motor slower)
 * 
 * Features:
 * - WebSocket real-time communication
 * - HTTP API fallback
 * - Car control via joystick/gyroscope with 8-direction support
 * - GPIO control for ALL 10 pins (including car control pins)
 * - WiFi configuration
 * - OTA updates support
 * 
 *  Author: Pappuraj Bhottacharjee
 *  Email: me@pappuraj.com
 *  Phone: +8801832755401
 */

 #include <ESP8266WiFi.h>
 #include <ESP8266WebServer.h>
 #include <WebSocketsServer.h>
 #include <ArduinoJson.h>
 #include <ESP8266HTTPUpdateServer.h>
 
 // WiFi credentials
 const char* ssid = "IOT";  // Replace with your WiFi name
 const char* password = "11223344";  // Replace with your WiFi password

 // Hotspot configuration
 const char* hotspot_ssid = "Ez Control Device";
 const char* hotspot_password = "11223344";
 const IPAddress hotspot_ip(192, 168, 4, 1);
 const IPAddress hotspot_gateway(192, 168, 4, 1);
 const IPAddress hotspot_subnet(255, 255, 255, 0);

 // WiFi mode flag
 bool isHotspotMode = false;
 
 // Pin definitions - All 10 pins available
 const int RIGHT_MOTOR_FORWARD = D1;   // IN1
 const int RIGHT_MOTOR_BACKWARD = D2;  // IN2
 const int LEFT_MOTOR_FORWARD = D3;    // IN3
 const int LEFT_MOTOR_BACKWARD = D4;   // IN4
 const int MOTOR_SPEED = D5;           // ENA/ENB
 
 // ALL GPIO control pins - Total 10 pins
 const int GPIO_PINS[] = {D0, D1, D2, D3, D4, D5, D6, D7, D8, A0};
 const int TOTAL_GPIO_PIN_COUNT = 10;
 
 // WebSocket server
 WebSocketsServer webSocket = WebSocketsServer(81);
 ESP8266WebServer server(80);
 ESP8266HTTPUpdateServer httpUpdater;
 
 // Car control variables
 bool carEnabled = false;
 int currentSpeed = 0;
 int currentDirection = 0; // 0: stop, 1: forward, 2: backward, 3: left, 4: right, 5: lf, 6: lb, 7: rf, 8: rb
 
 // GPIO control variables - All 10 pins
 bool gpioEnabled = false;
 int gpioStates[TOTAL_GPIO_PIN_COUNT] = {0};
 
 // GPIO pin mapping for easier reference - All 10 pins
 const char* GPIO_PIN_NAMES[] = {"D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7", "D8", "A0"};
 
 void setup() {
   Serial.begin(115200);
   Serial.println("\n\nEZ Control NodeMCU Starting...");
   Serial.println("All 10 pins (D0-D8, A0) available for GPIO control!");
   
   // Initialize ALL GPIO pins as outputs
   for (int i = 0; i < TOTAL_GPIO_PIN_COUNT; i++) {
     pinMode(GPIO_PINS[i], OUTPUT);
     digitalWrite(GPIO_PINS[i], LOW);
     gpioStates[i] = 0;
   }
   
   // Stop motors initially
   stopMotors();
   
   // Try to connect to WiFi first
   Serial.println("Attempting to connect to WiFi...");
   WiFi.begin(ssid, password);
   
   // Wait for WiFi connection with timeout
   int wifiTimeout = 0;
   while (WiFi.status() != WL_CONNECTED && wifiTimeout < 20) {
     delay(500);
     Serial.print(".");
     wifiTimeout++;
   }
   
   if (WiFi.status() == WL_CONNECTED) {
     // WiFi connection successful
     isHotspotMode = false;
     Serial.println();
     Serial.println("WiFi connected successfully!");
     Serial.print("IP Address: ");
     Serial.println(WiFi.localIP());
     Serial.print("Gateway IP: ");
     Serial.println(WiFi.gatewayIP());
     Serial.print("Subnet Mask: ");
     Serial.println(WiFi.subnetMask());
     Serial.print("DNS IP: ");
     Serial.println(WiFi.dnsIP());
   } else {
     // WiFi connection failed, start hotspot mode
     isHotspotMode = true;
     Serial.println();
     Serial.println("WiFi connection failed. Starting hotspot mode...");
     
     // Configure hotspot
     WiFi.mode(WIFI_AP);
     WiFi.softAPConfig(hotspot_ip, hotspot_gateway, hotspot_subnet);
     WiFi.softAP(hotspot_ssid, hotspot_password);
     
     Serial.println("Hotspot started successfully!");
     Serial.print("Hotspot SSID: ");
     Serial.println(hotspot_ssid);
     Serial.print("Hotspot Password: ");
     Serial.println(hotspot_password);
     Serial.print("Hotspot IP: ");
     Serial.println(hotspot_ip);
     Serial.println("Connect your phone to this hotspot to control the device.");
   }
   
   // Setup WebSocket
   webSocket.begin();
   webSocket.onEvent(webSocketEvent);
   Serial.println("WebSocket server started on port 81");
   
   // Setup HTTP server
   setupHTTPServer();
   server.begin();
   Serial.println("HTTP server started on port 80");
   
   Serial.println("EZ Control NodeMCU Ready!");
   if (isHotspotMode) {
     Serial.println("Mode: HOTSPOT");
     Serial.println("Connect to WiFi: " + String(hotspot_ssid));
     Serial.println("Password: " + String(hotspot_password));
     Serial.println("Device IP: " + hotspot_ip.toString());
     Serial.println("1. Connect to this hotspot from your phone");
     Serial.println("2. Navigate to: http://" + hotspot_ip.toString() + "/");
     Serial.println("3. Use WebSocket: ws://" + hotspot_ip.toString() + ":81");
   } else {
     Serial.println("Mode: WIFI CLIENT");
     Serial.println("1. Connect to this IP via HTTP: http://" + WiFi.localIP().toString() + "/");
     Serial.println("2. Connect via WebSocket: ws://" + WiFi.localIP().toString() + ":81");
   }
   Serial.println("4. Use the Flutter app to discover this device");
 }
 
 void loop() {
   webSocket.loop();
   server.handleClient();
   
   // Add any background tasks here if needed
   delay(10);
 }
 
 void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
   switch(type) {
     case WStype_DISCONNECTED:
       Serial.printf("[%u] Disconnected!\n", num);
       break;
       
     case WStype_CONNECTED:
       {
         IPAddress ip = webSocket.remoteIP(num);
         Serial.printf("[%u] Connected from %d.%d.%d.%d\n", num, ip[0], ip[1], ip[2], ip[3]);
         
         // Send initial status
         sendStatus(num);
       }
       break;
       
     case WStype_TEXT:
       {
         Serial.printf("[%u] Received: %s\n", num, payload);
         handleWebSocketCommand(payload, length);
       }
       break;
   }
 }
 
 void handleWebSocketCommand(uint8_t * payload, size_t length) {
   String message = String((char*)payload);
   
   // Parse JSON command
   DynamicJsonDocument doc(512);
   DeserializationError error = deserializeJson(doc, message);
   
   if (error) {
     Serial.print("JSON parsing failed: ");
     Serial.println(error.c_str());
     return;
   }
   
   String command = doc["command"];
   
   if (command == "car_control") {
     handleCarControl(doc);
   } else if (command == "gpio_control") {
     handleGPIOControl(doc);
   } else if (command == "gpio_batch") {
     handleGPIOBatch(doc);
   } else if (command == "get_status") {
     // Status will be sent to all connected clients
     broadcastStatus();
   } else {
     Serial.println("Unknown command: " + command);
   }
 }
 
 void handleCarControl(DynamicJsonDocument& doc) {
   if (!doc.containsKey("action")) return;
   
   String action = doc["action"];
   
   if (action == "forward") {
     moveForward();
     currentDirection = 1;
   } else if (action == "backward") {
     moveBackward();
     currentDirection = 2;
   } else if (action == "left") {
     turnLeft();
     currentDirection = 3;
   } else if (action == "right") {
     turnRight();
     currentDirection = 4;
   } else if (action == "lf") {
     moveLeftForward();
     currentDirection = 5;
   } else if (action == "lb") {
     moveLeftBackward();
     currentDirection = 6;
   } else if (action == "rf") {
     moveRightForward();
     currentDirection = 7;
   } else if (action == "rb") {
     moveRightBackward();
     currentDirection = 8;
   } else if (action == "stop") {
     stopMotors();
     currentDirection = 0;
   } else if (action == "speed") {
     if (doc.containsKey("speed")) {
       int speed = doc["speed"];
       setMotorSpeed(speed);
       currentSpeed = speed;
     }
   }
   
   // Broadcast status to all clients
   broadcastStatus();
 }
 
 void handleGPIOControl(DynamicJsonDocument& doc) {
   if (!doc.containsKey("pin") || !doc.containsKey("state")) return;
   
   String pinName = doc["pin"];
   int state = doc["state"];
   
   int pinIndex = getPinIndex(pinName);
   if (pinIndex >= 0) {
     digitalWrite(GPIO_PINS[pinIndex], state);
     gpioStates[pinIndex] = state;
     
     // Broadcast status to all clients
     broadcastStatus();
   }
 }
 
 void handleGPIOBatch(DynamicJsonDocument& doc) {
   if (!doc.containsKey("pins")) return;
   
   JsonObject pinsObj = doc["pins"];
   int updatedCount = 0;
   
   for (JsonPair kv : pinsObj) {
     String pinName = kv.key().c_str();
     int state = kv.value().as<int>();
     int pinIndex = getPinIndex(pinName);
     
     if (pinIndex >= 0) {
       digitalWrite(GPIO_PINS[pinIndex], state);
       gpioStates[pinIndex] = state;
       updatedCount++;
     }
   }
   
   Serial.printf("Batch updated %d pins\n", updatedCount);
   
   // Broadcast status to all clients
   broadcastStatus();
 }
 
 void moveForward() {
   digitalWrite(RIGHT_MOTOR_FORWARD, HIGH);
   digitalWrite(RIGHT_MOTOR_BACKWARD, LOW);
   digitalWrite(LEFT_MOTOR_FORWARD, HIGH);
   digitalWrite(LEFT_MOTOR_BACKWARD, LOW);
   restoreFullMotorSpeed(); // Ensure full speed for straight movement
 }
 
 void moveBackward() {
   digitalWrite(RIGHT_MOTOR_FORWARD, LOW);
   digitalWrite(RIGHT_MOTOR_BACKWARD, HIGH);
   digitalWrite(LEFT_MOTOR_FORWARD, LOW);
   digitalWrite(LEFT_MOTOR_BACKWARD, HIGH);
   restoreFullMotorSpeed(); // Ensure full speed for straight movement
 }
 
 void turnLeft() {
   digitalWrite(RIGHT_MOTOR_FORWARD, HIGH);
   digitalWrite(RIGHT_MOTOR_BACKWARD, LOW);
   digitalWrite(LEFT_MOTOR_FORWARD, LOW);
   digitalWrite(LEFT_MOTOR_BACKWARD, HIGH);
   restoreFullMotorSpeed(); // Ensure full speed for turning
 }
 
 void turnRight() {
   digitalWrite(RIGHT_MOTOR_FORWARD, LOW);
   digitalWrite(RIGHT_MOTOR_BACKWARD, HIGH);
   digitalWrite(LEFT_MOTOR_FORWARD, HIGH);
   digitalWrite(LEFT_MOTOR_BACKWARD, LOW);
   restoreFullMotorSpeed(); // Ensure full speed for turning
 }
 
 // Diagonal movement functions
 void moveLeftForward() {
   // Left motor forward, right motor forward (but slower for turning effect)
   digitalWrite(RIGHT_MOTOR_FORWARD, HIGH);
   digitalWrite(RIGHT_MOTOR_BACKWARD, LOW);
   digitalWrite(LEFT_MOTOR_FORWARD, HIGH);
   digitalWrite(LEFT_MOTOR_BACKWARD, LOW);
   // Reduce right motor speed for turning effect
   analogWrite(MOTOR_SPEED, currentSpeed * 0.7); // Right motor slower
 }
 
 void moveLeftBackward() {
   // Left motor backward, right motor backward (but slower for turning effect)
   digitalWrite(RIGHT_MOTOR_FORWARD, LOW);
   digitalWrite(RIGHT_MOTOR_BACKWARD, HIGH);
   digitalWrite(LEFT_MOTOR_FORWARD, LOW);
   digitalWrite(LEFT_MOTOR_BACKWARD, HIGH);
   // Reduce right motor speed for turning effect
   analogWrite(MOTOR_SPEED, currentSpeed * 0.7); // Right motor slower
 }
 
 void moveRightForward() {
   // Right motor forward, left motor forward (but slower for turning effect)
   digitalWrite(RIGHT_MOTOR_FORWARD, HIGH);
   digitalWrite(RIGHT_MOTOR_BACKWARD, LOW);
   digitalWrite(LEFT_MOTOR_FORWARD, HIGH);
   digitalWrite(LEFT_MOTOR_BACKWARD, LOW);
   // Reduce left motor speed for turning effect
   analogWrite(MOTOR_SPEED, currentSpeed * 0.7); // Left motor slower
 }
 
 void moveRightBackward() {
   // Right motor backward, left motor backward (but slower for turning effect)
   digitalWrite(RIGHT_MOTOR_FORWARD, LOW);
   digitalWrite(RIGHT_MOTOR_BACKWARD, HIGH);
   digitalWrite(LEFT_MOTOR_FORWARD, LOW);
   digitalWrite(LEFT_MOTOR_BACKWARD, HIGH);
   // Reduce left motor speed for turning effect
   analogWrite(MOTOR_SPEED, currentSpeed * 0.7); // Left motor slower
 }
 
 void stopMotors() {
   digitalWrite(RIGHT_MOTOR_FORWARD, LOW);
   digitalWrite(RIGHT_MOTOR_BACKWARD, LOW);
   digitalWrite(LEFT_MOTOR_FORWARD, LOW);
   digitalWrite(LEFT_MOTOR_BACKWARD, LOW);
   restoreFullMotorSpeed(); // Restore full motor speed
 }
 
 void setMotorSpeed(int speed) {
   speed = constrain(speed, 0, 1023);
   currentSpeed = speed;
   analogWrite(MOTOR_SPEED, speed);
 }
 
 void restoreFullMotorSpeed() {
   analogWrite(MOTOR_SPEED, currentSpeed);
 }
 
 void sendStatus(uint8_t num) {
   DynamicJsonDocument doc(512);
   doc["type"] = "status";
   doc["car_enabled"] = carEnabled;
   doc["current_speed"] = currentSpeed;
   doc["current_direction"] = currentDirection;
   doc["wifi_mode"] = isHotspotMode ? "hotspot" : "wifi_client";
   doc["device_ip"] = getCurrentIP();
   
   JsonArray gpioArray = doc.createNestedArray("gpio_states");
   for (int i = 0; i < TOTAL_GPIO_PIN_COUNT; i++) {
     gpioArray.add(gpioStates[i]);
   }
   
   String jsonString;
   serializeJson(doc, jsonString);
   webSocket.sendTXT(num, jsonString);
 }
 
 void broadcastStatus() {
   DynamicJsonDocument doc(512);
   doc["type"] = "status";
   doc["car_enabled"] = carEnabled;
   doc["current_speed"] = currentSpeed;
   doc["current_direction"] = currentDirection;
   doc["wifi_mode"] = isHotspotMode ? "hotspot" : "wifi_client";
   doc["device_ip"] = getCurrentIP();
   
   JsonArray gpioArray = doc.createNestedArray("gpio_states");
   for (int i = 0; i < TOTAL_GPIO_PIN_COUNT; i++) {
     gpioArray.add(gpioStates[i]);
   }
   
   String jsonString;
   serializeJson(doc, jsonString);
   webSocket.broadcastTXT(jsonString);
 }
 
 void setupHTTPServer() {
   // OTA update endpoint
   httpUpdater.setup(&server);
   
   // Status endpoint
   server.on("/status", HTTP_GET, []() {
     DynamicJsonDocument doc(512);
     doc["car_enabled"] = carEnabled;
     doc["current_speed"] = currentSpeed;
     doc["current_direction"] = currentDirection;
     doc["wifi_mode"] = isHotspotMode ? "hotspot" : "wifi_client";
     doc["device_ip"] = getCurrentIP();
     
     JsonArray gpioArray = doc.createNestedArray("gpio_states");
     for (int i = 0; i < TOTAL_GPIO_PIN_COUNT; i++) {
       gpioArray.add(gpioStates[i]);
     }
     
     String jsonString;
     serializeJson(doc, jsonString);
     server.send(200, "application/json", jsonString);
   });
   
   // Car control endpoints (for backward compatibility)
   server.on("/car/forward", HTTP_POST, []() {
     moveForward();
     currentDirection = 1;
     server.send(200, "text/plain", "Moving forward");
   });
   
   server.on("/car/backward", HTTP_POST, []() {
     moveBackward();
     currentDirection = 2;
     server.send(200, "text/plain", "Moving backward");
   });
   
   server.on("/car/left", HTTP_POST, []() {
     turnLeft();
     currentDirection = 3;
     server.send(200, "text/plain", "Turning left");
   });
   
   server.on("/car/right", HTTP_POST, []() {
     turnRight();
     currentDirection = 4;
     server.send(200, "text/plain", "Turning right");
   });
   
   server.on("/car/stop", HTTP_POST, []() {
     stopMotors();
     currentDirection = 0;
     server.send(200, "text/plain", "Stopped");
   });
   
   // Diagonal movement endpoints
   server.on("/car/lf", HTTP_POST, []() {
     moveLeftForward();
     currentDirection = 5;
     server.send(200, "text/plain", "Moving left-forward");
   });
   
   server.on("/car/lb", HTTP_POST, []() {
     moveLeftBackward();
     currentDirection = 6;
     server.send(200, "text/plain", "Moving left-backward");
   });
   
   server.on("/car/rf", HTTP_POST, []() {
     moveRightForward();
     currentDirection = 7;
     server.send(200, "text/plain", "Moving right-forward");
   });
   
   server.on("/car/rb", HTTP_POST, []() {
     moveRightBackward();
     currentDirection = 8;
     server.send(200, "text/plain", "Moving right-backward");
   });
   
   server.on("/car/speed", HTTP_POST, []() {
     if (server.hasArg("speed")) {
       int speed = server.arg("speed").toInt();
       setMotorSpeed(speed);
       currentSpeed = speed;
       server.send(200, "text/plain", "Speed set to " + String(speed));
     } else {
       server.send(400, "text/plain", "Speed parameter required");
     }
   });
   
   // GPIO control endpoints - Fixed implementation
   server.on("/pin", HTTP_POST, []() {
     if (server.hasArg("pin") && server.hasArg("state")) {
       String pinName = server.arg("pin");
       int state = server.arg("state").toInt();
       
       int pinIndex = getPinIndex(pinName);
       if (pinIndex >= 0) {
         digitalWrite(GPIO_PINS[pinIndex], state);
         gpioStates[pinIndex] = state;
         server.send(200, "text/plain", "Pin " + pinName + " set to " + String(state));
       } else {
         server.send(400, "text/plain", "Invalid pin: " + pinName);
       }
     } else {
       server.send(400, "text/plain", "Pin and state parameters required");
     }
   });
   
   // Get all pin states
   server.on("/pins", HTTP_GET, []() {
     DynamicJsonDocument doc(512);
     JsonObject pinsObj = doc.createNestedObject("pins");
     
     for (int i = 0; i < TOTAL_GPIO_PIN_COUNT; i++) {
       pinsObj[GPIO_PIN_NAMES[i]] = gpioStates[i];
     }
     
     String jsonString;
     serializeJson(doc, jsonString);
     server.send(200, "application/json", jsonString);
   });
   
   // Get individual pin state
   server.on("/pin", HTTP_GET, []() {
     if (server.hasArg("pin")) {
       String pinName = server.arg("pin");
       int pinIndex = getPinIndex(pinName);
       
       if (pinIndex >= 0) {
         DynamicJsonDocument doc(256);
         doc["pin"] = pinName;
         doc["state"] = gpioStates[pinIndex];
         doc["gpio_number"] = getGPIONumber(pinName);
         
         String jsonString;
         serializeJson(doc, jsonString);
         server.send(200, "application/json", jsonString);
       } else {
         server.send(400, "text/plain", "Invalid pin: " + pinName);
       }
     } else {
       server.send(400, "text/plain", "Pin parameter required");
     }
   });
   
   // Set multiple pins at once
   server.on("/pins/batch", HTTP_POST, []() {
     if (server.hasArg("plain")) {
       String body = server.arg("plain");
       DynamicJsonDocument doc(1024);
       DeserializationError error = deserializeJson(doc, body);
       
       if (error) {
         server.send(400, "text/plain", "Invalid JSON");
         return;
       }
       
       if (doc.containsKey("pins")) {
         JsonObject pinsObj = doc["pins"];
         int updatedCount = 0;
         
         for (JsonPair kv : pinsObj) {
           String pinName = kv.key().c_str();
           int state = kv.value().as<int>();
           int pinIndex = getPinIndex(pinName);
           
           if (pinIndex >= 0) {
             digitalWrite(GPIO_PINS[pinIndex], state);
             gpioStates[pinIndex] = state;
             updatedCount++;
           }
         }
         
         server.send(200, "text/plain", "Updated " + String(updatedCount) + " pins");
       } else {
         server.send(400, "text/plain", "Pins object required");
       }
     } else {
       server.send(400, "text/plain", "JSON body required");
     }
   });
   
   // Reset all pins to LOW
   server.on("/reset", HTTP_POST, []() {
     for (int i = 0; i < TOTAL_GPIO_PIN_COUNT; i++) {
       digitalWrite(GPIO_PINS[i], LOW);
       gpioStates[i] = 0;
     }
     server.send(200, "text/plain", "All GPIO pins reset to LOW");
   });
   
   // Toggle pin state
   server.on("/toggle", HTTP_POST, []() {
     if (server.hasArg("pin")) {
       String pinName = server.arg("pin");
       int pinIndex = getPinIndex(pinName);
       
       if (pinIndex >= 0) {
         int newState = !gpioStates[pinIndex];
         digitalWrite(GPIO_PINS[pinIndex], newState);
         gpioStates[pinIndex] = newState;
         server.send(200, "text/plain", "Pin " + pinName + " toggled to " + String(newState));
       } else {
         server.send(400, "text/plain", "Invalid pin: " + pinName);
       }
     } else {
       server.send(400, "text/plain", "Pin parameter required");
     }
   });
   
   // Legacy GPIO endpoint (kept for backward compatibility)
   server.on("/gpio", HTTP_POST, []() {
     if (server.hasArg("pin") && server.hasArg("state")) {
       int pinIndex = server.arg("pin").toInt();
       int state = server.arg("state").toInt();
       
       if (pinIndex >= 0 && pinIndex < TOTAL_GPIO_PIN_COUNT) {
         digitalWrite(GPIO_PINS[pinIndex], state);
         gpioStates[pinIndex] = state;
         server.send(200, "text/plain", "GPIO " + String(pinIndex) + " set to " + String(state));
       } else {
         server.send(400, "text/plain", "Invalid pin index");
       }
     } else {
       server.send(400, "text/plain", "Pin and state parameters required");
     }
   });
   
   // Discovery endpoint for Flutter app
   server.on("/discovery", HTTP_GET, []() {
     DynamicJsonDocument doc(512);
     doc["device"] = "nodemcu";
     doc["name"] = "EZ Control NodeMCU";
     doc["version"] = "1.0.0";
     
     if (isHotspotMode) {
       doc["ip"] = hotspot_ip.toString();
       doc["mode"] = "hotspot";
       doc["ssid"] = hotspot_ssid;
       doc["status"] = "hotspot_active";
     } else {
       doc["ip"] = WiFi.localIP().toString();
       doc["mode"] = "wifi_client";
       doc["ssid"] = WiFi.SSID();
       doc["status"] = "connected";
     }
     
     doc["mac"] = WiFi.macAddress();
     doc["rssi"] = WiFi.RSSI();
     
     JsonArray pinsArray = doc.createNestedArray("availablePins");
     // All 10 pins available for GPIO control
     pinsArray.add("D0"); pinsArray.add("D1"); pinsArray.add("D2"); 
     pinsArray.add("D3"); pinsArray.add("D4"); pinsArray.add("D5");
     pinsArray.add("D6"); pinsArray.add("D7"); pinsArray.add("D8");
     pinsArray.add("A0");
     
     // Pin descriptions
     JsonObject pinDescriptions = doc.createNestedObject("pinDescriptions");
     pinDescriptions["D0"] = "GPIO16 - Available for GPIO control";
     pinDescriptions["D1"] = "GPIO5 - Right Motor Forward (IN1) + GPIO control";
     pinDescriptions["D2"] = "GPIO4 - Right Motor Backward (IN2) + GPIO control";
     pinDescriptions["D3"] = "GPIO0 - Left Motor Forward (IN3) + GPIO control";
     pinDescriptions["D4"] = "GPIO2 - Left Motor Backward (IN4) + GPIO control";
     pinDescriptions["D5"] = "GPIO14 - Motor Speed Control (ENA/ENB) + GPIO control";
     pinDescriptions["D6"] = "GPIO12 - Available for GPIO control";
     pinDescriptions["D7"] = "GPIO13 - Available for GPIO control";
     pinDescriptions["D8"] = "GPIO15 - Available for GPIO control";
     pinDescriptions["A0"] = "Analog Input - Available for GPIO control";
     
     String jsonString;
     serializeJson(doc, jsonString);
     server.send(200, "application/json", jsonString);
   });
   
   // Root endpoint
   server.on("/", HTTP_GET, []() {
     String html = "<html><head><title>EZ Control NodeMCU</title></head><body>";
     html += "<h1>EZ Control NodeMCU</h1>";
     html += "<h2>All 10 Pins Available for GPIO Control!</h2>";
     html += "<p><strong>Available Pins:</strong> D0, D1, D2, D3, D4, D5, D6, D7, D8, A0</p>";
     
     if (isHotspotMode) {
       html += "<h3>🔴 HOTSPOT MODE</h3>";
       html += "<p><strong>Hotspot SSID:</strong> " + String(hotspot_ssid) + "</p>";
       html += "<p><strong>Hotspot Password:</strong> " + String(hotspot_password) + "</p>";
       html += "<p><strong>Device IP:</strong> " + hotspot_ip.toString() + "</p>";
       html += "<p><em>WiFi connection failed. Device is running as a hotspot.</em></p>";
     } else {
       html += "<h3>🟢 WIFI CLIENT MODE</h3>";
       html += "<p><strong>Connected to:</strong> " + WiFi.SSID() + "</p>";
       html += "<p><strong>Device IP:</strong> " + WiFi.localIP().toString() + "</p>";
       html += "<p><em>Successfully connected to WiFi network.</em></p>";
     }
     
     html += "<p>WebSocket server running on port 81</p>";
     html += "<p>HTTP API available on port 80</p>";
     html += "<p>Use WebSocket for real-time control</p>";
     html += "<p><a href='/discovery'>Discovery Info</a></p>";
     html += "<p><a href='/status'>Status</a></p>";
     html += "<p><a href='/pins'>All GPIO Pins</a></p>";
     html += "<p><strong>Note:</strong> Pins D1-D5 can be used for both car control AND GPIO control!</p>";
     html += "</body></html>";
     server.send(200, "text/html", html);
   });
 }
 
 // Helper function to get pin index from pin name
 int getPinIndex(String pinName) {
   for (int i = 0; i < TOTAL_GPIO_PIN_COUNT; i++) {
     if (pinName == GPIO_PIN_NAMES[i]) {
       return i;
     }
   }
   return -1; // Pin not found
 }
 
 // Helper function to get GPIO number from pin name
 int getGPIONumber(String pinName) {
   if (pinName == "D0") return 16;
   if (pinName == "D1") return 5;
   if (pinName == "D2") return 4;
   if (pinName == "D3") return 0;
   if (pinName == "D4") return 2;
   if (pinName == "D5") return 14;
   if (pinName == "D6") return 12;
   if (pinName == "D7") return 13;
   if (pinName == "D8") return 15;
   if (pinName == "A0") return 17; // A0 is analog input
   return -1; // Pin not found
 }

 // Helper function to get current IP address based on mode
 String getCurrentIP() {
   if (isHotspotMode) {
     return hotspot_ip.toString();
   } else {
     return WiFi.localIP().toString();
   }
 }
 