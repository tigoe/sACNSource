/*
   Fading six DMX channels with six potentiometers

  circuit: six potentiometers attached to pins A0 through A5

   This was originally tested with a DMXKing eDMX1 Pro
   and a DMX fixture with six channels
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

  created 19 Feb 2018
  by Tom Igoe
*/
#include <SPI.h>
//#include <WiFi101.h>      // use this for MKR1000
#include <WiFiNINA.h>       // use this for MKR1010, Nano33 IoT
//#include <ESP8266WiFi.h>    // This should work with the ESP8266 as well.
#include <WiFiUdp.h>
#include <sACNSource.h>
#include "arduino_secrets.h"

WiFiUDP Udp;                                  // instance of UDP library
sACNSource myController(Udp);                 // Your Ethernet-to-DMX device

int myUniverse = 1;                                 // DMX universe
char myDevice[] = "myDeviceName";                   // sender name

byte channels[] = {201, 203, 210, 213, 214, 217};    // 6 random DMX channels to control

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
}

void loop() {
  // an array for current fader levels:
  int level[6];

  // iterate over the faders:
  for (int fader = 0; fader < 6; fader++) {
    // read the current level of each fader, conver to a byte:
    level[fader] = analogRead(fader) / 4;
    // set the channel appropriate to the level:
    myController.setChannel(channels[fader], level[fader]);
    // print the current level:
    Serial.print(fader);
    Serial.print(": ");
    Serial.print(level[fader] / 4);
    Serial.print("\t");
  }
  Serial.println();

  // every 500 milliseconds, send a sACN packet:
  if (millis() % 500 < 10) {
    myController.sendPacket(SECRET_SACN_RECV); // send the data
  }
}
