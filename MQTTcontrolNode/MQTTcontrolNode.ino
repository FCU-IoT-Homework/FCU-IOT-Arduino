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
