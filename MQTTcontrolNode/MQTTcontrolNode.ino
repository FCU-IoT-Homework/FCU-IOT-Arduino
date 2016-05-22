#include <dht.h>
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include "config.h"

#define CHANNEL_LIGHT_0 "light_0"
#define CHANNEL_LIGHT_1 "light_1"
#define CHANNEL_LIGHT_2 "light_2"
#define CHANNEL_DOOR_0  "door_0"
#define CHANNEL_AIR_CONDITIONING_0 "air_conditioning_0"
#define CHANNEL_INFRARED_TRANSMITTER_0 "infrared_transmitter_0"

void subscribeChannel();
void callback(char* topic, byte* payload, unsigned int length);
void printReceiveData(char* topic, byte* payload, unsigned int length);
void controlRelay(const int pin_number, byte* payload);

EthernetClient ethClient;
PubSubClient client(ethClient);

void setup() {
    Serial.begin(9600);
    
    client.setServer(server, 1883);
    client.setCallback(callback);

    if (USE_DHCP) {
        if (Ethernet.begin(mac) == 0) {
            Serial.println("Failed to configure Ethernet using DHCP");
            while (1) ;
        }
    }
    else {
        Ethernet.begin(mac, ip);
    }
    Serial.print("IP: ");
    Serial.println(Ethernet.localIP());
    
    // Allow the hardware to sort itself out
    delay(1500);

    pinMode(PIN_LIGHT_0, OUTPUT);
    pinMode(PIN_LIGHT_1, OUTPUT);
    pinMode(PIN_LIGHT_2, OUTPUT);
    pinMode(PIN_DOOR_0, OUTPUT);
    pinMode(PIN_AIR_CONDITIONING_0, OUTPUT);

    // TODO: PIN_INFRARED_TRANSMITTER_0
    
    digitalWrite(PIN_LIGHT_0, HIGH);
    digitalWrite(PIN_LIGHT_1, HIGH);
    digitalWrite(PIN_LIGHT_2, HIGH);
    digitalWrite(PIN_DOOR_0, HIGH);
    digitalWrite(PIN_AIR_CONDITIONING_0, HIGH);

    // TODO: PIN_INFRARED_TRANSMITTER_0 init
}

void loop() {
    if (client.connected()) {

    }
    else {
        reconnect();
    }
    client.loop();
}

void reconnect() {
    // Loop until we're reconnected
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        if (client.connect("arduinoClient")) {
            Serial.println("connected");
            // resubscribe
            subscribeChannel();
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void subscribeChannel() {
    client.subscribe(CHANNEL_LIGHT_0);
    client.subscribe(CHANNEL_LIGHT_1);
    client.subscribe(CHANNEL_LIGHT_2);
    client.subscribe(CHANNEL_DOOR_0);
    client.subscribe(CHANNEL_AIR_CONDITIONING_0);
    client.subscribe(CHANNEL_INFRARED_TRANSMITTER_0);
}

void callback(char* topic, byte* payload, unsigned int length) {
    //debug
    printReceiveData(topic, payload, length);

    if (strcmp(topic, CHANNEL_LIGHT_0) == 0) {
        controlRelay(PIN_LIGHT_0, payload);
    }
    else if (strcmp(topic, CHANNEL_LIGHT_1) == 0) {
        controlRelay(PIN_LIGHT_1, payload);
    }
    else if (strcmp(topic, CHANNEL_LIGHT_2) == 0) {
        controlRelay(PIN_LIGHT_2, payload);
    }
    else if (strcmp(topic, CHANNEL_DOOR_0) == 0) {
        controlRelay(PIN_DOOR_0, payload);
    }
    else if (strcmp(topic, CHANNEL_AIR_CONDITIONING_0) == 0) {
        controlRelay(PIN_AIR_CONDITIONING_0, payload);
    }
    else if (strcmp(topic, CHANNEL_INFRARED_TRANSMITTER_0) == 0) {
        // TODO
    }
}

void printReceiveData(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i=0;i<length;i++) {
        Serial.print((char)payload[i]);
    }
    Serial.println();
}

void controlRelay(const int pin_number, byte* payload) {
    if (memcmp(payload, "on", 2) == 0) {
        digitalWrite(pin_number, LOW);
    }
    else if (memcmp(payload, "off", 3) == 0) {
        digitalWrite(pin_number, HIGH);
    }
}

