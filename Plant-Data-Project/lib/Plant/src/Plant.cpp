#include "Arduino.h"
#include "ESP8266WiFi.h"
#include "DallasTemperature.h"
#include "ArduinoJson.h"
#include "PubSubClient.h"
#include "OneWire.h"
#include "Plant.h"

#define ONE_WIRE_BUS 12
#define TEMP_RESOLUTION 12
#define TEMP_SENSOR_INDEX 0
#define MOISTURE_SENSOR_PIN A0

Plant::Plant() //: oneWire(ONE_WIRE_BUS), temp_sensor(&oneWire)
{
}

Plant::Plant(char *name, char *ssid, char *pass, char *mqtt_broker, int mqtt_port) //: oneWire(ONE_WIRE_BUS), temp_sensor(&oneWire)
{
    _name = name;
    _ssid = ssid;
    _pass = pass;
    _mqtt_broker = mqtt_broker;
    _mqtt_port = mqtt_port;
    // setup();
}

Plant::Plant(char *name, char *ssid, char *pass, char *mqtt_broker, int mqtt_port, IPAddress ip_address, IPAddress subnet) //: oneWire(ONE_WIRE_BUS), temp_sensor(&oneWire)
{
    _name = name;
    _ssid = ssid;
    _pass = pass;
    _subnet = subnet;
    _device_ip = ip_address;
    _usingCustomNetwork = true;
    // setup();
}

void Plant::setup()
{
    Serial.println("*********************************************************\n");
    setWiFi();
    setTempSensor();
    setMQTT();
    // setMoistSensor();
    if (!_wifi_success)
    {
        Serial.println("> Error: Unable To connect to network.");
        Serial.println(WiFi.status());
        // setWiFi();
    }
    if (!_temp_sensor_success)
    {
        Serial.println("> Error: No temperature sensor(s) found");
    }
    Serial.println("*********************************************************\n");
}

String Plant::getName()
{
    return _name;
}

void Plant::sendPayload(String name, String sensor, float value)
{
    StaticJsonDocument<256> doc;
    doc["sensor"] = sensor;
    doc["time"] = 1351824120;
    doc["data"] = value;
    // Generate the minified JSON and send it to the Serial port.
    char out[128];
    serializeJson(doc, out);

    String topic = name + sensor;

    mqttClient->publish(topic.c_str(), out); // publishes to the specific sensor topic
    // The above line prints:
    // {"sensor":"gps","time":1351824120,"data":[48.756080,2.302038]}
    doc.clear();
}

float Plant::getTemperature_F()
{
    if (&temp_sensor != NULL){
        temp_sensor->requestTemperatures(); // Send the command to get temperatures
        return temp_sensor->getTempFByIndex(TEMP_SENSOR_INDEX);
    }
    return -1;
}

float Plant::getTemperature_C()
{
    temp_sensor->requestTemperatures(); // Send the command to get temperatures
    return temp_sensor->getTempCByIndex(TEMP_SENSOR_INDEX);
}

int Plant::getSoilMoisture()
{
    return analogRead(MOISTURE_SENSOR_PIN);
}

IPAddress Plant::getDeviceAddress()
{
    return _device_ip;
}

void Plant::setDeviceAddress()
{
    _device_ip = WiFi.localIP();
}

void Plant::setWiFi(void)
{
    Serial.println("********* Connecting to ");
    Serial.print(_ssid);
    Serial.println(" network *********");

    WiFi.begin(_ssid, _pass);
    if (_usingCustomNetwork)
    {
        Serial.println("> Configuring custom network...");
        WiFi.config(_device_ip, _gateway, _subnet);
    }

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.println(".");
        delay(1000);
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("> Arduino connected to Wifi");
        setDeviceAddress();
        _wifi_success = true;
    }
}

void Plant::setTempSensor()
{
    Serial.println("********* Starting Dallas Temperature Sensor *********");
    // Start up the library
    oneWire = new OneWire(ONE_WIRE_BUS);
    temp_sensor = new DallasTemperature(oneWire);
    temp_sensor->begin();
    temp_sensor->setResolution(TEMP_RESOLUTION);

    // store device information
    const int connectedDevices = temp_sensor->getDeviceCount();
    Serial.println("> Device(s) info:");
    Serial.print("Number of Connected Devices: ");
    Serial.println(connectedDevices);

    if (connectedDevices > 0)
    {
        _temp_sensor_success = true;
    }
}

void Plant::setMQTT()
{
    Serial.println("********* Setting up MQTT Broker Connection *********");
    wifiClient = new WiFiClient();
    mqttClient = new PubSubClient(*wifiClient);

    Serial.println("> MQTT Broker Config:");
    Serial.print("> Broker IP: ");
    Serial.println(_mqtt_broker);
    Serial.print("> Broker Port: ");
    Serial.println(_mqtt_port);

    mqttClient->setServer(_mqtt_broker, _mqtt_port);
    mqttClient->connect(_name.c_str());
    Serial.println("> Connecting to MQTT Broker");

    int RECONNECT_COUNT = 25;
    while (!mqttClient->connected() && RECONNECT_COUNT > 0)
    {
        Serial.println(".");
        delay(1000);
        RECONNECT_COUNT--;
    }

    if (RECONNECT_COUNT == 0)
    {
        Serial.println("> Unable to connect to MQTT Broker");
    }
    else
    {
        Serial.println("> Successfully connected to MQTT Broker");
    }
}