/*
   sACN intro

   This sketch creates a sACN packet, then uses it to send
   fade information to an sACN receiver.
   This was originally tested with a DMXKing eDMX1 Pro
   and a DMX fixture with intensity on channel 4
   https://dmxking.com/artnetsacn/edmx1-pro

   You'll also need to add a tab to your sketch called arduino_secrets.h
   for the SSID and password of the network to which you plan to connect,
   as follows:
   #define SECRET_SSID "ssid"  // fill in your value
  #define SECRET_PASS "password" // fill in your value
  #define SECRET_SACN_RECV "192.168.0.14"  // your sACM receiver's IP address
  // Unique ID of your SACN source. You can generate one from https://uuidgenerator.net
  // or on the command line by typing uuidgen
  #define SECRET_SACN_UUID "CBC0C271-8022-4032-BC6A-69F614C62816"

   created 17 Jan 2018
   updated 2 Apr 2020
   by Tom Igoe

*/
#include <SPI.h>
//#include <WiFi101.h>      // use this for MKR1000
#include <WiFiNINA.h>       // use this for MKR1010, Nano33 IoT
//#include <ESP8266WiFi.h>  // This should work with the ESP8266 as well.
#include <WiFiUdp.h>
#include <sACNSource.h>
#include "arduino_secrets.h"

WiFiUDP Udp;                        // instance of UDP library
sACNSource myController(Udp);       // Your Ethernet-to-DMX device
int myUniverse = 1;                 // DMX universe
char myDevice[] = "myDeviceName";   // sender name

long lastSendTime = 0;              // timestamp of last transmission
int interval = 500;                 // sending interval

void setup() {
  Serial.begin(9600);
  //  while you're not connected to a WiFi AP,
  while ( WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(SECRET_SSID);           // print the network name (SSID)
    WiFi.begin(SECRET_SSID, SECRET_PASS);     // try to connect
    delay(2000);
  }
  // initialize sACN source:
  myController.begin(myDevice, SECRET_SACN_UUID, myUniverse);

  // When you're connected, print out the device's network status:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // set DMX channel values to 0:
  for (int dmxChannel = 1; dmxChannel < 513; dmxChannel++) {
    myController.setChannel(dmxChannel, 0);
  }
  myController.sendPacket(SECRET_SACN_RECV);
}

void loop() {

  // read an analog sensor:
  int sensorValue = analogRead(A0);
  // convert to a byte:
  byte level = sensorValue / 4;
  myController.setChannel(4, level);              // set channel intensity

  // if the send time threshold has passed:
  if (millis() - lastSendTime > interval) {
    Serial.println(level);                          // print level
    myController.sendPacket(SECRET_SACN_RECV);      // send the data
    lastSendTime = millis();  // save current timestamp for comparison next time
  }
}
