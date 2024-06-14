#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

const char* WIFI_SSID = "my_wifi_network";
const char* WIFI_PASSWORD = "my_wifi_password";

const char* SERVER_URL = "http://127.0.0.1:5000/sensor/data";

#define DHTPIN 4
#define DHTTYPE DHT11
DHT dhtSensor(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  connectToWiFi();
  dhtSensor.begin();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    float temp = dhtSensor.readTemperature();
    float humid = dhtSensor.readHumidity();
    
    if (isnan(temp) || isnan(humid)) {
      Serial.println("Error reading sensor data!");
      return;
    }

    String jsonData = "{\"temperature\": " + String(temp) + ", \"humidity\": " + String(humid) + "}";
    sendPostRequest(SERVER_URL, jsonData);
  } else {
    Serial.println("WiFi disconnected!");
  }

  delay(1000);
}

void connectToWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void sendPostRequest(const char* url, String data) {
  HTTPClient http;
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  int responseCode = http.POST(data);
  
  if (responseCode > 0) {
    String response = http.getString();
    Serial.println("Response: " + response);
  } else {
    Serial.println("Error sending POST request: " + String(responseCode));
  }
  
  http.end();
}
