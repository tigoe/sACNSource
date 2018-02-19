/*
   Fading six DMX channels with six potentiometers

  circuit: six potentiometers attached to pins A0 through A5

   You'll also need to add a tab to your sketch called arduino_secrets.h
   for the SSID and password of the network to which you plan to connect,
   as follows:
   #define SECRET_SSID "ssid"  // fill in your value
   #define SECRET_PASS "password" // fill in your value

  created 19 Feb 2018
  by Tom Igoe
*/
#include <SPI.h>
#include <WiFi101.h>
#include <WiFiUdp.h>
#include <sACNSource.h>
#include "arduino_secrets.h"

WiFiUDP Udp;                                  // instance of UDP library
sACNSource myController(Udp);                 // Your Ethernet-to-DMX device
char receiverAddress[] = "192.168.0.12";      // sACN receiver address

int myUniverse = 1;                                 // DMX universe
char myDevice[] = "myDeviceName";                   // sender name
char myUuid[] = "130edd1b-2d17-4289-97d8-2bff1f4490fb"; // sender UUID

int previousLevel[6];
int threshold = 2;
bool packetChanged = false;     // send a packet only when this is true

void setup() {
  Serial.begin(9600);
  //  while you're not connected to a WiFi AP,
  while ( WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);           // print the network name (SSID)
    WiFi.begin(ssid, password);     // try to connect
    delay(2000);
  }
  // initialize sACN source:
  myController.begin(myDevice, myUuid, myUniverse);

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
  int currentLevel[6];

  // iterate over the faders:
  for (int fader = 0; fader < 6; fader++) {
    // read the current level of each fader:
    currentLevel[fader] = analogRead(fader);
    // if the current is different from the previous by more than the threshold:
    if (abs(currentLevel[fader] - previousLevel[fader]) > threshold) {
      // send the new reading:
      myController.setChannel(fader, currentLevel[fader]);
      // print the current level:
      Serial.println(fader);
      Serial.print(": ");
      Serial.println(currentLevel[fader]);
      // something changed in the packet, so you'll need to send:
      packetChanged = true;
    }
    // save current level for next time through:
    previousLevel[fader] = currentLevel[fader];
  }
  Serial.println();
  
  if (packetChanged) {
    myController.sendPacket(receiverAddress); // send the data
    packetChanged = false;
  }
}


