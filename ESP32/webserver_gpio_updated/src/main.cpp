#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "AsyncTCP.h"
#include "SPIFFS.h"

// Network Credentials
const char *ssid = "FORTI_INTERNET_2G4";
const char *password = "HTLInternet";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String getState(int val)
{
  return "ON";
  /*if (val == 1)
  {
    if (digitalRead(ledPin1))
    {
      return "ON";
    }
    else
    {
      return "OFF";
    }
  }
  else if (val == 2)
  {
    if (digitalRead(ledPin2))
    {
      return "ON";
    }
    else
    {
      return "OFF";
    }
  }*/
}

String processor(const String &var)
{
  //Serial.println(var);
  if (var == "OUTPUT")
  {
    String out = "";
    out += "<tr><td><h3>" + getState(1) + "</h3></td><td><h3>" + getState(2) + "</h3></td></tr>";

    return out;
  }
  return String();
}

void setup()
{

  Serial.begin(115200);

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

  // GET Requests
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/index.html", String(), false, processor); });

  server.on("/on_1", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/index.html", String(), false, processor); });

  server.on("/on_2", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/index.html", String(), false, processor); });

  // Route to load favicon
  server.on("/favicon.png", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/favicon.png", "image/png"); });

  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/style.css", "text/css"); });

  server.begin();
}

void loop()
{
  // put your main code here, to run repeatedly:
}