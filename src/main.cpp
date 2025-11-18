#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>

const char *ssid = "Ilies";
const char *password = "ovvl1205";
const int led = 2; // Led intégrée à l'ESP32

AsyncWebServer server(80);

void setup()
{

    Serial.begin(115200);
    Serial.println("\n");

    pinMode(led, OUTPUT);
    digitalWrite(led, LOW);

    //-------------------------- LittleFS ---------------------
    if (!LittleFS.begin(true)) // Le paramètre true force un formatage de la partition LittleFS si elle est corrompue.
    {                          // Démarrage du gestionnaire de fichiers LittleFS
        Serial.println("Erreur LittleFS...");
        return;
    }

    // Détection des fichiers présents sur l’ESP32
    File root = LittleFS.open("/");  // Ouverture de la racine
    File file = root.openNextFile(); // Ouverture du 1er fichier

    while (file)
    { // Boucle de test de présence des fichiers - Si plus de fichiers la boucle s'arrête
        Serial.print("File: ");
        Serial.println(file.name());
        file.close();
        file = root.openNextFile(); // Lecture du fichier suivant
    }

    //-------------------------- WIFI ------------------------
    WiFi.begin(ssid, password); // Connexion au WiFi
    Serial.print("Tentative de connexion...");

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(100);
    }

    Serial.println("\nConnexion établie !");
    Serial.print("Adresse IP: ");
    Serial.println(WiFi.localIP());

    //-------------------------- SERVEUR ------------------------
    /* Lorsque le serveur est actif , la page index.html est chargée */
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(LittleFS, "/index.html", "text/html"); });

    // Allumer la LED (lorsque l'on clique sur ON)
    server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request)
              {
        digitalWrite(led, HIGH);
        request->send(LittleFS, "/index.html", "text/html"); });

    // Éteindre la LED (lorsque l'on clique sur OFF)
    server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request)
              {
        digitalWrite(led, LOW);
        request->send(LittleFS, "/index.html", "text/html"); });
    /* On affiche que le serveur est actif */
    server.begin();
    Serial.println("Serveur actif !");
}

void loop()
{
    // Vide
}
