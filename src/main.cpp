#include <Arduino.h>
#include <Wifi.h>
#include "State.h"
#include "config.h"
#include "Peripherals.h"
#include "Network.h"
#include "telemetry.h"
#include "model.h"
#include "edge_ai.h"
#include "optimization.h"
#include "secrets.h"
// WiFi Configuration
const char* WIFI_SSID = "Wokwi-GUEST";
const char* WIFI_PASS = "";

// MQTT Configuration
const char* MQTT_SERVER = "thingsboard.cloud";
const int MQTT_PORT = 1883;

// ThingsBoard Configuration
const char* BAY_ID = "Bay1";
void setup()
{Serial.begin(115200);
    dht.begin();  // initialise sesnor
    configTime(0,0,"pool.ntp.org","time.nist.gov");
    pinMode(BTN_PLUGIN,INPUT_PULLUP);
    pinMode(BTN_PLUGOUT,INPUT_PULLUP);
    pinMode(RELAY_PIN,OUTPUT);
    pinMode(LED_GREEN,OUTPUT);
    pinMode(LED_YELLOW,OUTPUT);
    pinMode(LED_RED,OUTPUT);
    connectWiFi();
    mqtt.setServer(MQTT_SERVER,MQTT_PORT);
    mqtt.setCallback(mqttCallback);
    connectMQTT();}
unsigned long now;
unsigned long last_print;
void loop()
{
    mqtt.loop();
    //print vals every 2 sec
    now = millis();
    if((now - last_print) > 5000)
    {
        last_print = now;
        sample_sensor();
        runEdgeAIInference();
        runOptimization();
        publishTelemetry();
        runEdgeAIInference();
    }
    plug_status();
    updateLeds();
}