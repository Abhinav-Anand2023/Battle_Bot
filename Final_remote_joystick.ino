#define BLYNK_TEMPLATE_ID "Your Template_ID"
#define BLYNK_TEMPLATE_NAME "Your_Template_Name"
#define BLYNK_AUTH_TOKEN "Your Auth token"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// WiFi Credentials
char ssid[] = "Your Wifi";
char pass[] = "Your Password";

// Auto-Reset Variables
unsigned long lastConnected = 0;
const long RESET_TIMEOUT = 10000; // 10 seconds

// Motor Pin Definitions
// Motor 1
const int RPWM1 = 18;  
const int LPWM1 = 19;
const int REN1 = 21;  // REN1 used for motor 1 and motor 2 both
const int LEN1 = 22;  // LEN1 used for motor 1 and motor 2 both 

// Motor 2
const int RPWM2 = 26;  //23
const int LPWM2 = 25;
const int REN2 = 32;  // REN2 used for motor 1 and motor 2 both
const int LEN2 = 33;  // LEN2 used for motor 1 and motor 2 both 

// Spinner Motor Pins (BTS7960)
const int SPINNER_RPWM = 16;  // PWM for clockwise
const int SPINNER_LPWM = 17;  // PWM for anticlockwise
const int SPINNER_R_EN = 4;  // Right Enable
const int SPINNER_L_EN = 5;  // Left Enable

// LEDC PWM Configuration
const int PWM_FREQ = 10000;  // 10 kHz for BTS7960
const int PWM_RES = 8;       // 8-bit resolution

// Speed Control
int speedPercentage = 100; // Default to full speed

// LEDC Channel Assignments
#define CH_RPWM1 0
#define CH_LPWM1 1
#define CH_RPWM2 2
#define CH_LPWM2 3
#define CH_SPINNER_RPWM 4
#define CH_SPINNER_LPWM 5

// Joystick Values
int x = 0;
int y = 0;

String connectionStatus = "Disconnected";
unsigned long lastUpdate = 0;
const long updateInterval = 2000; // Update every 2 seconds

// Spinner Control Variables
int spinnerSpeed = 0;          // Slider value (0-255)
bool clockwiseActive = false;
bool anticlockwiseActive = false;


void setup() {
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  lastConnected = millis(); // Initialize connection timer

  // Enable Pin Setup
  pinMode(REN1, OUTPUT);
  pinMode(LEN1, OUTPUT);
  pinMode(REN2, OUTPUT);
  pinMode(LEN2, OUTPUT);

  // LEDC PWM Setup
  // Motor 1
  ledcSetup(CH_RPWM1, PWM_FREQ, PWM_RES);
  ledcAttachPin(RPWM1, CH_RPWM1);
  ledcSetup(CH_LPWM1, PWM_FREQ, PWM_RES);
  ledcAttachPin(LPWM1, CH_LPWM1);

  // Motor 2
  ledcSetup(CH_RPWM2, PWM_FREQ, PWM_RES);
  ledcAttachPin(RPWM2, CH_RPWM2);
  ledcSetup(CH_LPWM2, PWM_FREQ, PWM_RES);
  ledcAttachPin(LPWM2, CH_LPWM2);

  // Initial status
  Blynk.virtualWrite(V4, "Connecting...");

  // Spinner Motor Pins
  pinMode(SPINNER_R_EN, OUTPUT);
  pinMode(SPINNER_L_EN, OUTPUT);
  digitalWrite(SPINNER_R_EN, LOW); // Disable initially
  digitalWrite(SPINNER_L_EN, LOW);

  // PWM Setup
  ledcSetup(CH_SPINNER_RPWM, PWM_FREQ, PWM_RES);
  ledcAttachPin(SPINNER_RPWM, CH_SPINNER_RPWM);
  ledcSetup(CH_SPINNER_LPWM, PWM_FREQ, PWM_RES);
  ledcAttachPin(SPINNER_LPWM, CH_SPINNER_LPWM);

}

// Manual Reset via Blynk (V9)
BLYNK_WRITE(V9) { 
  if (param.asInt() == 1) {
    ESP.restart();
  }
}


BLYNK_WRITE(V3) {
  speedPercentage = param.asInt();
}

// Update status on Blynk connection/disconnection
// Improved connection handling
BLYNK_CONNECTED() {
  updateConnectionStatus();
}

BLYNK_DISCONNECTED() {
  updateConnectionStatus();
}


BLYNK_WRITE(V0) { x = param.asInt(); }
BLYNK_WRITE(V1) { y = param.asInt(); }



// Slider for Spinner Speed (V6)
BLYNK_WRITE(V6) {
  spinnerSpeed = param.asInt(); 
  updateSpinner(); 
}

