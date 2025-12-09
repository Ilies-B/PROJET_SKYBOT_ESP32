#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>

const char *ssid = "Ilies";
const char *password = "ovvl1205";

const int ledPin = 2;  // LED intégrée à l'ESP32
const int potPin = 34; // Potentiomètre (ADC1_CH6)

String ledState;

AsyncWebServer server(80);

// Remplace l'espace réservé repéré par des % dans index.html
String processor(const String &var)
{
    float mesure = analogRead(potPin);
    Serial.println(var);
    Serial.println(String(mesure));
    return String(mesure);
}

void setup()
{
    Serial.begin(115200);
    pinMode(ledPin, OUTPUT);

    // ----------- LittleFS -----------
    if (!LittleFS.begin())
    {
        Serial.println("Erreur LittleFS...");
        return;
    }

    // ----------- WIFI ---------------
    WiFi.begin(ssid, password);
    Serial.print("Tentative de connexion...");

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(500);
    }

    Serial.println("\nConnexion établie !");
    Serial.print("Adresse IP: ");
    Serial.println(WiFi.localIP());

    // ----------- SERVEUR ------------

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(LittleFS, "/index.html", String(), false, processor); });

    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(LittleFS, "/style.css", "text/css"); });

    server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request)
              {
        digitalWrite(ledPin, HIGH);
        request->send(LittleFS, "/index.html", String(), false, processor); });

    server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request)
              {
        digitalWrite(ledPin, LOW);
        request->send(LittleFS, "/index.html", String(), false, processor); });

    server.begin();
    Serial.println("Serveur démarré !");
}

void loop()
{
    // Vide
}
