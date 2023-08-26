# sACN Source Library for Arduino

This library creates an array for sending DMX-512 messages using streaming ACN (sACN). This affords control of many modern stage lighting fixtures over UDP. The library wraps a UDP transport object in the constructor so it can send UDP packets.

The Library uses the Arduino WiFiUDP library. It can work with any boards using a variant of the Arduino WiFi library, including the MKR1000 (WiFi101), Nano 33 IoT and Nano RP2040 Connect (WiFiNINA) or Uno R4 WiFI (WiFiS3), or ESP8266 boards using the ESP8266WiFi library.

For more detail on the sACN protocol, see the [ESTA E.131 protocol document](http://tsp.esta.org/tsp/documents/docs/E1-31-2016.pdf)

## [Commands](#commands) 

### [sACNSource](#sacnsource)

#### Syntax
````
WiFiUDP Udp;                       // instance of UDP library
sACNSource myController(Udp);      // Your Ethernet-to-DMX device
````

#### Parameters
takes a UDP object for transport

#### Returns
void

#### Notes
Initializes the sACN packet.


### [begin](#begin)

#### Syntax
````
myController.begin(myDevice, myUuid, myUniverse);
myController.begin(myDevice, myUuid, myUniverse, thisPort);
````

#### Parameters
`myDevice` - a character string containing the name of the source device

`myUuid` - a  character string containing the UUID of the source device. This must be in ASCII-encoded hexadecimal, but can include dashes as delimiters. See [UUID Generator](https://www.uuidgenerator.net/) for examples.

`myUniverse` - an integer containing the DMX universe you want to control.

`thisPort` - if you want to use a port other than the sACN standard (5568), you can pass in an int with your port number

**Note:** You can change the universe, UUID, and device name after initialization using the appropriate commands for setting them. 

#### Returns
void

#### Notes
Sets the initial parameters for the sACN packet.


### [setUuid](#setuuid)

#### Syntax
````
myController.setUuid(myUuid);
````

#### Parameters
`myUuid` - a  character string containing the UUID of the source device.

#### Returns
void

#### Notes
This must be in ASCII-encoded hexadecimal, but can include dashes as delimiters. See [UUID Generator](https://www.uuidgenerator.net/) for examples.

### [setSourceName](#setsourcename)

#### Syntax
````
myController.setSourceName(myDevice);
````

#### Parameters
`myDevice` - a character array containing the device name of your source. Max. 64 characters.

#### Returns
void

#### Notes

### [setUniverse](#setuniverse)

#### Syntax
````
myController.setUniverse(dmxUniverse);
````

#### Parameters
`dmxUniverse` - an integer containing the DMX universe that you wish to control.

#### Returns
void

#### Notes
The sACN formula for setting the IP address based on the universe number is: `239.255.universeHigh.universeLow`. For example, a unverse number anywhere from 0-255 would be `239.255.0.universeNumber`. For a universe number 256, it would be `239.255.1.0`. For universe 1023 it would be `239.255.3.255`. This comes from [ETC's multicast calculator](https://support.etcconnect.com/ETC/Networking/General/sACN_Multicast_Calculator).

**NOTE:** the library currently does not recalculate the multicast IP based on the universe number. 

### [setChannel](#setchannel)
#### Syntax
````
myController.setChannel(dmxChannel, level);
````

#### Parameters
`dmxChannel` - an integer containing the DMX channel you want to set (1-512)

`level` - a byte containing the level to which you want to set the channel ( 0 - 255)

#### Returns
void

#### Notes
Note that DMX channels are usually 1-indexed, rather than 0-indexed. So the first channel is channel 1.

### [sendPacket](#sendpacket)
#### Syntax
````
myController.sendPacket(receiverAddress);
myController.sendPacket(receiverAddress, thisPort);
````

#### Parameters
`receiverAddress` - a character array containing the numeric IP address of the receiver

`thisPort` - if you want to use a port other than the sACN standard (5568), you can pass in an int with your port number

#### Returns
void

#### Notes
This function auto-increments the sequence number for packets, so you don't have to.

### [setSequenceNumber](#setsequencenumber)
#### Syntax
````
myController.setSequenceNumber(seqNumber);
````

#### Parameters
`seqNumber` - a byte containing the packet sequence number.

#### Returns
void

#### Notes
sACN packets are given a packet sequence number so that the receiver can keep the sequence of packets from a given sender. The _sendPacket_ function automatically increments the sequence number, so there's no real need to set this unless you want to set a specific number.

### [getSequenceNumber](#getsequencenumber)

#### Syntax
````
byte num = myController.getSequenceNumber();
````

#### Parameters
none

#### Returns
a byte containing the latest packet sequence number.

#### Notes

### [setSyncAddress](#setsyncaddress)
#### Syntax
````
myController.setSyncAddress(syncAddress);
````

#### Parameters
`syncAddress` - an int containing the sync universe address. Default is 0.

#### Returns
void

#### Notes

### [setOptions](#setoptions)
#### Syntax
````
myController.setOptions(options);
````

#### Parameters
a byte containing the options flags for preview data, stream termination, and forced synchronization.

#### Returns
void

#### Notes
Not implemented in version 0.0.2

### [readData](#readdata)
#### Syntax
````
myController.readData(byteNumber);
````

#### Parameters
`byteNumber` - a byte containing the number of the byte in the packet that you want to read.

#### Returns
a byte variable containing the value of the byte you requested

#### Notes

### [packetSize](#packetsize)
#### Syntax
````
myController.packetSize();
````

#### Parameters
none

#### Returns
an int variable containing the size of the packet.

#### Notes
Unless modified, this will always be 638.
