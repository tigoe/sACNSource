# Examples

## [Example 1](({{site.github.repository_url}}/tree/main/examples/Example1_sACNIntro)
This sketch creates a sACN packet, then uses it to send fade information to an sACN receiver.

## [Example 2](({{site.github.repository_url}}/tree/main/examples/Example2_OneFader)
  This sketch creates a sACN packet, then uses it to send
   fade information to an sACN receiver. It reads from
   one analog input to generate the channel level. 

## [Example 3](({{site.github.repository_url}}/tree/main/examples/Example3_SixFaders)

## [Example 4](({{site.github.repository_url}}/tree/main/examples/Example4_PanTiltJoystick)

## [Example 5](({{site.github.repository_url}}/tree/main/examples/Example5_encoderHueChange)

## [Example 6](({{site.github.repository_url}}/tree/main/examples/Example6_printPacket)

This sketch creates a sACN packet, then loops over the DMX channels, fading each one up and down at a time. It continually prints out the packet hexadecimal values, so you can see what each byte value is. It does not send the packets out via UDP, however. This sketch is for understanding the protocol only. It does not send any packets.

## [Example 7](Example7_sACNEthernet)
This sketch creates a sACN packet, then uses it to send fade information to an sACN receiver over wired Ethernet.
 
