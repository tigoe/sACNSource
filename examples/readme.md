# Examples

## [Example 1]({{site.github.repository_url}}/tree/main/examples/Example1_sACNIntro)
This sketch creates a sACN packet, then uses it to send fade information to an sACN receiver.

## [Example 2]({{site.github.repository_url}}/tree/main/examples/Example2_OneFader)
This sketch creates a sACN packet, then uses it to send fade information to an sACN receiver. It reads from one analog input to generate the channel level. 

## [Example 3]({{site.github.repository_url}}/tree/main/examples/Example3_SixFaders)
Fading six DMX channels with six potentiometers 

circuit: six potentiometers attached to pins A0 through A5

## [Example 4]({{site.github.repository_url}}/tree/main/examples/Example4_PanTiltJoystick)
Using a joystick to control pan, tilt, and brightness of an Elation Platinum Spot LED Pro II in standard mode. Datasheet is at [this link](http://cdb.s3.amazonaws.com/ItemRelatedFiles/9955/elation_platinum_spot_led_pro_II_user_manual_ver_2.pdf). 

DMX channels:
* 1 - pan
* 2 - tilt
* 12 - brightness

## [Example 5]({{site.github.repository_url}}/tree/main/examples/Example5_encoderHueChange)
Rotary encoder controlling the hue of a DMX-controlled light using HSI calculations, with the following channels:
*  1 - overall dimmer
* 2 - red
* 3 - green
* 4 - blue
* 5 - white

The color of the light will depend on the angle of the encoder relative to its zero position. 0 degrees = red, 120 degrees = green, 240 degrees = blue. The calculation of angle on the encoder is approximate. It assumes 88 steps per rotation. Pressing the pushbutton resets the encoder to zero, and resets the colro wheel to 0 degrees (red).


## [Example 6]({{site.github.repository_url}}/tree/main/examples/Example6_printPacket)

This sketch creates a sACN packet, then loops over the DMX channels, fading each one up and down at a time. It continually prints out the packet hexadecimal values, so you can see what each byte value is. It does not send the packets out via UDP, however. This sketch is for understanding the protocol only. It does not send any packets.

## [Example 7](Example7_sACNEthernet)
This sketch creates a sACN packet, then uses it to send fade information to an sACN receiver over wired Ethernet.
 
