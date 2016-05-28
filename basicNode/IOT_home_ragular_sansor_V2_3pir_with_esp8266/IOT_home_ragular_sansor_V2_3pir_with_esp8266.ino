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
#include "SoftwareSerial.h"
#include <dht.h>

#define dht_dpin 2
#define pir1_pin 3
#define pir2_pin 4
#define pir3_pin 5
#define soil_pin A0

String ssid = "12345";
String password = "1234567890";
SoftwareSerial esp(6, 7);// ESP_RX >> 7, ESP_TX >> 6
String server = "140.134.26.72"; // www.example.com
String uri = "/IoT/Hardware/arduino/add.php";// our example is /esppost.php
String pirUri = "/IoT/Hardware/arduino/infrared_add.php";

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
String data , pirData;

void setup() {
  esp.begin(9600);
  Serial.begin(9600);
  Serial.println("start?");
  pinMode(pir1_pin, INPUT);
  pinMode(pir2_pin, INPUT);
  pinMode(pir3_pin, INPUT);
  reset();
  Serial.println("OK");
  data = "";
  pirData = "";
  Serial.println("every thing start here");
  connectWifi();
}
void loop () {
  Serial.println("i servive!");
  temp();
  hum();
  soil();
  dataGather();
  httppost();
  Serial.print("//==========//");
  Serial.print(data);
  Serial.print("//==========//");
  for (i = 0; i < 60; i++) {
    PIRcheck();
    pirDataGather();
    httppostPir();
    Serial.print("//==========//");
    Serial.print(pirData);
    Serial.print("//==========//");
    delay(1000);
  }
}
//reset the esp8266 module
void reset() {
  esp.println("AT+RST");
  delay(1000);
  if (esp.find("OK") ) Serial.println("Module Reset");
}
void connectWifi() {
  String cmd = "AT+CWJAP=\"" + ssid + "\",\"" + password + "\"";
  esp.println(cmd);
  delay(4000);
  if (esp.find("OK")) {
    Serial.println("Connected!");
  }
  else {
    connectWifi();
    Serial.println("Cannot connect to wifi");
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
void httppost () {
  esp.println("AT+CIPSTART=\"TCP\",\"" + server + "\",80");//start a TCP connection.
  if ( esp.find("OK")) {
    Serial.println("TCP connection ready");
  } delay(1000);
  String postRequest =
    "POST " + uri + " HTTP/1.0\r\n" +
    "Host: " + server + "\r\n" +
    "Accept: *" + "/" + "*\r\n" +
    "Content-Length: " + data.length() + "\r\n" +
    "Content-Type: application/x-www-form-urlencoded\r\n" +
    "\r\n" + data;
  String sendCmd = "AT+CIPSEND=";//determine the number of caracters to be sent.
  esp.print(sendCmd);
  esp.println(postRequest.length() );
  delay(500);
  if (esp.find(">")) {
    Serial.println("Sending.."); esp.print(postRequest);
    if ( esp.find("SEND OK")) {
      Serial.println("Packet sent");
      while (esp.available()) {
        String tmpResp = esp.readString();
        Serial.println(tmpResp);
      }
      // close the connection
      esp.println("AT+CIPCLOSE");
    }
  }
}
void  httppostPir () {
  esp.println("AT+CIPSTART=\"TCP\",\"" + server + "\",80");//start a TCP connection.
  if ( esp.find("OK")) {
    Serial.println("TCP connection ready");
  } delay(1000);
  String postRequest =
    "POST " + pirUri + " HTTP/1.0\r\n" +
    "Host: " + server + "\r\n" +
    "Accept: *" + "/" + "*\r\n" +
    "Content-Length: " + pirData.length() + "\r\n" +
    "Content-Type: application/x-www-form-urlencoded\r\n" +
    "\r\n" + pirData;
  String sendCmd = "AT+CIPSEND=";//determine the number of caracters to be sent.
  esp.print(sendCmd);
  esp.println(postRequest.length() );
  delay(500);
  if (esp.find(">")) {
    Serial.println("Sending.."); esp.print(postRequest);
    if ( esp.find("SEND OK")) {
      Serial.println("Packet sent");
      while (esp.available()) {
        String tmpResp = esp.readString();
        Serial.println(tmpResp);
      }
      // close the connection
      esp.println("AT+CIPCLOSE");
    }
  }
}