// Clockwise Switch (V7) 
BLYNK_WRITE(V7) {
  if (param.asInt()) {
    anticlockwiseActive = false;
    Blynk.virtualWrite(V8, 0); // Force anticlockwise OFF
    clockwiseActive = true;
  } else {
    clockwiseActive = false;
  }
  updateSpinner();
}

// Anticlockwise Switch (V8)
BLYNK_WRITE(V8) {
  if (param.asInt()) {
    clockwiseActive = false;
    Blynk.virtualWrite(V7, 0); // Force clockwise OFF
    anticlockwiseActive = true;
  } else {
    anticlockwiseActive = false;
  }
  updateSpinner();
}

void updateConnectionStatus() {
  String wifiStatus = WiFi.status() == WL_CONNECTED ? 
    "Wi-Fi: Connected | Signal: " + String(WiFi.RSSI()) + "dBm" : 
    "Wi-Fi: Disconnected";
    
  String blynkStatus = Blynk.connected() ? 
    "Blynk: Connected" : 
    "Blynk: Disconnected";

  connectionStatus = wifiStatus + " | " + blynkStatus;
  Blynk.virtualWrite(V4, connectionStatus);
}


// Update Spinner Motor
void updateSpinner() {
  if (clockwiseActive) {
    digitalWrite(SPINNER_R_EN, HIGH); // Enable clockwise
    digitalWrite(SPINNER_L_EN, HIGH);
    ledcWrite(CH_SPINNER_RPWM, spinnerSpeed); // Set speed
    ledcWrite(CH_SPINNER_LPWM, 0);
    Serial.println("Spinner Clockwise");
  } 
  else if (anticlockwiseActive) {
    digitalWrite(SPINNER_L_EN, HIGH); // Enable anticlockwise
    digitalWrite(SPINNER_R_EN, HIGH);
    ledcWrite(CH_SPINNER_LPWM, spinnerSpeed); // Set speed
    ledcWrite(CH_SPINNER_RPWM, 0);
    Serial.println("Spinner AntiClockwise");
  } 
  else {
    digitalWrite(SPINNER_R_EN, LOW); // Disable both
    digitalWrite(SPINNER_L_EN, LOW);
    ledcWrite(CH_SPINNER_RPWM, 0);
    ledcWrite(CH_SPINNER_LPWM, 0);
  }
}


// Add this function to update signal strength
void updateSignal() {
  if (WiFi.status() == WL_CONNECTED) {
    int rssi = WiFi.RSSI(); // Get signal strength
    Blynk.virtualWrite(V4, "Wi-Fi: Connected | Signal: " + String(rssi) + "dBm");
  } else {
    Blynk.virtualWrite(V4, "Wi-Fi: Disconnected");
  }
}



void loop() {
  Blynk.run();

  // Auto-Reset on Connection Loss
  if (Blynk.connected() && WiFi.status() == WL_CONNECTED) {
    lastConnected = millis();
  } else {
    if (millis() - lastConnected > RESET_TIMEOUT) {
      ESP.restart();
    }
  }
    
    if(x==0 && y==0){
          stop();
     }
   // Forward
     else if(x >= -40 && x <= 40 && y >= 0 && y <= 255){
          forward();
     }
          
  // Backward
     else if(x >= -40 && x <= 40 && y >= -255 && y <= 0){
          backward();
     }
     
  // Left
     else if(x >= -255 && x <= 0 && y >= -40 && y <= 40){
          left();
     }
     
  // Right
     else if(x >= 0 && x <= 255 && y >= -40 && y <= 40){
          right();
     }

  // Forward Right
     else if(x > 0 && x < 255 && y > 40 && y < 255){
          forwardRight();
     }

  // Forward Left
     else if(x > -255 && x < 0 && y > 40 && y < 255){
          forwardLeft();
     }

  // Backward Right
     else if(x > 0 && x < 255 && y > -255 && y < -40){
          backwardRight();
     }

  // Backward Left
     else if(x >= -255 && x <= 0 && y >= -255 && y <= -40){
          backwardLeft();
     }

       // Update status regularly
  if (millis() - lastUpdate > updateInterval) {
    updateConnectionStatus();
    lastUpdate = millis();
  }
}

// Motor Control Functions
void forward() {
  //forward
  Serial.println("Forward");

  int scaledY = (y * speedPercentage) / 100;

  digitalWrite(REN1, HIGH);
  digitalWrite(LEN1, HIGH);
  digitalWrite(REN2, HIGH);
  digitalWrite(LEN2, HIGH);

  ledcWrite(CH_RPWM1, scaledY);
  ledcWrite(CH_LPWM1, 0);
  ledcWrite(CH_RPWM2, scaledY);
  ledcWrite(CH_LPWM2, 0);
}

