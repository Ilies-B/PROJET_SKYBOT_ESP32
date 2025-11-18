#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <LittleFS.h>
#include <WiFi.h>
const char *ssid = "Ilies";
const char *password = "ovvl1205";
const int led = 2; // Led intégrée à l'ESP32

AsyncWebServer server(80);
void setup()
{

    Serial.begin(115200);
    pinMode(led, OUTPUT);
    digitalWrite(led, LOW);

    //---------------------------LittleFS-------------------
    if (!LittleFS.begin()) /* Démarrage du gestionnaire de fichiers LittleFS */
    {
        Serial.println("Erreur LittleFS...");
        return;
    }

    /* Détection des fichiers présents sur l'Esp32 */
    File root = LittleFS.open("/");  /* Ouverture de la racine */
    File file = root.openNextFile(); /* Ouverture du 1er fichier */
    while (file)                     /* Boucle de test de présence des fichiers - Si plus de fichiers la boucle s'arrête*/
    {
        Serial.print("File: ");
        Serial.println(file.name());
        file.close();
        file = root.openNextFile(); /* Lecture du fichier suivant */
    }

    //-----------------------WIFI-----------------------------
    WiFi.begin(ssid, password); /* Connexion au réseau Wifi */
    Serial.print("Tentative de connexion...");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(100);
    }

    Serial.println("\n");
    Serial.println("Connexion etablie!");
    Serial.print("Adresse IP: ");
    Serial.println(WiFi.localIP());

    //--------------------------SERVEUR--------------------------
    /* Lorsque le serveur est actif , la page index.html est chargée */
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(LittleFS, "/style.css", "text/css"); });
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(LittleFS, "/index.html", "text/html"); });

    /* Lorsque l'on clique sur ON, on allume la led */
    server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request)
              {     
    digitalWrite(led, HIGH);     
    request->send(LittleFS, "/index.html", "text/html"); });
    /* Lorsque l'on clique sur OFF, on éteint la led */
    server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request)
              {     
    digitalWrite(led, LOW);     
    request->send(LittleFS, "/index.html", "text/html"); });
    /* On affiche que le serveur est actif */
    server.begin();
    Serial.println("Serveur actif!");
}
void loop() /* La loop est vide */
{
}
