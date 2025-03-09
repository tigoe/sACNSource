/*
   sACN intro for wired Ethernet

   This sketch creates a sACN packet, then uses it to 
   send fade information to an sACN receiver over wired Ethernet.
   This was originally tested with a DMXKing eDMX1 Pro
   and a DMX fixture with intensity on channel 4
   https://dmxking.com/artnetsacn/edmx1-pro

   You'll also need to add a tab to your sketch called arduino_secrets.h
   for the following:

// your sACM receiver's IP address. This is the broadcast default:
   #define SECRET_SACN_RECV "239.255.0.1"  
  // Unique ID of your SACN source. You can generate one from https://uuidgenerator.net
  // or on the command line by typing uuidgen
  #define SECRET_SACN_UUID "CBC0C271-8022-4032-BC6A-69F614C62816"

   created 1 Mar 2021
   updated 8 Mar 2025
   by Tom Igoe
*/
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <sACNSource.h>
#include "arduino_secrets.h"

// enter a MAC address here. To be safe on an institutional network,
// use an address where the first octet ends in 2, 6, A, or E,
// e.g. 0x 02, 0x86, 0x4A or 0xDE
// this will ensure that it's a locally administered, unicast address.
// if you know someone else will be using this sketch on your LAN,
// don't use the default address below:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

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
  // take pin 4 high to disable SD card on ETH shield:
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  pinMode(A0, OUTPUT);
  // initialize pin  as ETH shield chip select pin:
  //Ethernet.init(10);  // Most Arduino shields
  Ethernet.init(5);  // MKR ETH Shield
  //Ethernet.init(0);   // Teensy 2.0
  //Ethernet.init(20);  // Teensy++ 2.0
  //Ethernet.init(15);  // ESP8266 with Adafruit FeatherWing Ethernet
  //Ethernet.init(33);  // ESP32 with Adafruit FeatherWing Ethernet
 
   // start the Ethernet connection:
  Serial.println("Initialize Ethernet with DHCP:");
  // try to connect via DHCP:
  if (Ethernet.begin(mac) == 0) { 
    Serial.print("Failed to configure Ethernet  using DHCP");
    Serial.print("Trying fixed address");
    // Alternative if you're not using DHCP:
    Ethernet.begin(mac, ip);
  }

    // if the Ethernet shield can't be detected:
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.");
      
    // if the cable's not connected or the port it's plugged into isn't live:
    } else if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
      
    }
    // no point in carrying on, so do nothing forevermore:
    while (true);
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
    myController.setChannel(1, level);              // set channel 1 (intensity)
    Serial.println(level);                          // print level
    myController.sendPacket(SECRET_SACN_RECV);       // send the data
    delay(100);                                    // wait .1 second
  }
  delay(1000);
  // fade down:
  for (int level = 255; level >= 0; level--) {
    myController.setChannel(1, level);              // set channel 1 (intensity)
    Serial.println(level);                          // print level
    myController.sendPacket(SECRET_SACN_RECV);      // send the data
    delay(100);                                    // wait .1 second
  }
  delay(1000);
}
