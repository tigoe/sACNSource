/*
  Allows an Arduino to send DMX-512 using sACN (E1.31) protocol.
  Does not define transport protocol, though sACN is generally carried
  over UDP.

  For protocol definition, see http://tsp.esta.org/tsp/documents/docs/E1-31-2016.pdf

*/
#ifndef sACNSource_h
#define sACNSource_h

#include "Arduino.h"
#include <Udp.h>

#define SACN_DEVICENAME "mysACNDevice"
#define SACN_UUID "0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a"
#define SACN_UNIVERSE 1
#define SACN_PACKETLENGTH 638
#define SACN_UDP_PORT 5568
class sACNSource
{
  public:
    sACNSource(UDP& udp);                          // constructor with UDP
    void begin(const char* myDevice, const char* myUuid, int myUniverse, int thisPort=SACN_UDP_PORT); // initialize data packet
    void setUuid(const char* myUuid);               // set source ID
    void setSourceName(const char* myDevice);       // set source name
    void setUniverse(int myUniverse);               // set universe number
    void setChannel(int channelNum, byte level);    // set individual DMX Channels
    void setSequenceNumber(byte seqNum);            // set packet sequence number
    byte getSequenceNumber();                       // get packet sequence number
    void setSyncAddress(int syncAddress);           // set sync address
    void setOptions(byte options);                  // set options flags
    byte readData(int b);                           // read packet data
    int packetSize();                              // packet size
    void sendPacket(const char* addr, int port=SACN_UDP_PORT);        // send the packet via UDP

  private:
    UDP*  _udp;                                   // reference to transport
    int _myUniverse;                              // DMX universe
    String _myDevice;                              // sender name
    String _myUuid;                                // sender UUID
    byte _sequenceNumber;                          // packet sequence number
    byte _byteArray[SACN_PACKETLENGTH];            // establish byte array
};

#endif
