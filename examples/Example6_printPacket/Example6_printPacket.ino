/*
   PrintPacket

   This sketch creates a sACN packet, then loops over the DMX channels,
   fading each one up and down at a time. It continually prints out the
   packet hexadecimal values, so you can see what each byte value is.
   It does not send the packets out via UDP, however. This sketch
   is for understanding the protocol only. It does not send any packets.

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
//#include <WiFi101.h>    // use this for MKR1000
#include <WiFiNINA.h>       // use this for MKR1010, Nano33 IoT
//#include <ESP8266WiFi.h>  // This should work with the ESP8266 as well.
#include <WiFiUdp.h>
#include <sACNSource.h>
#include "arduino_secrets.h"

WiFiUDP Udp;                                  // instance of UDP library
sACNSource myController(Udp);                 // Your Ethernet-to-DMX device
int myUniverse = 1;                           // DMX universe
char myDevice[] = "myDeviceName";             // sender name
byte dmxChannel = 1;                          // DMX channel level (1-512)

void setup() {
  Serial.begin(9600);
  // initialize controller with a name, 16-byte UUID, and universe number:
  myController.begin(myDevice, SECRET_SACN_UUID, myUniverse);
  // set channel values to 0:
  for (dmxChannel = 1; dmxChannel < 512; dmxChannel++) {
    myController.setChannel(dmxChannel, 0);
  }
}

void loop() {
  for (byte level = 0; level < 256; level++) {    // iterate on the level
    myController.setChannel(dmxChannel, level);   // set the channel with the current level
    Serial.print("Channel: ");                    // print the channel
    Serial.print(dmxChannel);
    Serial.print("  level: ");                    // and the level
    Serial.println(level);
    printPacket();             // print the packet, nicely formatted
    delay(100);
  }
  if (dmxChannel > 512) {   // increment the channel from 1 - 512
    dmxChannel++;
  } else {
    dmxChannel = 1;
  }
}

void printPacket() {
  for (int x = 0; x < myController.packetSize(); x++) {
    if (x % 16 == 0)      // every 16 bytes
      Serial.println();   // add a line break
    Serial.print("0x");   // add hex header
    if (myController.readData(x) < 0x10)  // if there's only one hex charaxter,
      Serial.print("0");                  // add a leading zero
    Serial.print(myController.readData(x), HEX);  // print the byte hex value
    Serial.print(" ");                    // print a space after each byte
  }
  Serial.println();
}
