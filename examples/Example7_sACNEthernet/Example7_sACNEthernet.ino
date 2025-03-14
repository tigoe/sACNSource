/*
   sACN intro for wired Ethernet

   This sketch creates a sACN packet, then uses it to 
   send fade information to an sACN receiver over wired Ethernet.
   This was originally tested with a DMXKing eDMX1 Pro
   and a DMX fixture with intensity on channel 4
   https://dmxking.com/artnetsacn/edmx1-pro

   You'll also need to add a tab to your sketch called arduino_secrets.h
   for the following:

// your sACN receiver's IP address. 
The broadcast default is 239.255.0.1, but it causes considerable
slowdown for the Ethernet library. A better alternative is the
broadcast address on your own network. So if your sender, e.g.
is 192.168.0.10, then send to 192.168.0.255:
   #define SECRET_SACN_RECV "192.168.0.255"  
  // Unique ID of your SACN source. You can generate one from https://uuidgenerator.net
  // or on the command line by typing uuidgen
  #define SECRET_SACN_UUID "CBC0C271-8022-4032-BC6A-69F614C62816"

   created 1 Mar 2021
   updated 10 Mar 2025
   by Tom Igoe
*/
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <sACNSource.h>
#include "arduino_secrets.h"

// control pins for SPI Chip select of the Ethernet module and SD card:
const int CSPin = 5;
const int SD_CSPin = 4;

// enter a MAC address here. To be safe on an institutional network,
// use an address where the first octet ends in 2, 6, A, or E,
// e.g. 0x 02, 0x86, 0x4A or 0xDE
// this will ensure that it's a locally administered, unicast address.
// if you know someone else will be using this sketch on your LAN,
// don't use the default address below:
byte mac[] = {
  0xA8, 0x61, 0x0A, 0x12, 0x34, 0xAB
};
// IP address if you're setting a fixed address:
IPAddress ip(192, 168, 0, 11);

// An EthernetUDP instance:
EthernetUDP Udp;
// An sACNSource instance using the Ethernet instance:
sACNSource myController(Udp);
// DMX universe:
int myUniverse = 1;
// sender name:
char myDevice[] = "myDeviceName";

void setup() {
  Serial.begin(9600);
  // delay 3 seconds if serial monitor is not open:
  if (!Serial) delay(3000);
  // take pin SD_CS high to disable SD card on ETH shield:
  pinMode(SD_CSPin, OUTPUT);
  digitalWrite(SD_CSPin, HIGH);

  // initialize pin  as ETH shield chip select pin:
  Ethernet.init(CSPin);
  // initialize Ethernet with a fixed IP address:
  Serial.println("initializing Ethernet with fixed address");
  Ethernet.begin(mac, ip);

  // Alternately start the Ethernet connection with DHCP:
  // Serial.println("Initialize Ethernet with DHCP:");
  // try to connect via DHCP:
  // if (Ethernet.begin(mac) == 0) {
  //   Serial.println("Failed to configure Ethernet  using DHCP");
  // }

  // if the Ethernet shield can't be detected:
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.");

    // if the cable's not connected or the port it's plugged into isn't live:
  } else if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
    // no point in carrying on, so do nothing forevermore:
    while (true)
      ;
  }
  // When you're connected, print out the device's network status:
  IPAddress ip = Ethernet.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  // turn on LED to indicate a good link:
  digitalWrite(A0, HIGH);

  // initialize sACN source:
  myController.begin(myDevice, SECRET_SACN_UUID, myUniverse);

  // set DMX channel values to 0:
  for (int dmxChannel = 0; dmxChannel < 513; dmxChannel++) {
    myController.setChannel(dmxChannel, 0);
  }
  myController.sendPacket(SECRET_SACN_RECV);
}

void loop() {
  Ethernet.maintain();

  // fade up:
  for (int level = 0; level < 256; level++) {
    myController.setChannel(1, level);          // set channel 1 (intensity)
    Serial.println(level);                      // print level
    myController.sendPacket(SECRET_SACN_RECV);  // send the data
    delay(100);                                 // wait .1 second
  }
  delay(1000);
  // fade down:
  for (int level = 255; level >= 0; level--) {
    myController.setChannel(1, level);          // set channel 1 (intensity)
    Serial.println(level);                      // print level
    myController.sendPacket(SECRET_SACN_RECV);  // send the data
    delay(100);                                 // wait .1 second
  }
  delay(1000);
}
