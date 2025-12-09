#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>

const char *ssid = "Ilies";
const char *password = "ovvl1205";

const int ledPin = 2;
String ledState;

AsyncWebServer server(80);

// Mettre l'état du ROBOT dans l'emplacement réservé
String processor(const String &var)
{
    Serial.println(var);

    if (var == "STATE")
    {
        if (digitalRead(ledPin))
        {
            ledState = "MARCHE";
        }
        else
        {
            ledState = "ARRET";
        }

        Serial.print(ledState);
        return ledState;
    }
    return String();
}

void setup()
{
    Serial.begin(115200);
    pinMode(ledPin, OUTPUT);

    // Initialisation de LittleFS
    if (!LittleFS.begin(true))
    {
        Serial.println("Erreur lors du montage de LittleFS");
        return;
    }

    // Connexion au WiFi
    WiFi.begin(ssid, password);
    Serial.print("Connexion au WiFi");

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nConnecté !");
    Serial.print("Adresse IP : ");
    Serial.println(WiFi.localIP());

    // Routes serveur
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
    Serial.println("Serveur actif !");
}

void loop()
{
    // Vide
}
