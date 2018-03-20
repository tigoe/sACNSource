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
char receiverAddress[] = "128.122.151.182";      // sACN receiver address

int myUniverse = 1;                                 // DMX universe
char myDevice[] = "myDeviceName";                   // sender name
char myUuid[] = "130edd1b-2d17-4289-97d8-2bff1f4490fb"; // sender UUID
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
  int level[6];

  // iterate over the faders:
  for (int fader = 0; fader < 6; fader++) {
    // read the current level of each fader, conver to a byte:
    level[fader] = analogRead(fader)/4;
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
    myController.sendPacket(receiverAddress); // send the data

  }
}


