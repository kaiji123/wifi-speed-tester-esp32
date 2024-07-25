#include "WiFi.h"
#include "HTTPClient.h"

const char* file_url = "http://ipv4.download.thinkbroadband.com/1MB.zip"; // Replace with the URL of a file to download

void setup() {
  Serial.begin(115200);
  Serial.print("Connecting to WiFi");
  WiFi.begin("Wokwi-GUEST", "", 6);


  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }

  Serial.println("Connected!");
}

void loop() {
  HTTPClient http;
  http.begin(file_url);

  Serial.println("Starting download...");
  unsigned long startTime = millis();
  int httpCode = http.GET();
  unsigned long endTime = millis();

  if (httpCode > 0) {
    if (httpCode == HTTP_CODE_OK) {
      int len = http.getSize();
      WiFiClient* stream = http.getStreamPtr();

      int bytesRead = 0;
      uint8_t buffer[128] = { 0 };

      while (http.connected() && (len > 0 || len == -1)) {
        size_t size = stream->available();
        if (size) {
          int c = stream->readBytes(buffer, ((size > sizeof(buffer)) ? sizeof(buffer) : size));
          bytesRead += c;
          if (len > 0) {
            len -= c;
          }
        }
        delay(1);
      }

      Serial.print("Download completed in ");
      Serial.print((endTime - startTime) / 1000.0);
      Serial.println(" seconds");

      Serial.print("Total bytes read: ");
      Serial.println(bytesRead);

      float speed = (bytesRead / 1024.0) / ((endTime - startTime) / 1000.0); // Speed in KBps
      Serial.print("Download speed: ");
      Serial.print(speed);
      Serial.println(" KB/s");
    }
  } else {
    Serial.print("HTTP error: ");
    Serial.println(httpCode);
  }

  http.end();

  delay(10000); // Wait 10 seconds before the next test
}
