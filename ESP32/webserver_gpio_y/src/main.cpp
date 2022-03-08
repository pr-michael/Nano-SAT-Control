#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"

// Replace with your network credentials
const char *ssid = "ssid";
const char *password = "passwd";

// Set LED GPIO
const int ledPin1 = 2;
const int ledPin2 = 4;
// Stores LED state
String ledState1;
String ledState2;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Replaces placeholder with LED state value
String processor(const String &var)
{
  if (var == "STATE1")
  {
    if (digitalRead(ledPin1))
    {
      ledState1 = "ON";
    }
    else
    {
      ledState1 = "OFF";
    }
    Serial.print(ledState1 + "\n");
    return ledState1;
  }
  else if (var == "STATE2")
  {
    if (digitalRead(ledPin2))
    {
      ledState2 = "ON";
    }
    else
    {
      ledState2 = "OFF";
    }
    Serial.print(ledState2 + "\n");
    return ledState2;
  }
  return String();
}

void setup()
{
  // Serial port for debugging purposes
  Serial.begin(115200);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);

  // Initialize SPIFFS
  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/index.html", String(), false, processor); });

  // Route to load favicon
  server.on("/favicon.png", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/favicon.png", "image/png"); });

  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/style.css", "text/css"); });

  // Route to set cable1 to HIGH
  server.on("/on1", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              digitalWrite(ledPin1, HIGH);
              request->send(SPIFFS, "/index.html", String(), false, processor);
            });

  // Route to set cable1 to LOW
  server.on("/off1", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              digitalWrite(ledPin1, LOW);
              request->send(SPIFFS, "/index.html", String(), false, processor);
            });

  // Route to set cable2 to HIGH
  server.on("/on2", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              digitalWrite(ledPin2, HIGH);
              request->send(SPIFFS, "/index.html", String(), false, processor);
            });

  // Route to set cable2 to LOW
  server.on("/off2", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              digitalWrite(ledPin2, LOW);
              request->send(SPIFFS, "/index.html", String(), false, processor);
            });

  // Start server
  server.begin();
}

void loop()
{
}