#include "SoftwareSerial.h"
String ssid = "12345";
String password = "1234567890";
SoftwareSerial esp(6, 7);// ESP_RX >> 7, ESP_TX >> 6
String server = "140.134.26.72"; // www.example.com
String uri = "/IoT/Hardware/arduino/danger_add.php";// our example is /esppost.php

const int FIREpin = A0;
const int GASpin = A1;
const int COpin = A2;
const int IRpin = 2;
int FIREvalue;
int GASvalue;
int COvalue;
int IRvalue;
String data;

void setup() {
  esp.begin(9600);
  Serial.begin(9600);
  Serial.println("start?");
  reset();
  Serial.println("OK");
  data = "";
  Serial.println("every thing start here");
  connectWifi();
}
void loop () {
  checkValue();
  overLimit();
  dataGather();
  httppost();
  delay(500);
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