void backward() {
  //Backward
  Serial.println("Backward");
  
  int scaledY = (-y * speedPercentage) / 100;

  digitalWrite(REN1, HIGH);
  digitalWrite(LEN1, HIGH);
  digitalWrite(REN2, HIGH);
  digitalWrite(LEN2, HIGH);

  ledcWrite(CH_RPWM1, 0);
  ledcWrite(CH_LPWM1, scaledY);
  ledcWrite(CH_RPWM2, 0);
  ledcWrite(CH_LPWM2, scaledY);
}

void stop() {
  //Stop
  //Serial.println("Stop");
  
  digitalWrite(REN1, LOW);
  digitalWrite(LEN1, LOW);
  digitalWrite(REN2, LOW);
  digitalWrite(LEN2, LOW);

  ledcWrite(CH_RPWM1, 0);
  ledcWrite(CH_LPWM1, 0);
  ledcWrite(CH_RPWM2, 0);
  ledcWrite(CH_LPWM2, 0);
}

void left() {
  //Left
  Serial.println("Left");

  int speed = (255 * speedPercentage) / 100;

  digitalWrite(REN1, HIGH);
  digitalWrite(LEN1, HIGH);
  digitalWrite(REN2, HIGH);
  digitalWrite(LEN2, HIGH);

  ledcWrite(CH_RPWM1, 0);
  ledcWrite(CH_LPWM1, speed);
  ledcWrite(CH_RPWM2, speed);
  ledcWrite(CH_LPWM2, 0);
}

void right() {
  //Right
  Serial.println("Right");

  int speed = (255 * speedPercentage) / 100;

  digitalWrite(REN1, HIGH);
  digitalWrite(LEN1, HIGH);
  digitalWrite(REN2, HIGH);
  digitalWrite(LEN2, HIGH);

  ledcWrite(CH_RPWM1, speed);
  ledcWrite(CH_LPWM1, 0);
  ledcWrite(CH_RPWM2, 0);
  ledcWrite(CH_LPWM2, speed);
}

void forwardRight() {
  //Forward Right
  Serial.println("Forward_Right");

  int scaledY = (y * speedPercentage) / 100;
  int scaledX = (x * speedPercentage) / 100;

  digitalWrite(REN1, HIGH);
  digitalWrite(LEN1, HIGH);
  digitalWrite(REN2, HIGH);
  digitalWrite(LEN2, HIGH);

  ledcWrite(CH_RPWM1, scaledY);
  ledcWrite(CH_LPWM1, 0);
  ledcWrite(CH_RPWM2, scaledX/1.5);
  ledcWrite(CH_LPWM2, 0);
}

  void forwardLeft() {
  //Forward Left
  Serial.println("Forward_Left");
 
  int scaledY = (y * speedPercentage) / 100;
  int scaledX = (x * speedPercentage) / 100;

  digitalWrite(REN1, HIGH);
  digitalWrite(LEN1, HIGH);
  digitalWrite(REN2, HIGH);
  digitalWrite(LEN2, HIGH);

  ledcWrite(CH_RPWM1, ((-scaledX)/1.5));
  ledcWrite(CH_LPWM1, 0);
  ledcWrite(CH_RPWM2, scaledY);
  ledcWrite(CH_LPWM2, 0);
}

void backwardRight() {
  //Backward Right
  Serial.println("BackwardRight");
 
  int scaledY = (y * speedPercentage) / 100;
  int scaledX = (x * speedPercentage) / 100;

  digitalWrite(REN1, HIGH);
  digitalWrite(LEN1, HIGH);
  digitalWrite(REN2, HIGH);
  digitalWrite(LEN2, HIGH);

  ledcWrite(CH_RPWM1, 0);
  ledcWrite(CH_LPWM1, ((-scaledY)));
  ledcWrite(CH_RPWM2, 0);
  ledcWrite(CH_LPWM2, scaledX/1.5);
}

void backwardLeft() {
  //Backward Left
  Serial.println("Backward_left");

  int scaledY = (y * speedPercentage) / 100;
  int scaledX = (x * speedPercentage) / 100;

  digitalWrite(REN1, HIGH);
  digitalWrite(LEN1, HIGH);
  digitalWrite(REN2, HIGH);
  digitalWrite(LEN2, HIGH);

  ledcWrite(CH_RPWM1, 0);
  ledcWrite(CH_LPWM1, ((-scaledX)/1.5));
  ledcWrite(CH_RPWM2, 0);
  ledcWrite(CH_LPWM2, ((-scaledY)));

}

