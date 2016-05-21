/*
  soil mositure
  0~300 > dry
  300~700 > humid soil
  700~950 > in water

  s >> soil mositure
  h >> humidty
  t >> tempure
  p >> pir
*/
#include <Ethernet.h>
#include <SPI.h>
#include <dht.h>

#define dht_dpin 2
#define pir1_pin 3
#define pir2_pin 6
#define pir3_pin 5
#define soil_pin A0

byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x01 };
EthernetClient client;

dht DHT;
int sensorValue1 = 0;
int sensorValue2 = 0;
int sensorValue3 = 0;
int h = 0;
int t = 0;
int s = 0;
int p1 = 0;
int p2 = 0;
int p3 = 0;
int i = 0;

long previousMillis = 0;
unsigned long currentMillis = 0;
long interval = 250000; // READING INTERVAL
String data , pirData;

void setup() {
  Serial.begin(9600);
  pinMode(pir1_pin, INPUT);
  pinMode(pir2_pin, INPUT);
  pinMode(pir3_pin, INPUT);
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
  }
  Serial.println("I'm ok");
  delay(1000);
  data = "";
  pirData = "";
  Serial.println("every thing start here");
}
void loop() {
  temp();
  hum();
  soil();
  dataGather();
  sendPackage();
  for (i = 0; i < 12; i++) {
    PIRcheck();
    pirDataGather();
    sendPackagePir();
    delay(5000);
  }
}

void temp() {
  DHT.read11(dht_dpin);
  t = DHT.temperature;
}
void hum() {
  DHT.read11(dht_dpin);
  h = DHT.humidity;
}
void soil() {
  s = analogRead(soil_pin);
}
void PIRcheck() {
  sensorValue1 = digitalRead(pir1_pin);
  if (sensorValue1 == HIGH) {
    p1 = 1;// 有人
  }
  else {
    p1 = 0;// 沒人
  }
  sensorValue2 = digitalRead(pir2_pin);
  if (sensorValue2 == HIGH) {
    p2 = 1;// 有人
  }
  else {
    p2 = 0;// 沒人
  }
  sensorValue3 = digitalRead(pir3_pin);
  if (sensorValue3 == HIGH) {
    p3 = 1;// 有人
  }
  else {
    p3 = 0;// 沒人
  }
}
void dataGather() {
  data = "temp1=";
  data.concat(t);
  data.concat("&hum1=");
  data.concat(h);
  data.concat("&soil=");
  data.concat(s);
}
void pirDataGather() {
  pirData = "pir1=";
  pirData.concat(p1);
  pirData.concat("&pir2=");
  pirData.concat(p2);
  pirData.concat("&pir3=");
  pirData.concat(p3);
}

void sendPackage() {
  if (client.connect("140.134.26.72", 80)) { // REPLACE WITH YOUR SERVER ADDRESS
    client.println("POST /IoT/Hardware/arduino/add.php HTTP/1.1");
    client.println("Host: 140.134.26.72"); // SERVER ADDRESS HERE TOO
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.println(data.length());
    client.println();
    client.print(data);
  }
  Serial.println(data);
  if (client.connected()) {
    client.stop();  // DISCONNECT FROM THE SERVER
  }
}
void sendPackagePir() {
  if (client.connect("140.134.26.72", 80)) { // REPLACE WITH YOUR SERVER ADDRESS
    client.println("POST /IoT/Hardware/arduino/infrared_add.php HTTP/1.1");
    client.println("Host: 140.134.26.72"); // SERVER ADDRESS HERE TOO
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.println(pirData.length());
    client.println();
    client.print(pirData);
  }
  Serial.println(pirData);
  if (client.connected()) {
    client.stop();  // DISCONNECT FROM THE SERVER
  }
}
