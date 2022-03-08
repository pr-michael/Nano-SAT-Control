#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "AsyncTCP.h"
#include "SPIFFS.h"

// Network Credentials
const char *ssid = "FORTI_INTERNET_2G4";
const char *password = "HTLInternet";

// Set LED GPIO pins
const int ledPin1 = 33;
const int ledPin2 = 32;
const int ledPin3 = 25;
const int ledPin4 = 26;

// Stores LED state
String ledState1;
String ledState2;
String ledState3;
String ledState4;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Returns state of led as a String
String getState(int val)
{
  return "ON";
  if (val == 1)
  {
    if (digitalRead(ledPin1))
    {
      return "REVERSE";
    }
    else
    {
      return "FORWARD";
    }
  }
  else if (val == 3)
  {
    if (digitalRead(ledPin3))
    {
      return "REVERSE";
    }
    else
    {
      return "FORWARD";
    }
  }
}

// Executed whith server.on(...)
String processor(const String &var)
{
  if (var == "OUTPUT")
  {
    String out = "";
    out += "<td><h3>" + getState(1) + "</h3></td>";
    out += "<td><h3>" + getState(3) + "</h3></td>";

    return out;
  }
  return String();
}

void setup()
{

  Serial.begin(115200);

  // Set pinmode to output
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);

  // Initialize SPIFFS
  if (!SPIFFS.begin(true))
  {
    Serial.println("An error has occurred while mounting SPIFFS");
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

  // GET-requests

  // GET-request on /
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/index.html", String(), false, processor); });

  // GET-request on link /on_1
  server.on("/on_1", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              digitalWrite(ledPin1, HIGH);
              request->send(SPIFFS, "/index.html", String(), false, processor);
            });

  // GET-request on link /on_2
  server.on("/on_2", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              digitalWrite(ledPin2, HIGH);
              request->send(SPIFFS, "/index.html", String(), false, processor);
            });

  // GET-request on link /off_1
  server.on("/off_1", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              digitalWrite(ledPin1, LOW);
              request->send(SPIFFS, "/index.html", String(), false, processor);
            });

  // GET-request on link /off_2
  server.on("/off_2", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              digitalWrite(ledPin2, LOW);
              request->send(SPIFFS, "/index.html", String(), false, processor);
            });
  
  // GET-request on link /sw1_f
  server.on("/sw1_f", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              digitalWrite(ledPin1, LOW);
              digitalWrite(ledPin2, HIGH);
              request->send(SPIFFS, "/index.html", String(), false, processor);
            });

  // GET-request on link /sw1_r
  server.on("/sw1_r", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              digitalWrite(ledPin2, LOW);
              digitalWrite(ledPin1, HIGH);
              request->send(SPIFFS, "/index.html", String(), false, processor);
            });

  // GET-request on link /sw2_f
  server.on("/sw2_f", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              digitalWrite(ledPin3, LOW);
              digitalWrite(ledPin4, HIGH);
              request->send(SPIFFS, "/index.html", String(), false, processor);
            });

  // GET-request on link /sw2_r
  server.on("/sw2_r", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              digitalWrite(ledPin4, LOW);
              digitalWrite(ledPin3, HIGH);
              request->send(SPIFFS, "/index.html", String(), false, processor);
            });

  // GET-request to favicon
  server.on("/favicon.png", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/favicon.png", "image/png"); });

  // GET-request to stylesheet
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/style.css", "text/css"); });

  server.begin();
}

void loop()
{
  // put your main code here, to run repeatedly:
}