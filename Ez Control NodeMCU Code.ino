/*
 * EZ Control - Flexible NodeMCU Controller
 * Car Control and GPIO Control work simultaneously
 * 
 * Car Control: Only affects motor pins (D1-D5)
 * GPIO Control: Can control all pins (D0-D8, A0)
 * 
 * Pin Configuration:
 * - D1: Left Motor Forward (IN1)
 * - D2: Left Motor Backward (IN2) 
 * - D3: Right Motor Forward (IN3)
 * - D4: Right Motor Backward (IN4)
 * - D5: Motor Speed Control (ENA/ENB)
 * - D0, D6-D8, A0: Available for GPIO control
 * 
 * Version: 5.0.0
 *
 *  Author: Pappuraj Bhottacharjee
 *  Email: me@pappuraj.com
 *  Website: https://pappuraj.com
 *  Date: 07/08/2025
 *  Version: 3.0.0
 *  Description: This is a complete controller for the EZ Control project.
   
 */

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>

// WiFi Configuration
const char* ssid = "PAPPURAJ"; //Change this to your own ssid
const char* password = "22222222"; //Change this to your own password

//NodeMCU Hotspot Configuration
const char* hotspot_ssid = "EZ_Control_AP"; //Change this to your own hotspot ssid
const char* hotspot_password = "12345678"; //Change this to your own hotspot password

// GPIO Pins (ALL available for GPIO control)
const int GPIO_PINS[] = {D0, D1, D2, D3, D4, D5, D6, D7, D8, A0};
const int NUM_GPIO_PINS = 10;
bool gpioStates[NUM_GPIO_PINS] = {false, false, false, false, false, false, false, false, false, false};

// Car Control Pins
const int LEFT_MOTOR_FORWARD = D1;
const int LEFT_MOTOR_BACKWARD = D2;
const int RIGHT_MOTOR_FORWARD = D3;
const int RIGHT_MOTOR_BACKWARD = D4;
const int MOTOR_SPEED = D5;

// Car Control Variables
int currentSpeed = 512;
String currentDirection = "STOP";
bool isMoving = false;
unsigned long lastCommandTime = 0;

// Web Server and UDP
ESP8266WebServer server(80);
WiFiUDP udp;

// Device Information
const String DEVICE_NAME = "EZ Control Device";
const String DEVICE_VERSION = "5.0.0";
String deviceMAC = "";
bool isHotspotMode = false;

void setup() {
  Serial.begin(115200);
  Serial.println("\n=== EZ Control Flexible Controller ===");
  Serial.println("Version: " + DEVICE_VERSION);
  
  deviceMAC = WiFi.macAddress();
  
  // Initialize ALL GPIO pins
  for (int i = 0; i < NUM_GPIO_PINS; i++) {
    pinMode(GPIO_PINS[i], OUTPUT);
    digitalWrite(GPIO_PINS[i], LOW);
  }
  
  // Stop car initially
  stopCar();
  
  // Connect to WiFi
  connectToWiFi();
  
  // Setup mDNS
  if (MDNS.begin("ezcontrol-device")) {
    Serial.println("mDNS responder started");
  }
  
  // Setup routes
  setupRoutes();
  
  server.begin();
  udp.begin(8266);
  
  digitalWrite(GPIO_PINS[0], HIGH); // Status LED
  
  Serial.println(" EZ Control Ready!");
  Serial.println(" Car Control: ALWAYS AVAILABLE");
  Serial.println(" GPIO Control: ALL PINS AVAILABLE");
  Serial.println(" Motor pins (D1-D5) can be controlled by both car and GPIO");
}

void loop() {
  server.handleClient();
  MDNS.update();
  handleUDPDiscovery();
  
  // Auto-stop after 5 seconds of no command
  if (isMoving && (millis() - lastCommandTime > 5000)) {
    stopCar();
  }
}

void connectToWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    isHotspotMode = false;
  } else {
    Serial.println("\nStarting AP mode...");
    WiFi.mode(WIFI_AP);
    WiFi.softAP(hotspot_ssid, hotspot_password);
    Serial.print("AP IP: ");
    Serial.println(WiFi.softAPIP());
    isHotspotMode = true;
  }
}

