#include <dht.h>
#include <Ethernet.h>
#include <SPI.h>

byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x01 }; // RESERVED MAC ADDRESS
EthernetClient client;

#define dht_dpin 3 
//#define DHTTYPE DHT11 // SENSOR TYPE - THE ADAFRUIT LIBRARY OFFERS SUPPORT FOR MORE MODELS
dht DHT;

long previousMillis = 0;
unsigned long currentMillis = 0;
long interval = 250000; // READING INTERVAL

int t = 25;	// TEMPERATURE VAR
int h = 30;	// HUMIDITY VAR
String data;

void setup() { 
	Serial.begin(9600);

	if (Ethernet.begin(mac) == 0) {
		Serial.println("Failed to configure Ethernet using DHCP"); 
	}
        Serial.println("I'm ok");
	//dht.begin(); 
	delay(10000); // GIVE THE SENSOR SOME TIME TO START

	h = (int)DHT.humidity; 
	t = (int)DHT.temperature; 

	data = "";
}

void loop(){
  DHT.read11(dht_dpin); 
	currentMillis = millis();
	if(currentMillis - previousMillis > interval) { // READ ONLY ONCE PER INTERVAL
		previousMillis = currentMillis;
    h = DHT.humidity; 
    t = DHT.temperature; 
	}
        data = "temp1=";
        data.concat(t);
        data.concat("&hum1=");
        data.concat(h);

	if (client.connect("140.134.26.72",80)) { // REPLACE WITH YOUR SERVER ADDRESS
		client.println("POST /IoT/Hardware/arduino/add.php HTTP/1.1"); 
		client.println("Host: 140.134.26.72"); // SERVER ADDRESS HERE TOO
		client.println("Content-Type: application/x-www-form-urlencoded"); 
		client.print("Content-Length: "); 
		client.println(data.length()); 
		client.println(); 
		client.print(data); 
	} 
  Serial.println(data);
        //Serial.println("I'm alive! 2");
	if (client.connected()) { 
		client.stop();	// DISCONNECT FROM THE SERVER
	}

	delay(3000); // WAIT FIVE MINUTES BEFORE SENDING AGAIN
}



