/*
   PrintPacket

   This sketch creates a sACN packet, then loops over the DMX channels,
   fading each one up and down at a time. It continually prints out the
   packet hexadecimal values, so you can see what each byte value is.
   It does not send the packets out via UDP, however. This sketch
   is for understanding the protocol only.

   created 17 Jan 2018
   by Tom Igoe

*/
#include <SPI.h>
#include <WiFi101.h>
//#include <ESP8266WiFi.h>    // This should work with the ESP8266 as well.
#include <WiFiUdp.h>
#include <sACNSource.h>

WiFiUDP Udp;                         // UDP instance just to initialize sACN source
sACNSource myController(Udp);        // Your Ethernet-to-DMX device

int myUniverse = 1;                                     // DMX universe
char myDevice[] = "myDeviceName";                       // sender name
char myUuid[] = "130edd1b-2d17-4289-97d8-2bff1f4490fb"; // sender UUID
byte dmxChannel = 1;                                    // DMX channel level (1-512)

void setup() {
  Serial.begin(9600);
  // initialize controller with a name, 16-byte UUID, and universe number:
  myController.begin(myDevice, myUuid, myUniverse);
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
