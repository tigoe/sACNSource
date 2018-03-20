
/*
  Rotary encoder controlling the hue of a DMX-controlled light
  using HSI calculations, with the following channels:
  1 - overall dimmer
  2 - red
  3 - green
  4 - blue
  5 - white

  The color of the light will depend on the angle
  of the encoder relative to its zero position. 0 degrees = red,
  120 degrees = green, 240 degrees = blue.
  The calculation of angle on the encoder is approximate. It assumes
  88 steps per rotation. Pressing the pushbutton resets the encoder
  to zero, and resets the colro wheel to 0 degrees (red).

  Uses Paul Stoffregen's Encoder library:
  https://www.pjrc.com/teensy/td_libs_Encoder.html

  Uses sACNSource library an a MKR1000. Will probably work on an ESP8266,
  but has not been tested.

  Based on HSI conversion from Saiko LED, by Brian Neitner:
  http://blog.saikoled.com/post/44677718712/how-to-convert-from-hsi-to-rgb-white
  http://blog.saikoled.com/post/43693602826/why-every-led-light-should-be-using-hsi

  created 19 Feb. 2018
  by Tom Igoe
*/

#include <Encoder.h>            // encoder library
#include "HSI.h"                // HSI to RGB calculations
#include <SPI.h>                // WiFi101 dependencies
#include <WiFi101.h>
#include <WiFiUdp.h>
#include <sACNSource.h>         // sACN library
#include "arduino_secrets.h"    // your SSID username and password go here

WiFiUDP Udp;                                  // instance of UDP library
sACNSource myController(Udp);                 // Your Ethernet-to-DMX device
char receiverAddress[] = "128.122.151.182";      // sACN receiver address

int myUniverse = 1;                                 // DMX universe
char myDevice[] = "myDeviceName";                   // sender name
char myUuid[] = "130eda1b-2e24-4349-97ae-2bfa1f4390fb"; // sender UUID

const int buttonPin = 4;    // pushbutton pin of the encoder
Encoder thisEncoder(6, 7);  // initialize the encoder on pins 6 and 7
long lastPosition = 0;      // last position of the knob
int lastButtonState = HIGH; // last state of the button

int bright = 255;                 // brightness of the light (Channel 1)
int red, green, blue, white = 0;  // next four channels

void setup() {
  Serial.begin(9600);       // initialize serial communication
  pinMode(buttonPin, INPUT_PULLUP); // pushbutton on pin 4

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
  myController.sendPacket(receiverAddress);
}

void loop() {
  // read the encoder:
  int currentPosition = thisEncoder.read();
  // see if it's changed:
  if (currentPosition != lastPosition) {
    setColors(currentPosition); // set the colors based on the position
    setChannels();              // send the data packet
  }
  // save current position for comparison next time:
  lastPosition = currentPosition;

  // read the pushbutton state:
  int buttonState = digitalRead(buttonPin);
  // see if it's changed:
  if (buttonState != lastButtonState) {
    // if it's currently pressed:
    if (buttonState == LOW) {
      // reset the encoder count:
      thisEncoder.write(0);
    }
  }
  // save current state for comparison next time:
  lastButtonState = buttonState;
}

void setColors(int sensorReading) {
  // calculate the angle of the hue:
  int hue = abs(360 / 88 * sensorReading);
  // constrain it to 0-360:
  hue = constrain(hue, 0, 360);
  // use the angle to calculate RGB:
  unsigned long color = hsiToRgb(hue, 100, 100);
  // hsiToRgb returns a long with 3 bytes, R, G, B, W.
  // separate them out:
  red = (color >> 16) % 256;      // red is the high byte
  green = (color >> 8) % 256;     // green is the middle byte
  blue = color % 256;             // blue is the low byte

  // print the hue and the colors (for diagnostic purposes only):
  Serial.println(hue);
  String colors = String(red);
  colors += " ";
  colors += green;
  colors += " ";
  colors += blue;
  Serial.println(colors);
}

void setChannels() {
  myController.setChannel(1, bright);
  myController.setChannel(2, red);
  myController.setChannel(3, green);
  myController.setChannel(4, blue);
  myController.setChannel(5, white);
  myController.sendPacket(receiverAddress);       // send the data
}