void setupRoutes() {
  server.on("/", HTTP_GET, handleRoot);
  server.on("/status", HTTP_GET, handleStatus);
  server.on("/discovery", HTTP_GET, handleDiscovery);
  server.on("/test", HTTP_GET, handleTest);
  
  // Car control routes
  server.on("/car_forward", HTTP_POST, handleForward);
  server.on("/car_backward", HTTP_POST, handleBackward);
  server.on("/car_left", HTTP_POST, handleLeft);
  server.on("/car_right", HTTP_POST, handleRight);
  server.on("/car_left_forward", HTTP_POST, handleLeftForward);
  server.on("/car_right_forward", HTTP_POST, handleRightForward);
  server.on("/car_left_backward", HTTP_POST, handleLeftBackward);
  server.on("/car_right_backward", HTTP_POST, handleRightBackward);
  server.on("/car_stop", HTTP_POST, handleStop);
  server.on("/car_speed", HTTP_POST, handleSpeed);
  
  // GPIO control routes
  server.on("/pin", HTTP_POST, handlePinControl);
  server.on("/toggle", HTTP_POST, handlePinToggle);
  server.on("/pins", HTTP_GET, handleGetAllPins);
  server.on("/reset", HTTP_POST, handleResetAllPins);
  
  server.enableCORS(true);
}

// Car control handlers with detailed logging
void handleForward() {
  Serial.println("=== CAR FORWARD COMMAND RECEIVED ===");
  int speed = getSpeedFromRequest();
  Serial.print("Speed: ");
  Serial.println(speed);
  
  moveForward(speed);
  Serial.println("=== FORWARD COMMAND EXECUTED ===");
  sendSuccessResponse();
}

void handleBackward() {
  Serial.println("=== CAR BACKWARD COMMAND RECEIVED ===");
  int speed = getSpeedFromRequest();
  Serial.print("Speed: ");
  Serial.println(speed);
  
  moveBackward(speed);
  Serial.println("=== BACKWARD COMMAND EXECUTED ===");
  sendSuccessResponse();
}

void handleLeft() {
  Serial.println("=== CAR LEFT COMMAND RECEIVED ===");
  int speed = getSpeedFromRequest();
  Serial.print("Speed: ");
  Serial.println(speed);
  
  turnLeft(speed);
  Serial.println("=== LEFT COMMAND EXECUTED ===");
  sendSuccessResponse();
}

void handleRight() {
  Serial.println("=== CAR RIGHT COMMAND RECEIVED ===");
  int speed = getSpeedFromRequest();
  Serial.print("Speed: ");
  Serial.println(speed);
  
  turnRight(speed);
  Serial.println("=== RIGHT COMMAND EXECUTED ===");
  sendSuccessResponse();
}

void handleLeftForward() {
  Serial.println("=== CAR LEFT-FORWARD COMMAND RECEIVED ===");
  int speed = getSpeedFromRequest();
  moveLeftForward(speed);
  sendSuccessResponse();
}

void handleRightForward() {
  Serial.println("=== CAR RIGHT-FORWARD COMMAND RECEIVED ===");
  int speed = getSpeedFromRequest();
  moveRightForward(speed);
  sendSuccessResponse();
}

void handleLeftBackward() {
  Serial.println("=== CAR LEFT-BACKWARD COMMAND RECEIVED ===");
  int speed = getSpeedFromRequest();
  moveLeftBackward(speed);
  sendSuccessResponse();
}

void handleRightBackward() {
  Serial.println("=== CAR RIGHT-BACKWARD COMMAND RECEIVED ===");
  int speed = getSpeedFromRequest();
  moveRightBackward(speed);
  sendSuccessResponse();
}

void handleStop() {
  Serial.println("=== CAR STOP COMMAND RECEIVED ===");
  stopCar();
  Serial.println("=== STOP COMMAND EXECUTED ===");
  sendSuccessResponse();
}

void handleSpeed() {
  Serial.println("=== CAR SPEED COMMAND RECEIVED ===");
  if (server.hasArg("speed")) {
    int speed = server.arg("speed").toInt();
    setSpeed(speed);
    Serial.print("Speed set to: ");
    Serial.println(speed);
  }
  sendSuccessResponse();
}

