Petit script Arduino pour ESP8266 Aliexpress équipé d'un écran OLED. Récupère la température, l'humidité et la pression sur un module capteur BME280. Affiche la température et l'humidité en haut de l'écran de l'ESP, et l'heure en bas. Envoi par le wifi la température, l'humidité et la pression via MQTT afin de s'interfacer avec Home Assistant.

Simple Arduino script for an ESP8266 (Aliexpress) equipped with an OLED display.
Retrieves temperature, humidity, and pressure from a BME280 sensor module.
Displays temperature and humidity at the top of the ESP’s screen, and the time at the bottom.
Sends temperature, humidity, and pressure data over Wi-Fi via MQTT to interface with Home Assistant.

BME280 : https://fr.aliexpress.com/item/32912100752.html
ESP8266 : https://fr.aliexpress.com/item/1005006272676085.html (3.3v version)

Wiring / câblage :

![ESP8266_BME280_wiring](https://github.com/user-attachments/assets/0617ca93-58b9-4974-9992-faa74b0b076d)
