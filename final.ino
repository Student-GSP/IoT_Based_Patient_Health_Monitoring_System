/*
---------------------------------------------------
Project : IoT-Based Patient Health Monitoring System
Author  : Shobha Gupta
Board   : ESP32 DevKit V1
Sensor  : MAX30100
Display : OLED 128x64
Features:
- Heart Rate Monitoring
- SpO2 Monitoring
- OLED Display
- Web Dashboard
- WiFi Connectivity
---------------------------------------------------
*/

// ===== IoT Patient Monitoring (MAX30100 + ESP32 + OLED 128x64 + Web dashboard + wifi connectivity) =====
#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ---------- OLED Configuration ----------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ---------- Wi-Fi Credentials ----------
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

// ---------- Objects ----------
PulseOximeter pox;
WebServer server(80);

// ---------- Variables ----------
float BPM = 0, SpO2 = 0;
uint32_t lastReport = 0;
bool beatDetected = false;

// ---------- Alert Pins ----------
#define LED_PIN 4        // Indicator LED
#define BUZZER_PIN 2     // Buzzer pin

// ---------- LEDC Settings ----------
#define BUZZER_CHANNEL 0
#define BUZZER_FREQ 1000
#define BUZZER_RESOLUTION 8

// ---------- Beat Callback ----------
void onBeatDetected() {
  Serial.println("❤️ Beat detected!");
  beatDetected = true;
}

// ---------- HTML Dashboard ----------
String webpage(float BPM, float SpO2) {
  String html = "<!DOCTYPE html><html><head><title>Patient Monitor</title>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<style>body{font-family:Arial;text-align:center;background:#f7f7f7;}h1{color:#008080;}";
  html += ".card{background:#fff;padding:20px;margin:20px auto;width:300px;"
          "box-shadow:0 4px 8px rgba(0,0,0,0.2);border-radius:10px;}"
          ".alert{color:red;font-weight:bold;}</style></head><body>";
  html += "<h1>Health Monitor</h1><div class='card'>";

  if (BPM <= 0 || SpO2 <= 0) {
    html += "<h3>Waiting for finger...</h3>";
  } else {
    html += "<h3>Heart Rate: <span style='color:red;'>" + String((int)BPM) + " BPM</span></h3>";
    html += "<h3>SpO₂: <span style='color:blue;'>" + String((int)SpO2) + " %</span></h3>";

    if (BPM < 60 || BPM > 100)
      html += "<p class='alert'>Abnormal Heart Rate!</p>";
    if (SpO2 < 95)
      html += "<p class='alert'>Low Oxygen Level!</p>";
  }

  html += "</div><script>setTimeout(()=>{location.reload();},1000);</script></body></html>";
  return html;
}

// ---------- Web Server Route ----------
void handleRoot() {
  server.send(200, "text/html", webpage(BPM, SpO2));
}

// ---------- OLED Update Function ----------
void updateOLED(float BPM, float SpO2, bool beatDetected) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Header
  display.setCursor(10, 0);
  display.println("IoT Patient Monitor");
  display.drawLine(0, 10, 127, 10, SSD1306_WHITE);

  if (BPM <= 0 || SpO2 <= 0 || isnan(BPM) || isnan(SpO2)) {
    display.setCursor(20, 30);
    display.println("Place finger...");
  } else {
    display.setCursor(0, 20);
    display.print("Heart Rate: ");
    display.print((int)BPM);
    display.println(" BPM");

    display.setCursor(0, 35);
    display.print("SpO2: ");
    display.print((int)SpO2);
    display.println(" %");

    // Alerts
    if (BPM < 60 || BPM > 100) {
      display.setCursor(0, 50);
      display.print("HR Alert!");
    }
    if (SpO2 < 95) {
      display.setCursor(70, 48);
      display.print("SpO2 Low!");
    }

    // Heartbeat animation
    if (beatDetected) {
      display.fillCircle(120, 30, 5, SSD1306_WHITE);
    } else {
      display.drawCircle(120, 30, 5, SSD1306_WHITE);
    }
  }

  display.display();
}

// ---------- Setup ----------
void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22); // SDA, SCL pins

  pinMode(LED_PIN, OUTPUT);
  ledcSetup(BUZZER_CHANNEL, BUZZER_FREQ, BUZZER_RESOLUTION);
  ledcAttachPin(BUZZER_PIN, BUZZER_CHANNEL);
  digitalWrite(LED_PIN, LOW);
  ledcWriteTone(BUZZER_CHANNEL, 0);

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("❌ OLED init failed!");
    while (1);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 25);
  display.println("System Starting...");
  display.display();

  // Wi-Fi Setup
  Serial.println("Connecting WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  display.clearDisplay();
  display.setCursor(10, 25);
  display.println("WiFi Connected!");
  display.display();
  delay(1000);

  // Initialize MAX30100
  Serial.println("Initializing MAX30100...");
  if (!pox.begin()) {
    Serial.println("❌ MAX30100 init failed! Check wiring.");
    display.clearDisplay();
    display.setCursor(10, 25);
    display.println("MAX30100 Error!");
    display.display();
    while (1);
  }
  Serial.println("✅ MAX30100 ready.");
  pox.setOnBeatDetectedCallback(onBeatDetected);
  pox.setIRLedCurrent(MAX30100_LED_CURR_20_8MA);

  // Start Web Server
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started.");

  display.clearDisplay();
  display.setCursor(10, 25);
  display.println("Monitoring...");
  display.display();
}

// ---------- Loop ----------
void loop() {
  pox.update();
  BPM = pox.getHeartRate();
  SpO2 = pox.getSpO2();
  server.handleClient();

  bool heartAlert = (BPM < 60 || BPM > 100);
  bool spo2Alert = (SpO2 < 95 || SpO2 > 100);

  if (heartAlert || spo2Alert) {
    digitalWrite(LED_PIN, HIGH);
    ledcWriteTone(BUZZER_CHANNEL, 1000);
     }
else {
    digitalWrite(LED_PIN, LOW);
    ledcWriteTone(BUZZER_CHANNEL, 0);
     }

  // Update every 1 second
  if (millis() - lastReport > 1000) {
    Serial.print("Heart Rate: "); Serial.print(BPM);
    Serial.print(" | SpO2: "); Serial.println(SpO2);

    updateOLED(BPM, SpO2, beatDetected);
    beatDetected = false; // reset after display update
    lastReport = millis();
  }

  delay(20);
}
