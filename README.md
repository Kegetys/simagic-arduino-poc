# Overview

![](https://junk.kegetys.fi/simagic_banner.jpg)

This is a description of the wireless protocol used by Simagic direct drive wheel bases and rims. With this you can make your own wireless rims that send button presses to the Simagic base, receive data from a Simagic rim or maybe make a separate button box that activates the base buttons (for wheels without any buttons on them).

I haven't figured out the entire protocol. For example the data sent back by the base to adjust the rim LEDs is still unknown.

# Hardware

The rim and base contain AS01-Ml01S V4.0 wireless modules. These modules use nRF24L01 chips which are commonly used in wireless keyboards, mice and many other wireless devices. These are easy to use with Arduino IDE using the RF24 library: https://www.arduino.cc/reference/en/libraries/rf24/

The Simagic base quick release provides a 5V voltage to the rim, which is perfect for Arduino and ESP32 based microcontroller dev boards. The power is received with 5pin 2.45mm pitch pogo pins on the rim with contacts on the base. Despite having 5 pins the connector only has 5V and GND with the rest of the extra pins probably for physical redundancy.

For powering and attaching your custom rim you can find a complete Simagic compatible QR with the power connectors included from Aliexpress: https://www.aliexpress.com/item/1005005142013070.html

Interestingly that QR also comes with the base side connector, so you could use it also to attach a simagic rim to another base and power it, then receive button data from the rim and adapt it to the other base.

There is also a custom PCB you can create in printables.com that should work in a NRG QR: https://www.printables.com/model/489715-simagic-qr-quickrelease-wheelside-pcb

In the bases the wireless module is located next to the green LED above the wheel axis. You can check out Barry's disassembly of the Alpha Mini base for more details about the HW setup: https://www.youtube.com/watch?v=wPe628qP5n8

# nRF24L01 setup

This is the most interesting part, since in order to communicate with either the rim or the base the nRF24 needs to know the network ID, channel, speed and CRC setup used by Simagic. I attached a logic analyzer to the wireless module in my rim in order to intercept the module setup and was able to capture both the module setup process and communication with the base.

The settings used are as follows:
- The network ID is 0x0110104334.
- The channel can be configured from SimPro Manager, it seems to default to 60 and it matches the RF24 hardware channel.
- Speed is 250KBps, CRC is 8 bits.
- Dynamic payloads appear to be enabled, but the payload from rim to wheel is always 8 bytes.
- AutoAck is enabled with 7 retires, delay setting 5.
- ACK payloads are enabled and used by the base to send data back to the rim.

### Note about nRF24L01 clones

The chips in the base & rim appear to be genuine Nordic Semiconductor parts while most nRF24L01 chips you find ie. from Aliexpress are clones, including my Si24R1 modules. These clone modules have a hardware bug where they have the message NO_ACK bit inverted from the genuine ones and this means they are not directly capable of communicating the ACK packets with the genuine chips. If you just want to send button and axis data from your rim to the wheel this is not a big problem, human input is so low rate that you can just send the packets multiple times and the base will amost certainly receive at least one message due to the very close proximity. I haven't noticed any missed input with this approach.

But I also did find a possible workaround for this issue that works at least with the Si24R1. It involves first enabling the dynamic ACKs, writing the message to the chip buffer but with the NO_ACK bit asking to be set, then disabling dynamic ACKs again before actually sending the buffer out:

```
radio.enableDynamicAck();  // enable dynamic ACKs so se can request the NO_ACK bit
radio.startFastWrite(&m, sizeof(packetTx), true); // ask for NO_ACK, but it actually ends up not set since the logic is inverted
radio.disableDynamicAck();          // disable dynamic ACKs so our chip ignores the feature completely
const bool ok = radio.txStandBy();  // send the buffer, returns true if ACK was received
if (radio.available())
{
  // read ACK payload
}
```

What I believe happens with this is that the NO_ACK bit gets not set in the FIFO buffer since its use is inverted from the spec, so the buffer is sent to the genuine module on the other end with the flag disabled. The genuine module then responds with ACK, but since we have disabled dynamic acks before actually sending the buffer out our chip correctly receives and processes the ACK packet.

# Rim to base packet structure

The rim always appears to send 8 byte packets. Interestingly despite the wireless layer having a CRC checksum enabled the last byte of the packet contains another redundant 1 byte CRC-8/MAXIM checksum of the first 7 bytes. If this is not correct the base ignores the message.

The structure appears to be as follows:
```
D1 D2 D3 D4 CO D5 D6 CC 
            ^^       ^^ CRC checksum
            ^^ Command
^^ D1-D6 are dynamic data depending on the Command sent and CRC being the CRC-8/MAXIM checksum.
```

## Button + axis packet

Button and axis data are sent with command 0x3c.

D1 to D4 are bit flags for buttons currently held down

D5 and D5 are clutch paddle axis values (0-255)

## Rim connect packets

When the rim is connected it identifies itself with the base and the rim apperas in SimPro Manager. However even without this being sent the button packet above works, so you don't actually need to send this for custom rims to work fine.

The data contains at least the rim type, firmware version and what looks like some kind of per-button information (maybe the LED colors?). I haven't fully examined this data, a dump is included in initpacket.h            

# Unknowns
- There is apparently some kind of automatic channel negotation in some rims?
- Parsing the ACK reply packets (they seem to contain LED info)
- The FX Pro rim display probably also uses the same communication channel
- Various things in the rim initialization packet

# Proof-of-concept Arduino project

See "example" folder for an example proof-of-concept Arduino project for a rim that sends button presses to the base. The pinout is for a Wemos D1 Mini microcontroller but all you really need is an nRF24 module correctly wired to the board so any Arduino or ESP board should be easy to make work.


