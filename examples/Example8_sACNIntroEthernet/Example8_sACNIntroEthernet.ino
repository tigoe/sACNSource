/*
   sACN intro with Ethernet

   This sketch creates a sACN packet, then uses it to send
   fade information to an sACN receiver.
   This was originally tested with a DMXKing eDMX1 Pro
   and a DMX fixture with intensity on channels 1-4
   https://dmxking.com/artnetsacn/edmx1-pro

   You'll also need to add a tab to your sketch called arduino_secrets.h
   for the sACH receiver and UUID
   as follows:
   // your sACM receiver's IP address. This is the broadcast default:
   #define SECRET_SACN_RECV "239.255.0.1"  
  // Unique ID of your SACN source. You can generate one from https://uuidgenerator.net
  // or on the command line by typing uuidgen
  #define SECRET_SACN_UUID "CBC0C271-8022-4032-BC6A-69F614C62816"
  
  Uses a fixed IP address as a fallback in case DHCP fails:
   created 3 Mar 2025
   by Tom Igoe

*/
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <sACNSource.h>
#include "arduino_secrets.h"

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed
// on a sticker on the shield
byte mac[] = {
  // 0xA8, 0x61, 0x0A, 0xAE, 0x28, 0xD7
  0xDE, 0xED, 0xCA, 0xFE, 0xF0, 0x0D
};
// ip address in case DHCP fails:
IPAddress ip(192, 168, 0, 10);

EthernetUDP Udp;                   // instance of UDP library
sACNSource myController(Udp);      // Your Ethernet-to-DMX device
int myUniverse = 1;                // DMX universe
char myDevice[] = "myDeviceName";  // sender name

void setup() {
  // You can use Ethernet.init(pin) to configure the CS pin
  //Ethernet.init(10);  // Most Arduino shields
  Ethernet.init(5);  // MKR ETH Shield
  //Ethernet.init(0);   // Teensy 2.0
  //Ethernet.init(20);  // Teensy++ 2.0
  //Ethernet.init(15);  // ESP8266 with Adafruit FeatherWing Ethernet
  //Ethernet.init(33);  // ESP32 with Adafruit FeatherWing Ethernet

  Serial.begin(9600);
  if (!Serial) delay(3000);

  // start the Ethernet connection:
  Serial.println("Initialize Ethernet with DHCP:");
  // try to get an address via DHCP:
  if (!Ethernet.begin(mac)) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // Alternative if you're not using DHCP:
    Ethernet.begin(mac, ip);
  }




  // check if the shield is there:
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true)
      ;
  }
  // see if the Ethernet shield is connected:
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
    while (true)
      ;
  }
  // print your local IP address:
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());
  delay(3000);
  // initialize sACN source:
  myController.begin(myDevice, SECRET_SACN_UUID, myUniverse);

  // set DMX channel values to 0:
  for (int dmxChannel = 0; dmxChannel < 513; dmxChannel++) {
    myController.setChannel(dmxChannel, 0);
  }
  myController.sendPacket(SECRET_SACN_RECV);
}

void loop() {
  // fade up:
  for (int level = 0; level < 256; level += 5) {
    // loop over channels 1-4:
    for (int ch = 0; ch < 5; ch++) {
      myController.setChannel(ch, level);  // set channel
    }
    Serial.println(level);                      // print level
    myController.sendPacket(SECRET_SACN_RECV);  // send the data
    delay(500);                                 // wait .5 second
  }
  delay(1000);
  // fade down:
  for (int level = 255; level >= 0; level -= 5) {
    // loop over channels 1-4:
    for (int ch = 0; ch < 5; ch++) {
      myController.setChannel(ch, level);  // set channel
    }
    Serial.println(level);                      // print level
    myController.sendPacket(SECRET_SACN_RECV);  // send the data
    delay(500);                                 // wait .5 second
  }
  delay(1000);
}
