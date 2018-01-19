 /*
 *  sACN intro 
 *  
 *  This sketch creates a sACN packet, then uses it to send 
 *  fade information to an sACN receiver. 
 *  This was originally tested with a DMXKing eDMX1 Pro
 *  and a Coemar LEDko Revlection series spot.
 *  https://dmxking.com/artnetsacn/edmx1-pro
 *  http://www.coemar.com/bin/index.php?id=22&serie=2&lng=en&tipologia=1
 *  
 *  Channels used on the LEDko reflection:
 *  1 - brightness
 *  2 - red
 *  3 - green
 *  4 - blue
 *  
 *  You'll also need to add a tab to your sketch called config.h
 *  for the SSID and password of the network to which you plan to connect,
 *  as follows:
 *  char ssid[]     = "network";  // fill in your value
*   char password[] = "password"; // fill in your value
 *  
 *  created 17 Jan 2018
 *  by Tom Igoe
 *  
 */
#include <SPI.h>
#include <WiFi101.h>
//#include <ESP8266WiFi.h>    // This should work with the ESP8266 as well.
#include <WiFiUdp.h>
#include <sACNSource.h>
#include "config.h"

WiFiUDP Udp;                                  // instance of UDP library
sACNSource myController(Udp);                 // Your Ethernet-to-DMX device
char receiverAddress[] = "192.168.0.12";      // sACN receiver address

int myUniverse = 1;                                 // DMX universe
char myDevice[] = "myDeviceName";                   // sender name
char myUuid[] = "130edd1b-2d17-4289-97d8-2bff1f4490fb"; // sender UUID
byte level;                                         // DMX channel level

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
  // set green and blue channels to make a teal hue:
  myController.setChannel(3, 145);
  myController.setChannel(4, 80);
}

void loop() {
  myController.setChannel(1, level);              // set channel 1 (brightness)
  level = level + 10;                             // increment level
  Serial.println(level);                          // print level
  myController.sendPacket(receiverAddress); // send the data
  delay(1000);                                    // wait 1 second
}