// GPIO control handlers
void handlePinControl() {
  if (server.hasArg("pin") && server.hasArg("state")) {
    String pinStr = server.arg("pin");
    String stateStr = server.arg("state");
    
    int pinIndex = getPinIndex(pinStr);
    if (pinIndex >= 0) {
      bool state = (stateStr == "HIGH");
      setPinState(pinIndex, state);
      sendSuccessResponse();
    } else {
      sendErrorResponse("Invalid pin");
    }
  } else {
    sendErrorResponse("Missing pin or state parameter");
  }
}

void handlePinToggle() {
  if (server.hasArg("pin")) {
    String pinStr = server.arg("pin");
    int pinIndex = getPinIndex(pinStr);
    
    if (pinIndex >= 0) {
      gpioStates[pinIndex] = !gpioStates[pinIndex];
      digitalWrite(GPIO_PINS[pinIndex], gpioStates[pinIndex] ? HIGH : LOW);
      sendSuccessResponse();
    } else {
      sendErrorResponse("Invalid pin");
    }
  } else {
    sendErrorResponse("Missing pin parameter");
  }
}

void handleGetAllPins() {
  DynamicJsonDocument doc(512);
  JsonObject pinStates = doc.createNestedObject("pinStates");
  
  String pinNames[] = {"D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7", "D8", "A0"};
  for (int i = 0; i < NUM_GPIO_PINS; i++) {
    pinStates[pinNames[i]] = gpioStates[i] ? "HIGH" : "LOW";
  }
  
  String response;
  serializeJson(doc, response);
  server.send(200, "application/json", response);
}

void handleResetAllPins() {
  for (int i = 0; i < NUM_GPIO_PINS; i++) {
    gpioStates[i] = false;
    digitalWrite(GPIO_PINS[i], LOW);
  }
  sendSuccessResponse();
}

// Status and discovery handlers
void handleRoot() {
  String html = "<html><head><title>EZ Control Device</title>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<style>body{font-family:Arial;margin:20px;background:#f0f0f0;}";
  html += ".container{max-width:800px;margin:0 auto;background:white;padding:20px;border-radius:10px;}";
  html += ".status{background:#e8f5e8;padding:10px;border-radius:5px;margin:10px 0;}";
  html += "</style></head><body>";
  html += "<div class='container'>";
  html += "<h1>EZ Control Flexible Controller</h1>";
  
  if (isHotspotMode) {
    html += "<div class='status'>";
    html += "<strong>Mode:</strong> Hotspot Mode<br>";
    html += "<strong>SSID:</strong> " + String(hotspot_ssid) + "<br>";
    html += "<strong>IP:</strong> " + WiFi.softAPIP().toString() + "<br>";
    html += "<strong>MAC:</strong> " + deviceMAC + "<br>";
    html += "</div>";
  } else {
    html += "<div class='status'>";
    html += "<strong>Mode:</strong> WiFi Mode<br>";
    html += "<strong>IP:</strong> " + WiFi.localIP().toString() + "<br>";
    html += "<strong>MAC:</strong> " + deviceMAC + "<br>";
    html += "<strong>SSID:</strong> " + WiFi.SSID() + "<br>";
    html += "</div>";
  }
  
  html += "<h2>Features</h2>";
  html += "<p><strong>Car Control:</strong> Always available, controls motor pins (D1-D5)</p>";
  html += "<p><strong>GPIO Control:</strong> All pins available (D0-D8, A0)</p>";
  html += "<p><strong>Flexible:</strong> Both systems work simultaneously</p>";
  html += "</div></body></html>";
  
  server.send(200, "text/html", html);
}

void handleStatus() {
  DynamicJsonDocument doc(1024);
  doc["name"] = DEVICE_NAME;
  doc["version"] = DEVICE_VERSION;
  doc["device"] = "nodemcu";
  doc["mode"] = isHotspotMode ? "hotspot" : "wifi";
  
  if (isHotspotMode) {
    doc["ip"] = WiFi.softAPIP().toString();
    doc["ssid"] = hotspot_ssid;
  } else {
    doc["ip"] = WiFi.localIP().toString();
    doc["ssid"] = WiFi.SSID();
    doc["rssi"] = WiFi.RSSI();
  }
  
  doc["mac"] = deviceMAC;
  doc["status"] = "connected";
  doc["direction"] = currentDirection;
  doc["speed"] = currentSpeed;
  doc["isMoving"] = isMoving;
  
  JsonObject pinStates = doc.createNestedObject("pinStates");
  String pinNames[] = {"D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7", "D8", "A0"};
  for (int i = 0; i < NUM_GPIO_PINS; i++) {
    pinStates[pinNames[i]] = gpioStates[i] ? "HIGH" : "LOW";
  }
  
  String response;
  serializeJson(doc, response);
  server.send(200, "application/json", response);
}

