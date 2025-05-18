#include <WiFiNINA.h>
#include <PubSubClient.h>
#include "secrets.h" 
#include <Wire.h>
#include <BH1750.h>

// Wi-Fi credentials
const char* WIFI_SSID = SECRET_SSID;
const char* WIFI_PASSWORD = SECRET_PASS; 

// MQTT server details
const char* MQTT_SERVER = SECRET_MQTT_SERVER; 
const int MQTT_PORT = 8883;
const char* MQTT_TOPIC = "sensor/light";

const char* MQTT_USER = SECRET_MQTT_USER;
const char* MQTT_PASSWORD = SECRET_MQTT_PASSWORD;

// Use SSL Client instead of regular WiFiClient
WiFiSSLClient wifiSSLClient;
PubSubClient mqttClient(wifiSSLClient);

// BH1750 sensor instance, 0x5C is the I2C address of the sensor
BH1750 lightMeter(0x5C);

// Function to Connect to MQTT Broker
void connectMQTT() {
   while (!mqttClient.connected()) {
       Serial.println("Connecting to MQTT...");
       if (mqttClient.connect("ArduinoNanoIoT", MQTT_USER, MQTT_PASSWORD)) {
           Serial.println("Connected to MQTT Broker!");
       } else {
           Serial.print("Failed, rc=");
           Serial.print(mqttClient.state());
           Serial.println(" Retrying in 5 seconds...");
           delay(5000);
       }
   }
}


void setup() {
    Serial.begin(115200);
    Wire.begin(A4, A5); // Which points are being used for SDA, SCL
    Wire.setClock(100000);

    // initialize the light detector, as it wasn't working without it
    if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
      Serial.println("BH1750 initialized.");
    } else {
      Serial.println("Error initializing BH1750.");
    }

   // Connect to WiFi
   WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
   while (WiFi.status() != WL_CONNECTED) {
       delay(1000);
       Serial.println("Connecting to WiFi...");
   }
   Serial.println("Connected to WiFi");

   // Set MQTT Server
   mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
  
   // Connect to MQTT
   connectMQTT();
}

// The code the Nano 33 will loop through
void loop() {
  
  if (!mqttClient.connected()){
    connectMQTT();
  }
  mqttClient.loop();
  
    // get the data
    float lux = lightMeter.readLightLevel();

    // print in serial monitor for easier testing
    Serial.print(lux);
    Serial.println();

    // Since publish only works for strings, the lux value has to be changed to one
    char payload[10];
    snprintf(payload, sizeof(payload), "%.2f", lux); // turn to 2dp 

    // publish to MQTT
    mqttClient.publish(MQTT_TOPIC, payload);

  delay(5000); // loop every 5s
}
