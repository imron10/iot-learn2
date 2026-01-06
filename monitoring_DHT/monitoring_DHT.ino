#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "Imron";
const char* password = "imron123";

const char* apiUrl = "https://iot-learn1.vercel.app/api/insert";

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Memulai Koneksi WiFi...");
  WiFi.begin(ssid, password);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    attempts++;
    if (attempts > 20) {
      Serial.println("\n❌ Gagal koneksi WiFi");
      return;
    }
  }

  Serial.println("\n✅ WiFi terkoneksi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {

    float suhu = random(200, 401) / 10.0;
    float kelembaban = random(700, 901) / 10.0;

    // JSON SESUAI API NODE.JS
    StaticJsonDocument<128> doc;
    doc["suhu"] = suhu;
    doc["kelembaban"] = kelembaban;
    doc["device"] = "ESP32_01";

    String jsonString;
    serializeJson(doc, jsonString);

    HTTPClient http;
    http.begin(apiUrl);
    http.addHeader("Content-Type", "application/json");

    int httpCode = http.POST(jsonString);

    Serial.println("POST ke Vercel:");
    Serial.println(jsonString);

    if (httpCode > 0) {
      Serial.print("HTTP Code: ");
      Serial.println(httpCode);
      Serial.print("Response: ");
      Serial.println(http.getString());
    } else {
      Serial.print("❌ Gagal kirim: ");
      Serial.println(http.errorToString(httpCode));
    }

    http.end();

  } else {
    Serial.println("WiFi terputus, reconnect...");
    WiFi.reconnect();
  }

  delay(3000); // kirim tiap 3  detik (lebih aman)
}