void handleDiscovery() {
  DynamicJsonDocument doc(512);
  doc["type"] = "response";
  doc["device"] = "nodemcu";
  doc["name"] = DEVICE_NAME;
  doc["version"] = DEVICE_VERSION;
  doc["mode"] = isHotspotMode ? "hotspot" : "wifi";
  
  if (isHotspotMode) {
    doc["ip"] = WiFi.softAPIP().toString();
    doc["ssid"] = hotspot_ssid;
  } else {
    doc["ip"] = WiFi.localIP().toString();
    doc["ssid"] = WiFi.SSID();
    doc["rssi"] = WiFi.RSSI();
  }
  
  doc["mac"] = deviceMAC;
  doc["port"] = 80;
  doc["discovery_port"] = 8266;
  
  JsonObject pinStates = doc.createNestedObject("pinStates");
  String pinNames[] = {"D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7", "D8", "A0"};
  for (int i = 0; i < NUM_GPIO_PINS; i++) {
    pinStates[pinNames[i]] = gpioStates[i] ? "HIGH" : "LOW";
  }
  
  String response;
  serializeJson(doc, response);
  server.send(200, "application/json", response);
}

void handleTest() {
  Serial.println("=== TEST ENDPOINT CALLED ===");
  Serial.println("Connection test successful!");
  Serial.print("Current Direction: ");
  Serial.println(currentDirection);
  Serial.print("Current Speed: ");
  Serial.println(currentSpeed);
  Serial.print("Is Moving: ");
  Serial.println(isMoving ? "YES" : "NO");
  
  server.send(200, "application/json", "{\"status\":\"success\",\"message\":\"Test successful\"}");
}

// UDP Discovery handler
void handleUDPDiscovery() {
  int packetSize = udp.parsePacket();
  if (packetSize) {
    char incomingPacket[255];
    int len = udp.read(incomingPacket, 255);
    if (len > 0) {
      incomingPacket[len] = 0;
    }
    
    Serial.print("UDP packet received from ");
    Serial.print(udp.remoteIP());
    Serial.print(":");
    Serial.println(udp.remotePort());
    
    DynamicJsonDocument doc(256);
    DeserializationError error = deserializeJson(doc, incomingPacket);
    
    if (!error && doc["type"] == "discovery") {
      Serial.println("Discovery request received, sending response...");
      
      DynamicJsonDocument responseDoc(512);
      responseDoc["type"] = "response";
      responseDoc["device"] = "nodemcu";
      responseDoc["name"] = DEVICE_NAME;
      responseDoc["version"] = DEVICE_VERSION;
      responseDoc["mode"] = isHotspotMode ? "hotspot" : "wifi";
      
      if (isHotspotMode) {
        responseDoc["ip"] = WiFi.softAPIP().toString();
        responseDoc["ssid"] = hotspot_ssid;
      } else {
        responseDoc["ip"] = WiFi.localIP().toString();
        responseDoc["ssid"] = WiFi.SSID();
        responseDoc["rssi"] = WiFi.RSSI();
      }
      
      responseDoc["mac"] = deviceMAC;
      responseDoc["port"] = 80;
      responseDoc["discovery_port"] = 8266;
      
      JsonObject pinStates = responseDoc.createNestedObject("pinStates");
      String pinNames[] = {"D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7", "D8", "A0"};
      for (int i = 0; i < NUM_GPIO_PINS; i++) {
        pinStates[pinNames[i]] = gpioStates[i] ? "HIGH" : "LOW";
      }
      
      String response;
      serializeJson(responseDoc, response);
      
      udp.beginPacket(udp.remoteIP(), udp.remotePort());
      udp.write(response.c_str());
      udp.endPacket();
      
      Serial.println("Discovery response sent");
    }
  }
}

// Helper functions
int getSpeedFromRequest() {
  if (server.hasArg("speed")) {
    return server.arg("speed").toInt();
  }
  return currentSpeed > 0 ? currentSpeed : 512;
}

