// Proof of concept example for sending button presses to Simagic wheelbases
// by Keijo 'Kegetys' Ruotsalainen, http://www.kegetys.fi

// Pinout for wiring to Wemos D1 Mini (hardware SPI + CE & CS):
// Wemos pin - nRF24 pin
//      3.3V - VCC
//       GND - GND
//        D0 - CE
//        D5 - SCLK
//        D6 - MISO
//        D7 - MOSI
//        D8 - CS

// Requires RF24 library

#include "simagic.h"

#define PIN_SIMAGIC_CE  D0
#define PIN_SIMAGIC_CS  D8
#define SIMAGIC_CHANNEL 60 // channel configured in SimPro manager

// SPI used so CSN, MOSI, MISO and SCK must be wired to HW pins (IRQ not needed)
simagic base(PIN_SIMAGIC_CE, PIN_SIMAGIC_CS, SIMAGIC_CHANNEL);

//#include "initpacket.h"

void setup()
{
  Serial.begin(115200);
  delay(100);
  base.begin();

  pinMode(LED_BUILTIN, INPUT_PULLUP);
  Serial.println("simagic-poc by Kegetys");

  // uncomment this and the include above to send the rim identification packet
  // you should see the rim appear in SimPro manager then.
  // The buttons and axes however work even without doing this, you just won't see a rim in the manager.
  //sendRimInit();
}

void loop()
{
  // invent some button presses
  const int buttons = 8;
  const bool state[buttons] = 
  {
    (millis() % 1000) < 500,
    (millis() % 5000) < 2500,
    (millis() % 7000) < 2000,
    (millis() % 100)  < 50,
    (millis() % 2600) < 1000,
    (millis() % 2600) < 1500,
    (millis() % 2600) < 2000,
    (millis() % 2600) < 2500,
  };

  // axis data
  const char axis0 = (char) (millis() / 50L) % 127;
  const char axis1 = (char) (millis() / 200L) % 127;

  // convert buttons to bits
  unsigned long bits = 0x0;
  for (int i = 0; i < buttons; i++)
    if (state[i])
      bits |= 1 << i;
  
  // send state
  base.setButtonBits(bits);
  base.setAxis(0, axis0);
  base.setAxis(1, axis1);
  base.tick();

  // wait a bit and blink led
  delayMicroseconds(100);
  digitalWrite(LED_BUILTIN, (millis() % 1000) < 250 ? LOW : HIGH);
}
