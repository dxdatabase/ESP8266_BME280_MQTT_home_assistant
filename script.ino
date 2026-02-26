//Licence : do what you want with it

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_BME280.h>
#include <Adafruit_SSD1306.h>
#include <time.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

unsigned long lastDisplay = 0;
unsigned long lastMQTT = 0;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Adafruit_BME280 bme;

// WiFi
const char* ssid = "your_ssid";
const char* password = "ssid_password";

// MQTT
const char* mqtt_server = "ip_of_mqtt_server";
const int mqtt_port = 1883;
const char* mqtt_user = "user_mqtt";
const char* mqtt_pass = "pass_mqtt";

WiFiClient espClient;
PubSubClient client(espClient);


void reconnectMQTT() {
  if (client.connected()) return;

  Serial.print("Connexion MQTT...");
  if (client.connect("ESP8266Client", mqtt_user, mqtt_pass)) {
    Serial.println("MQTT OK");
    display.println("MQTT OK");
    display.display();
  } else {
    Serial.println("MQTT ERREUR");
    display.println("MQTT ERREUR");
    display.display();
  }
}



void setup() {
  Serial.begin(115200);
  delay(500);

  // I2C UNIQUE
  Wire.begin(14, 12);   // SDA=D6  SCL=D5
  delay(200);

  // OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Erreur OLED");
    while(1);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.println("Init OK");
  display.display();
  delay(1000);

  // BME280
  if (!bme.begin(0x76) && !bme.begin(0x77)) {
    display.clearDisplay();
    display.println("BME ERROR");
    display.display();
    while(1);
  }

  // WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connexion WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi OK ");
  Serial.print(WiFi.localIP());
  display.println("WIFI OK : ");
  display.print(WiFi.localIP());
  display.display();
  delay(2000);
  long rssi = WiFi.RSSI(); // On récupère la force du signal
  display.println("Signal WIFI: ");
  display.print(rssi);
  display.print(" dBm");
  display.display();
  delay(2000);
  client.setServer(mqtt_server, mqtt_port);

// HEURE
configTime(3600, 0, "pool.ntp.org", "time.nist.gov");  // gmtOffset = 3600s, daylightOffset = 0
// 1er param = offset GMT (3600 = GMT+1)
// 2e param = offset été (3600 = +1h en été)

Serial.println("Synchronisation NTP...");
time_t now = time(nullptr);

while (now < 100000) {   // attendre synchro
  delay(500);
  Serial.print(".");
  now = time(nullptr);
}

Serial.println("\nHeure synchronisée !");
display.println(" Heure synchronisée !");
display.display();
delay(1000);
}

void loop() {

  reconnectMQTT();
  client.loop();

  unsigned long now = millis();

  // ===== RAFRAICHISSEMENT ECRAN TOUTES LES SECONDES =====
  if (millis() - lastDisplay >= 1000) {

lastDisplay = now;

float temp = bme.readTemperature();
float hum = bme.readHumidity();
float press = bme.readPressure() / 100.0F;

display.clearDisplay();

// ===================
// HAUT ECRAN
// ===================

// Température
display.setTextSize(2);
display.setCursor(0,0);
display.printf("%.1f°C", temp);

// Humidité 
display.setTextSize(2);
display.setCursor(70,0);
display.printf("%.0f%%", hum);

// ===================
// HEURE BAS ECRAN
// ===================

// ===== HEURE NTP avec gestion DST =====
time_t nowTime = time(nullptr);
struct tm *timeinfo = localtime(&nowTime);

// Ajustement automatique heure d'été
int heureLocale = timeinfo->tm_hour;
if (timeinfo->tm_isdst > 0) {  // DST actif
  heureLocale += 1;
  if (heureLocale >= 24) heureLocale -= 24;
}

// Formater l'heure et la date
char bufferHeure[10];
sprintf(bufferHeure, "%02d:%02d:%02d", heureLocale, timeinfo->tm_min, timeinfo->tm_sec);

char bufferDate[20];
//FRENCH FORMAT
strftime(bufferDate, sizeof(bufferDate), "%d/%m/%Y", timeinfo);
//US FORMAT
//strftime(bufferDate, sizeof(bufferDate), "%m/%d/%Y", timeinfo);

// Affichage OLED
display.setTextSize(2);
display.setCursor(0, 32);
display.print(bufferHeure);

display.setTextSize(1);
display.setCursor(0, 52);
display.print(bufferDate);

//AFFICHAGE FINAL

display.display();
  }



  // ===== ENVOI MQTT TOUTES LES 5 MINUTES =====

   if (millis() - lastMQTT >= 300000) {
  
    lastMQTT = now;

    float temp = bme.readTemperature();
    float hum = bme.readHumidity();
    float press = bme.readPressure() / 100.0F;

    Serial.println("Envoi MQTT");

    if (client.connected()) {
      String payload = "{\"tempsdb\":" + String(temp) +
                       ",\"humsdb\":" + String(hum) +
                       ",\"presssdb\":" + String(press) + "}";

      client.publish("esp/bme280/sdb", payload.c_str(), true);  // retain activé
    }
  }
}
