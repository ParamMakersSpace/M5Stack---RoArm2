#include <M5Unified.h>
#include <WiFi.h>
#include <HTTPClient.h>

// WiFi credentials for RoArm2
const char* ssid = "RoArm-M2";
const char* password = "12345678";

// Robot IP
const char* robot_ip = "192.168.4.1";

// Base robot position
float baseX = 150.0;
float baseY = 150.0;
float baseZ = 80.0;
float baseT = 1.57; // ~90° in radians

// Sensitivity multipliers for motion mapping
float gyroScale = 0.02;   // adjust for rotation
float accelScale = 5.0;   // adjust for translation
float zScale = 3.0;

// Current dynamic position
float posX = baseX;
float posY = baseY;
float posZ = baseZ;
float posT = baseT;

// Function to send robot motion command
void sendCommand(float x, float y, float z, float t) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String jsonCmd = "{\"T\":1041,\"x\":" + String(x) +
                     ",\"y\":" + String(y) +
                     ",\"z\":" + String(z) +
                     ",\"t\":" + String(t) + "}";

    String url = "http://" + String(robot_ip) + "/js?json=" + jsonCmd;

    http.begin(url);
    int httpCode = http.GET();

    if (httpCode > 0) {
      Serial.println("Sent: " + jsonCmd);
      Serial.println("Response: " + http.getString());
    } else {
      Serial.println("HTTP request failed");
    }

    http.end();
  } else {
    Serial.println("WiFi not connected");
  }
}

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);

  Serial.begin(115200);
  M5.Display.setTextFont(&fonts::Font4);
  M5.Display.setTextSize(1);
  M5.Display.setTextColor(TFT_WHITE, TFT_BLACK);
  M5.Display.fillScreen(TFT_BLACK);
  M5.Display.println("Connecting to RoArm2...");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    M5.Display.print(".");
  }
  M5.Display.println("\nConnected to RoArm2!");
  Serial.println("WiFi Connected.");
}

void loop() {
  if (M5.Imu.update()) {
    float ax, ay, az;
    float gx, gy, gz;

    // Get IMU readings
    M5.Imu.getAccel(&ax, &ay, &az);
    M5.Imu.getGyro(&gx, &gy, &gz);

    // Map acceleration to position deltas
    posX += ay * accelScale;
    posY += ax * accelScale;
    posZ += (az - 1.0) * zScale; // subtract gravity offset
    posT += gz * gyroScale;

    // Clamp to safe robot working range
    posX = constrain(posX, 100, 200);
    posY = constrain(posY, 100, 200);
    posZ = constrain(posZ, 60, 100);
    posT = constrain(posT, 0.5, 2.5);

    // Send new command
    sendCommand(posX, posY, posZ, posT);

    // Display feedback
    M5.Display.fillScreen(TFT_BLACK);
    M5.Display.setCursor(0, 0);
    M5.Display.printf("IMU Control -> RoArm2\n");
    M5.Display.printf("X: %.2f  Y: %.2f\n", posX, posY);
    M5.Display.printf("Z: %.2f  T: %.2f\n", posZ, posT);
    M5.Display.printf("ax: %.2f  ay: %.2f  az: %.2f\n", ax, ay, az);
    M5.Display.printf("gx: %.2f  gy: %.2f  gz: %.2f\n", gx, gy, gz);

    delay(100); // smooth response
  }
}
