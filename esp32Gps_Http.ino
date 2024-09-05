#include <WiFi.h>
#include <HTTPClient.h>
#include <TinyGPS++.h>

const char* ssid = "XXXXXXX";
const char* password = "XXXXXXXX";

// Pines del GPS
#define RXD2 16
#define TXD2 17

HardwareSerial neogps(1);
TinyGPSPlus gps;

void setup() {
  Serial.begin(115200);
  neogps.begin(9600, SERIAL_8N1, RXD2, TXD2);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  while (neogps.available()) {
    if (gps.encode(neogps.read())) {
      if (gps.location.isValid()) {
        float latitude = gps.location.lat();
        float longitude = gps.location.lng();

        if (WiFi.status() == WL_CONNECTED) {
          HTTPClient http;
          String serverName = "http://XXX.XXX.X.XX:3000/api/coordinates"; // Replace with your IP
          http.begin(serverName);
          http.addHeader("Content-Type", "application/json");

          String jsonPayload = "{\"latitude\":" + String(latitude, 6) + ",\"longitude\":" + String(longitude, 6) + "}";
          Serial.print("Sending payload: ");
          Serial.println(jsonPayload);

          int httpResponseCode = http.POST(jsonPayload);

          if (httpResponseCode > 0) {
            String response = http.getString();
            Serial.println(httpResponseCode);
            Serial.println(response);
            Serial.print("Satellites: ");
            Serial.println(gps.satellites.value());
          } else {
            Serial.print("Error on sending POST: ");
            Serial.println(httpResponseCode);
          }

          http.end();
        } else {
          Serial.println("WiFi not connected");
        }
      } else {
        Serial.println("No GPS Signal");
      }
    }
  }

  delay(10000); // Send the coordinates every 10 seconds
}
