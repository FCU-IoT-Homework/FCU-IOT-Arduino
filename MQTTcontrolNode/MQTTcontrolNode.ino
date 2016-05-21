#include <dht.h>
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include "config.h"

void callback(char* topic, byte* payload, unsigned int length);

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
            // client.subscribe("LED0");
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void callback(char* topic, byte* payload, unsigned int length) {
    
}

