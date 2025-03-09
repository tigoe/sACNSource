# sACN over Ethernet

This example shows how to send sACN from an Arduino over Ethernet. It was written with a Nano 33 IoT and a MKR Ethernet shield, as shown in Figure 1, but should work with any Arduino and any Ethernet shield that's compatible with the Ethernet library. 

Figure 1 shows the circuit. The Arduino's SPI pins are connected to the Ethernet shield's SPI pins. There is an additional pin used to disable the SD Card on the MKR Ethernet shield. 

If you're using a MKR board (MKR Zero, MKR1010, etc), you can just plug the shield in directly. For other boards, you'll need to match the pins up. 

The pins for the Nano 33 IoT and other Nanos is in Table 1 below. The physical pin numbers for both the Nano and the shield are counted in a U pattern, starting with pin 1 on the top left. The physical pin numbers don't correspond to the pin labels, so the pin labels are listed in parentheses in table 1. 

| Function | Nano 33 IoT Physical Pin | MKR Ethernet Shield Physical Pin  | 
|-----|-----|-----|
| SDO | 29 (D1) | 17 (D8) | 
| SDI | 30 (D12)  | 19 (D10) | 
| CS | 23 (D5)  | 14 (D5) | 
| SCK | 1 (D13)  | 18 (D9) | 
| Reset | 3 (Reset) | 24 (Reset) | 
| SD Card Enable | 22 (D4) | 13 (D4) | 

_Table 1. Pin connections between an Arduino Nano and the MKR Ethernet shield_

The code can be found at [this link]({{site.github.repository_url}}/tree/main/examples/Example7_sACNEthernet). 

[![Figure 1. Nano 33 IoT connected to a MKR Ethernet shield](Nano_33_MKR_ETH.png)](Nano_33_MKR_ETH.png)

_Figure 1. Nano 33 IoT connected to a MKR Ethernet shield_