int getPinIndex(String pinStr) {
  if (pinStr == "D0") return 0;
  if (pinStr == "D1") return 1;
  if (pinStr == "D2") return 2;
  if (pinStr == "D3") return 3;
  if (pinStr == "D4") return 4;
  if (pinStr == "D5") return 5;
  if (pinStr == "D6") return 6;
  if (pinStr == "D7") return 7;
  if (pinStr == "D8") return 8;
  if (pinStr == "A0") return 9;
  return -1;
}

void setPinState(int pinIndex, bool state) {
  if (pinIndex >= 0 && pinIndex < NUM_GPIO_PINS) {
    gpioStates[pinIndex] = state;
    digitalWrite(GPIO_PINS[pinIndex], state ? HIGH : LOW);
    Serial.printf("GPIO %d set to %s\n", pinIndex + 1, state ? "HIGH" : "LOW");
  }
}

void setSpeed(int speed) {
  currentSpeed = constrain(speed, 0, 1023);
  analogWrite(MOTOR_SPEED, currentSpeed);
  Serial.printf("Speed set to: %d\n", currentSpeed);
}

// Car movement functions with detailed logging
void moveForward(int speed) {
  Serial.println("Setting motor pins for FORWARD movement:");
  Serial.print("  D1 (LEFT_FWD): HIGH, ");
  Serial.print("D2 (LEFT_BWD): LOW, ");
  Serial.print("D3 (RIGHT_FWD): HIGH, ");
  Serial.print("D4 (RIGHT_BWD): LOW, ");
  Serial.print("D5 (SPEED): ");
  Serial.println(speed);
  
  digitalWrite(LEFT_MOTOR_FORWARD, HIGH);
  digitalWrite(LEFT_MOTOR_BACKWARD, LOW);
  digitalWrite(RIGHT_MOTOR_FORWARD, HIGH);
  digitalWrite(RIGHT_MOTOR_BACKWARD, LOW);
  analogWrite(MOTOR_SPEED, speed);
  currentDirection = "FORWARD";
  isMoving = true;
  lastCommandTime = millis();
  Serial.println("Moving FORWARD");
}

void moveBackward(int speed) {
  Serial.println("🔧 Setting motor pins for BACKWARD movement:");
  Serial.print("  D1 (LEFT_FWD): LOW, ");
  Serial.print("D2 (LEFT_BWD): HIGH, ");
  Serial.print("D3 (RIGHT_FWD): LOW, ");
  Serial.print("D4 (RIGHT_BWD): HIGH, ");
  Serial.print("D5 (SPEED): ");
  Serial.println(speed);
  
  digitalWrite(LEFT_MOTOR_FORWARD, LOW);
  digitalWrite(LEFT_MOTOR_BACKWARD, HIGH);
  digitalWrite(RIGHT_MOTOR_FORWARD, LOW);
  digitalWrite(RIGHT_MOTOR_BACKWARD, HIGH);
  analogWrite(MOTOR_SPEED, speed);
  currentDirection = "BACKWARD";
  isMoving = true;
  lastCommandTime = millis();
  Serial.println(" Moving BACKWARD");
}

void turnLeft(int speed) {
  Serial.println("Setting motor pins for LEFT turn:");
  Serial.print("  D1 (LEFT_FWD): LOW, ");
  Serial.print("D2 (LEFT_BWD): LOW, ");
  Serial.print("D3 (RIGHT_FWD): HIGH, ");
  Serial.print("D4 (RIGHT_BWD): LOW, ");
  Serial.print("D5 (SPEED): ");
  Serial.println(speed);
  
  digitalWrite(LEFT_MOTOR_FORWARD, LOW);
  digitalWrite(LEFT_MOTOR_BACKWARD, LOW);
  digitalWrite(RIGHT_MOTOR_FORWARD, HIGH);
  digitalWrite(RIGHT_MOTOR_BACKWARD, LOW);
  analogWrite(MOTOR_SPEED, speed);
  currentDirection = "LEFT";
  isMoving = true;
  lastCommandTime = millis();
  Serial.println(" Turning LEFT");
}

