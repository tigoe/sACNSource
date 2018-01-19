#include "Arduino.h"
#include "sACNSource.h"

sACNSource::sACNSource(UDP& udp) {
  this->_udp = &udp;                           // initialize UDP
  _myUniverse = SACN_UNIVERSE;                 // DMX universe
  _byteArray[SACN_PACKETLENGTH];               // establish byte array
  _myDevice = SACN_DEVICENAME;                 // sender name
  _myUuid = SACN_UUID;                         // sender UUID
  _sequenceNumber = 0;                         // packet sequence number
}

void sACNSource::begin(const char* myDevice, const char* myUuid, int myUniverse, int thisPort) {
  setUniverse(myUniverse);
  setSourceName(myDevice);
  setUuid(myUuid);
  // call udp.begin():
  this->_udp->begin(thisPort);
  _sequenceNumber = 0;                      // packet sequence number

   // fill in the pre-defined values in the packet:
   // Packet opening: 0x00, 0x10, 0x00, 0x00 + 'ASC-E1.17' + 0x00, 0x00, 0x00
   byte header[] = {0x00, 0x10, 0x00, 0x00, 0x41, 0x53, 0x43, 0x2d, 0x45, 0x31, 0x2e, 0x31, 0x37, 0x00, 0x00, 0x00};
   // copy header to byteArray
   for (int c = 0; c < sizeof(header); c++) {
     _byteArray[c] = header[c];
   }
   // add root Layer PDU length: (7 | byte 16 to the end)
   int rootLayerPDU = (0x7 << 12) | (sizeof(_byteArray) - 16);
   // break into two bytes to add to array:
   _byteArray[16] = rootLayerPDU >> 8;
   _byteArray[17] = rootLayerPDU % 256;

   // add VECTOR_ROOT_E131_DATA (0x00000004) 4 bytes:
   _byteArray[18] = 0x00;
   _byteArray[19] = 0x00;
   _byteArray[20] = 0x00;
   _byteArray[21] = 0x04;

   // add (7 | PDU length)  -- byte 38 to the end
   int framingPDU = (0x7 << 12) | (sizeof(_byteArray) - 38);
   _byteArray[38] = framingPDU >> 8;
   _byteArray[39] = framingPDU % 256;

   // add VECTOR_E131_DATA_PACKET (0x00000002) 4 bytes
   _byteArray[40] = 0x00;
   _byteArray[41] = 0x00;
   _byteArray[42] = 0x00;
   _byteArray[43] = 0x02;

   // add priority, 1 bytes
   _byteArray[108] = 100;
   setSyncAddress(0x0000);
   // add sequence number:
   _byteArray[111] = _sequenceNumber;
   // add options (1 byte) (default 0x00)
   _byteArray[112] = 0x00;

   // add (7 | PDU length) -- byte 115 through the end
   int DMPlayerPDU = (0x7 << 12) | (sizeof(_byteArray) - 115);
   _byteArray[115] = DMPlayerPDU >> 8;
   _byteArray[116] = DMPlayerPDU % 256;

   // add VECTOR_DMP_SET_PROPERTY (0x02)
   _byteArray[117] = 0x02;
   // add address type & data type (0xA1)
   _byteArray[118] = 0xA1;
   // add first property address (0x0000)
   _byteArray[119] = 0x00;
   _byteArray[120] = 0x00;
   // add address increment (0x0001)
   _byteArray[121] = 0x00;
   _byteArray[122] = 0x01;
   // add property value count (0x0001 -- 0x0201) -- number of channels + 1
   int propertyValueCount = 513 ;
   // split it into two bytes:
   _byteArray[123] = propertyValueCount >> 8;
   _byteArray[124] = propertyValueCount % 256;

   // add 0x00  (DMX start byte)
   _byteArray[125] = 0x00;
   // channel values are in 126 -638, set by setChannel
}


// sets device ID (a 16-byte number) from a hex-encoded String:
void sACNSource::setUuid(const char* uuid) {
  // add 16-byte UUID (bytes 22 - 37)
  int i = 0;    // loop iteration counter
  // iterate in steps of two chars, so you can convert
  // each two-char substring to an unsigned long:
  for (int c = 0; c < strlen(uuid); c += 2) {
    char temp[] = {uuid[c], uuid[c+1], 0};  // get the temp string
    byte b = strtoul(temp, NULL, 16);     // convert it
    _byteArray[i + 22] = b;               // add the byte to the array
    i++;                                  // increment the counter
  }
}

// sets channel (1-512) to level (0-255):
void sACNSource::setChannel(int channelNum, byte level) {
  // DMX channels are generally 1-indexed from 1 - 512, so:
  if ((channelNum > 0) && (channelNum < 513)) {
    _byteArray[channelNum + 125] = level;
  }
}

// sets sourceName from a UTF-8 String, 0-terminated, 64 bytes max:
void sACNSource::setSourceName(const char* deviceName) {
  // add UTF-8 [UTF-8] encoded string, null-terminated, 64 bytes
  int l = 0;
  if ( strlen(deviceName) < 64) {
    for ( l = 0; l < strlen(deviceName); l++) {
      _byteArray[l + 44] = deviceName[l];
    }
    // name should be 0 terminated, to 64 bytes
    while (l < 64) {
      _byteArray[l + 44] = 0x00;
      l++;
    }
  } else {
    // take 64 bytes of the name:
    for ( l = 0; l < 63; l++) {
      _byteArray[l + 44] = deviceName[l];
    }
    // 0 terminate the string:
    _byteArray[l + 44] = 0x00;
  }
}

// sets universe number (16-bit number):
void sACNSource::setUniverse(int universeNumber) {
  // add universe number (2 bytes) ( default 0x0001)
  // split it into two bytes:
  _byteArray[113] = universeNumber >> 8;
  _byteArray[114] = universeNumber % 256;
}

void sACNSource::setSequenceNumber(byte seqNum) {
  _sequenceNumber = seqNum;
}

byte sACNSource::getSequenceNumber() {
  return _sequenceNumber;
}

// set sync address
void sACNSource::setSyncAddress(int syncAddress) {
  // add sync universe, 2 bytes (0x00)
  _byteArray[109] = syncAddress >> 8;
  _byteArray[110] = 0x00 % 256;
}

// set options flags:
void sACNSource::setOptions(byte options) {
  //TODO: set options
}

// return packet data
byte sACNSource::readData(int b) {
  return _byteArray[b];
}

// return packet size
int sACNSource::packetSize() {
  return sizeof(_byteArray);
}

void sACNSource::sendPacket(const char* addr, int thisPort) {
  // start a new packet:
  if (this->_udp != 0) {       // make sure there's a UDP transport
   this->_udp->beginPacket(addr, thisPort);                // start sACN packet
   this->_udp->write(_byteArray, sizeof(_byteArray));  // add payload to it
   this->_udp->endPacket();                            // finish and send packet
  // you can send continuously without incrementing the
  // sequence number, but it's good practice:
   this->setSequenceNumber(this->_sequenceNumber++);
 }
}
