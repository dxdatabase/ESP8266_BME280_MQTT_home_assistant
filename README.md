# Module Home Assistant température, humidité, pression sans fil à 10 balles / 10 bucks wireless temperature, humidity, pressure Home Assistant module (MQTT)

## French
Petit script Arduino pour ESP8266 Aliexpress équipé d'un écran OLED. Récupère la température, l'humidité et la pression sur un module capteur BME280.   Affiche la température et l'humidité en haut de l'écran de l'ESP, et l'heure et la date en bas.  Envoi par le wifi la température, l'humidité et la pression via MQTT afin de s'interfacer avec Home Assistant (ou tout système compatible MQTT).   Licence : faites ce que vous voulez !
## English
Simple Arduino script for an ESP8266 (Aliexpress) equipped with an OLED display.
Retrieves temperature, humidity, and pressure from a BME280 sensor module.
Displays temperature and humidity at the top of the ESP’s screen, and the time & date at the bottom. The date is displayed in French format. To use the US format, uncomment line 178 and comment out line 176.  
Sends temperature, humidity, and pressure data over Wi-Fi via MQTT to interface with Home Assistant (or any MQTT-compatible system).  Licence : do what you want!
## Matériel / Hardware :
- BME 280 : https://fr.aliexpress.com/item/32912100752.html (3.3v version)
- ESP 8266 (OLED) : https://fr.aliexpress.com/item/1005006272676085.html 

## Logiciels / Software
- Arduino IDE, NodeMCU 1.0 board (ESP-12E module).
- Libraries :
   - Adafruit Unified Sensor (by Adafruit) for sensor
   - PubSubClient (by Nick O’Leary) for MQTT
   - Adafruit SSD1306 (by Adafruit) for display
## Câblage / Wiring :

![ESP8266_BME280_wiring](https://github.com/user-attachments/assets/0617ca93-58b9-4974-9992-faa74b0b076d)