void turnRight(int speed) {
  Serial.println("Setting motor pins for RIGHT turn:");
  Serial.print("  D1 (LEFT_FWD): HIGH, ");
  Serial.print("D2 (LEFT_BWD): LOW, ");
  Serial.print("D3 (RIGHT_FWD): LOW, ");
  Serial.print("D4 (RIGHT_BWD): LOW, ");
  Serial.print("D5 (SPEED): ");
  Serial.println(speed);
  
  digitalWrite(LEFT_MOTOR_FORWARD, HIGH);
  digitalWrite(LEFT_MOTOR_BACKWARD, LOW);
  digitalWrite(RIGHT_MOTOR_FORWARD, LOW);
  digitalWrite(RIGHT_MOTOR_BACKWARD, LOW);
  analogWrite(MOTOR_SPEED, speed);
  currentDirection = "RIGHT";
  isMoving = true;
  lastCommandTime = millis();
  Serial.println(" Turning RIGHT");
}

void moveLeftForward(int speed) {
  digitalWrite(LEFT_MOTOR_FORWARD, LOW);
  digitalWrite(LEFT_MOTOR_BACKWARD, LOW);
  digitalWrite(RIGHT_MOTOR_FORWARD, HIGH);
  digitalWrite(RIGHT_MOTOR_BACKWARD, LOW);
  analogWrite(MOTOR_SPEED, speed);
  currentDirection = "LEFT-FORWARD";
  isMoving = true;
  lastCommandTime = millis();
  Serial.println("Moving LEFT-FORWARD (LF)");
}

void moveRightForward(int speed) {
  digitalWrite(LEFT_MOTOR_FORWARD, HIGH);
  digitalWrite(LEFT_MOTOR_BACKWARD, LOW);
  digitalWrite(RIGHT_MOTOR_FORWARD, LOW);
  digitalWrite(RIGHT_MOTOR_BACKWARD, LOW);
  analogWrite(MOTOR_SPEED, speed);
  currentDirection = "RIGHT-FORWARD";
  isMoving = true;
  lastCommandTime = millis();
  Serial.println("Moving RIGHT-FORWARD (RF)");
}

void moveLeftBackward(int speed) {
  digitalWrite(LEFT_MOTOR_FORWARD, LOW);
  digitalWrite(LEFT_MOTOR_BACKWARD, LOW);
  digitalWrite(RIGHT_MOTOR_FORWARD, LOW);
  digitalWrite(RIGHT_MOTOR_BACKWARD, HIGH);
  analogWrite(MOTOR_SPEED, speed);
  currentDirection = "LEFT-BACKWARD";
  isMoving = true;
  lastCommandTime = millis();
  Serial.println("Moving LEFT-BACKWARD (LB)");
}

void moveRightBackward(int speed) {
  digitalWrite(LEFT_MOTOR_FORWARD, LOW);
  digitalWrite(LEFT_MOTOR_BACKWARD, HIGH);
  digitalWrite(RIGHT_MOTOR_FORWARD, LOW);
  digitalWrite(RIGHT_MOTOR_BACKWARD, LOW);
  analogWrite(MOTOR_SPEED, speed);
  currentDirection = "RIGHT-BACKWARD";
  isMoving = true;
  lastCommandTime = millis();
  Serial.println("Moving RIGHT-BACKWARD (RB)");
}

void stopCar() {
  Serial.println("Setting motor pins for STOP:");
  Serial.print("  D1 (LEFT_FWD): LOW, ");
  Serial.print("D2 (LEFT_BWD): LOW, ");
  Serial.print("D3 (RIGHT_FWD): LOW, ");
  Serial.print("D4 (RIGHT_BWD): LOW, ");
  Serial.println("D5 (SPEED): 0");
  
  digitalWrite(LEFT_MOTOR_FORWARD, LOW);
  digitalWrite(LEFT_MOTOR_BACKWARD, LOW);
  digitalWrite(RIGHT_MOTOR_FORWARD, LOW);
  digitalWrite(RIGHT_MOTOR_BACKWARD, LOW);
  analogWrite(MOTOR_SPEED, 0);
  currentDirection = "STOP";
  isMoving = false;
  Serial.println("Car STOPPED");
}

// Response helpers
void sendSuccessResponse() {
  DynamicJsonDocument doc(128);
  doc["success"] = true;
  doc["status"] = "success";
  
  String response;
  serializeJson(doc, response);
  server.send(200, "application/json", response);
}

void sendErrorResponse(String message) {
  DynamicJsonDocument doc(128);
  doc["success"] = false;
  doc["status"] = "error";
  doc["message"] = message;
  
  String response;
  serializeJson(doc, response);
  server.send(400, "application/json", response);
}
