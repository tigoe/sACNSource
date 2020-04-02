/*
  Using a joystick to control pan, tilt, and brightness of an Elation
  Platinum Spot LED Pro II in standard mode.
  Datasheet: http://cdb.s3.amazonaws.com/ItemRelatedFiles/9955/elation_platinum_spot_led_pro_II_user_manual_ver_2.pdf

  DMX channels:
  1 - pan
  2 - tilt
  12 - brightness

  circuit: Adafruit joystick attached to A0 and A1, with button on D5
  (https://www.adafruit.com/products/512)

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
  updated 2 Apr 2020
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

const int buttonPin = 5;
byte pan, tilt, bright = 0;

void setup() {
  Serial.begin(9600);
  //  while you're not connected to a WiFi AP,
  while ( WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(SECRET_SSID);           // print the network name (SSID)
    WiFi.begin(SECRET_SSID, SECRET_PASS);     // try to connect
    delay(2000);
  }
  pinMode(buttonPin, INPUT_PULLUP);

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

  // read the input on analog pin 0:
  int x = analogRead(A0);
  delay(1);
  int y = analogRead(A1);
  int button = !digitalRead(buttonPin);

  // map x and y readings to a 3-point range
  // and subtract 1 to get -1 to 1, with
  // 0 at rest:
  x = map(x, 0, 1023, 0, 3) - 1;
  y = map(y, 0, 1023, 0, 3) - 1;


  if (x != 0) {
    pan += x;
    myController.setChannel(201, pan);
  }
  if (y != 0) {
    tilt += y;
    myController.setChannel(203, tilt);
  }

  if (button != 0) {
    bright += button;
    myController.setChannel(214, bright);
    myController.setChannel(213, 255);
  }

  // if you changed any of those things, you need to send a packet:
  if (x != 0 || y != 0 || button != 0 ) {
    myController.sendPacket(SECRET_SACN_RECV);       // send the data
  }

  // print the results
  Serial.print(x);
  Serial.print("\t");
  Serial.print(y);
  Serial.print("\t");
  Serial.println(button);
  delay(100);                                    // wait .1 second
}
