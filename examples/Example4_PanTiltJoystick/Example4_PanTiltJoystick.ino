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


   You'll also need to add a tab to your sketch called config.h
   for the SSID and password of the network to which you plan to connect,
   as follows:
   char ssid[]     = "network";  // fill in your value
   char password[] = "password"; // fill in your value

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
  myController.begin(myDevice, myUuid, myUniverse);

  // When you're connected, print out the device's network status:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // set DMX channel values to 0:
  for (int dmxChannel = 1; dmxChannel < 513; dmxChannel++) {
    myController.setChannel(dmxChannel, 0);
  }
  myController.sendPacket(receiverAddress);
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
    myController.setChannel(101, pan);
  }
  if (y != 0) {
    tilt += y;
    myController.setChannel(103, tilt);
  }

  if (button != 0) {
    bright += button;
    myController.setChannel(114, bright);
    myController.setChannel(113, 255);
  }

  // if you changed any of those things, you need to send a packet:
  if (x != 0 || y != 0 || button != 0) {
    myController.sendPacket(receiverAddress);       // send the data
  }

  // print the results
  Serial.print(x);
  Serial.print("\t");
  Serial.print(y);
  Serial.print("\t");
  Serial.println(button);
  delay(100);                                    // wait .1 second
}


