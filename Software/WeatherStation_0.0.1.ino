#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

/*
#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10
*/

Adafruit_BME280 bme; // I2C

// Wifi
const char* ssid = "SSID";                    // Change for your environment
const char* password = "PASSWORD";            // Change for your environment

// MQTT
  const char* mqtt_server = "IP_ADDRESS";     // Change for your environment
  const int mqtt_port = 1883;
  char msg[50];

  //MQTT Topics
    const char* mqtt_topic_temp = "weather/temp";
    const char* mqtt_topic_pres_atm = "weather/pres/inHg";
    const char* mqtt_topic_hum = "weather/hum";
    const char* mqtt_topic_pres_debug = "weather/pres/debug";

//BME280 Variables
  unsigned long delayTime = 10000; //take reading every 10 seconds
  float temp, pres, hum, pres_atm, pres_atm1, pres_inHg;
  const float pres_Pa_inHg = 3386.375258f; //[Pa/inHg]
  const float Altitude = 15.24f; //[m]

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
    Serial.begin(115200);
    setup_wifi();
    
    client.setServer(mqtt_server, mqtt_port);
    //client.setCallback(callback);
  
    Serial.println();

    bool status;
    status = bme.begin(0x76);
    /*
    Serial.println(F("BME280 test"));
    // default settings
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
    }*/
    Serial.println();

    delay(100); // let sensor boot up
}


void loop() { 
    if (!client.connected()) {
      reconnect();
    }
    
    printValues();
    client.loop();
    delay(delayTime);
}

void setup_wifi() {
  delay(10);

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("My IP address is: "); Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266-Weather";
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void printValues() {
  char temp_c[8], pres_c[8], hum_c[8];//, pres_atm_c1[8], pres_atm_c2[8];
  // Read values
  temp = bme.readTemperature();
  pres = bme.readPressure();
  hum =  bme.readHumidity();

  // Convert Values
  ConvertTemp();
  ConvertPres();
  CalcDewPoint();

  Serial.println("Sending MQTT messages");
  dtostrf(temp,5,2,temp_c);
  dtostrf(pres_atm1,6,3,pres_c);
  dtostrf(hum,5,2,hum_c);
  client.publish(mqtt_topic_temp,temp_c);
  client.publish(mqtt_topic_pres_atm,pres_c);
  client.publish(mqtt_topic_hum,hum_c);
  
  Serial.print("Temperature = ");
  Serial.print(temp);
  Serial.println(" *F");

  Serial.print("Pressure = ");
  Serial.print(pres_atm1);
  Serial.println(" inHg");

  Serial.print("Humidity = ");
  Serial.print(hum);
  Serial.println("%");

  Serial.println();
}

void ConvertTemp() {
  //Do conversion
    temp = temp*1.8f;
    temp = temp + 32;
}

void ConvertPres() {
  // Normalize pressure to sea level
  pres_atm = pres*pow(1- ((0.0065*Altitude)/(temp + 0.0065*Altitude + 273.15)),-5.5257);
  pres_atm1 = pres_atm / pres_Pa_inHg;
}

void CalcDewPoint() {
  //Do conversion
}

