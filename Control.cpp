#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// Your WiFi credentials.
char ssid[] = "batu";
char pass[] = "idenyupe";

// --- MOTOR PIN DEFINITIONS ---
// Left Motor (Driver 1)
#define LEFT_EN    13  // Master Enable
#define LEFT_LPWM  14  // Forward
#define LEFT_RPWM  27  // Reverse

// Right Motor (Driver 2)
#define RIGHT_EN   25  // Master Enable
#define RIGHT_LPWM 26  // Forward
#define RIGHT_RPWM 33  // Reverse

// --- PWM SETTINGS ---
// ESP32 uses channels for PWM. We need 4 channels for 4 direction pins.
const int freq = 5000;
const int resolution = 8; // 8-bit resolution = speeds from 0 to 255
const int maxSpeed = 200; // Adjust this (0-255) for your desired top speed

const int leftFwdCh = 0;
const int leftRevCh = 1;
const int rightFwdCh = 2;
const int rightRevCh = 3;

void setup() {
  Serial.begin(115200);

  // 1. Setup Enable Pins
  pinMode(LEFT_EN, OUTPUT);
  pinMode(RIGHT_EN, OUTPUT);
  
  // Make sure motors are disabled on startup
  digitalWrite(LEFT_EN, LOW);
  digitalWrite(RIGHT_EN, LOW);

  // 2. Setup PWM Channels
  ledcSetup(leftFwdCh, freq, resolution);
  ledcSetup(leftRevCh, freq, resolution);
  ledcSetup(rightFwdCh, freq, resolution);
  ledcSetup(rightRevCh, freq, resolution);

  // 3. Attach PWM Channels to GPIO Pins
  ledcAttachPin(LEFT_LPWM, leftFwdCh);
  ledcAttachPin(LEFT_RPWM, leftRevCh);
  ledcAttachPin(RIGHT_LPWM, rightFwdCh);
  ledcAttachPin(RIGHT_RPWM, rightRevCh);

  // 4. Connect to Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

// --- MOTOR MOVEMENT FUNCTIONS (WITH SOFT START) ---

void stopMotors() {
  ledcWrite(leftFwdCh, 0);
  ledcWrite(leftRevCh, 0);
  ledcWrite(rightFwdCh, 0);
  ledcWrite(rightRevCh, 0);
  digitalWrite(LEFT_EN, LOW);  // Disable drivers to save power
  digitalWrite(RIGHT_EN, LOW);
}

void moveForward() {
  digitalWrite(LEFT_EN, HIGH); // Enable drivers
  digitalWrite(RIGHT_EN, HIGH);
  ledcWrite(leftRevCh, 0);     // Ensure reverse is off
  ledcWrite(rightRevCh, 0);
  
  // Slower Soft Start: Ramp up power over 1 full second to protect battery
  for (int currentSpeed = 0; currentSpeed <= maxSpeed; currentSpeed += 10) {
    ledcWrite(leftFwdCh, currentSpeed); 
    ledcWrite(rightFwdCh, currentSpeed);
    delay(50); // Increased from 15ms to 50ms
  }
}

void moveReverse() {
  digitalWrite(LEFT_EN, HIGH);
  digitalWrite(RIGHT_EN, HIGH);
  ledcWrite(leftFwdCh, 0);     // Ensure forward is off
  ledcWrite(rightFwdCh, 0);
  
  // Slower Soft Start: Ramp up power over 1 full second
  for (int currentSpeed = 0; currentSpeed <= maxSpeed; currentSpeed += 10) {
    ledcWrite(leftRevCh, currentSpeed); 
    ledcWrite(rightRevCh, currentSpeed);
    delay(50); // Increased from 15ms to 50ms
  }
}

void turnLeft() {
  digitalWrite(LEFT_EN, HIGH);
  digitalWrite(RIGHT_EN, HIGH);
  ledcWrite(leftFwdCh, 0);     // Left motor STOPS
  ledcWrite(leftRevCh, 0);
  ledcWrite(rightRevCh, 0);    
  
  // Fast Soft Start: Right motor goes FORWARD (Only 1 motor, so 15ms is safe)
  for (int currentSpeed = 0; currentSpeed <= maxSpeed; currentSpeed += 10) {
    ledcWrite(rightFwdCh, currentSpeed);
    delay(15);
  }
}

void turnRight() {
  digitalWrite(LEFT_EN, HIGH);
  digitalWrite(RIGHT_EN, HIGH);
  ledcWrite(rightFwdCh, 0);    // Right motor STOPS
  ledcWrite(rightRevCh, 0);
  ledcWrite(leftRevCh, 0);
  
  // Fast Soft Start: Left motor goes FORWARD (Only 1 motor, so 15ms is safe)
  for (int currentSpeed = 0; currentSpeed <= maxSpeed; currentSpeed += 10) {
    ledcWrite(leftFwdCh, currentSpeed); 
    delay(15);
  }
}

// --- BLYNK VIRTUAL PIN CONTROLS ---

// V1: Forward Button
BLYNK_WRITE(V1) {
  int buttonState = param.asInt();
  if (buttonState == 1) moveForward();
  else stopMotors();
}

// V2: Reverse Button
BLYNK_WRITE(V2) {
  int buttonState = param.asInt();
  if (buttonState == 1) moveReverse();
  else stopMotors();
}

// V3: Left Button
BLYNK_WRITE(V3) {
  int buttonState = param.asInt();
  if (buttonState == 1) turnLeft();
  else stopMotors();
}

// V4: Right Button
BLYNK_WRITE(V4) {
  int buttonState = param.asInt();
  if (buttonState == 1) turnRight();
  else stopMotors();
}

// --- EMERGENCY FAILSAFE ---
BLYNK_DISCONNECTED() {
  // If the Wi-Fi connection to the phone drops, instantly kill all motors
  stopMotors(); 
  Serial.println("WARNING: Wi-Fi Lost! Motors Stopped to prevent runaway.");
}

void loop() {
  Blynk.run();
}
