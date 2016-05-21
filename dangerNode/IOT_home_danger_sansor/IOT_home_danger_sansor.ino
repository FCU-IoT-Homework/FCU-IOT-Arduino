#include <Ethernet.h>
#include <SPI.h>

const int FIREpin = A0;
const int GASpin = A1;
const int COpin = A2;
const int IRpin = 2;
int FIREvalue;
int GASvalue;
int COvalue;
int IRvalue;
String data;

byte mac[] = { 0x00, 0xAC, 0xDB, 0xCC, 0x08, 0x01 };
EthernetClient client;

void setup() {
  Serial.begin(9600);
  pinMode(IRpin, INPUT);
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
  }
  Serial.println("I'm ok");
  data = "";
  Serial.println("every thing start here");
}

void loop() {
  checkValue();
  overLimit();
  dataGather();
  sendPackage();
  delay(3000);
}
void checkValue() {
  FIREvalue = analogRead(FIREpin);
  GASvalue = analogRead(GASpin);
  COvalue = analogRead(COpin);
  IRvalue = digitalRead(IRpin);
  if (IRvalue == HIGH) {
    IRvalue = LOW;
  }
  else {
    IRvalue = HIGH;
  }
}
void beep() {
  //nothing for now;
}
void overLimit() {
  //nothing for now;
}
void dataGather() {
  data = "fire=";
  data.concat(FIREvalue);
  data.concat("&gas=");
  data.concat(GASvalue);
  data.concat("&co=");
  data.concat(COvalue);
  data.concat("&zone=");
  data.concat(IRvalue);
}
void sendPackage() {
  if (client.connect("140.134.26.72", 80)) { // REPLACE WITH YOUR SERVER ADDRESS
    client.println("POST /IoT/Hardware/arduino/danger_add.php HTTP/1.1");
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
