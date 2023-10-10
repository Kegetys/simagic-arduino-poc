// GTS rim initialization packets
// add to end of setup() to try
void sendRimInit()
{
  // initialization ritual
  base.sendRaw(0x00000000, 0x3C0000);  
  
  // set rim type
  // 0x05 = GTS rim
  // 0x04 = gt hub
  // 0x03 = fx pro rim
  // 0x02 = fx rim
  // 0x01 = doesn't work right, likely requires some different init?
  base.sendRaw(0x00003201, 0xBC8005); // last 0x05 = type
  base.sendRaw(0x00003201, 0xBC8005); // last 0x05 = type
  base.sendRaw(0x00003201, 0xBC8005); // last 0x05 = type
  
  // some kind of per-button data perhaps?
  base.sendRaw(0x03020100, 0xBC0104);
  base.sendRaw(0x03020100, 0xBC0104);
  base.sendRaw(0x03020100, 0xBC0104);
  base.sendRaw(0x03020100, 0xBC0104);
  base.sendRaw(0x03020100, 0xBC0104);
  base.sendRaw(0x03020100, 0xBC0104);
  base.sendRaw(0x08070605, 0xBC0209);
  base.sendRaw(0x0D0C0B0A, 0xBC030E);
  base.sendRaw(0x1211100F, 0xBC0413);
  base.sendRaw(0x17161514, 0xBC0518);
  base.sendRaw(0x1C1B1A19, 0xBC061D);
  base.sendRaw(0x21201F1E, 0xBC0722);
  base.sendRaw(0x26252423, 0xBC0827);
  base.sendRaw(0x4E4B4644, 0xBC0953);
  base.sendRaw(0x5D5A5855, 0xBC0A64);
  base.sendRaw(0x60909095, 0xBC0B93);
  base.sendRaw(0x60336090, 0xBC0C30);
  base.sendRaw(0x40206570, 0xBC0D50);
  base.sendRaw(0x52526070, 0xBC0E52);
  base.sendRaw(0x10313131, 0xBC0F10);
  base.sendRaw(0x10101010, 0xBC1010);
  base.sendRaw(0x40405858, 0xBC1140);
  base.sendRaw(0x40404040, 0xBC1290);
  base.sendRaw(0x40405858, 0xBC1340);
  base.sendRaw(0x40404040, 0xBC1490);
  base.sendRaw(0x40405858, 0xBC1540);
  base.sendRaw(0x90404040, 0xBC1690);
  base.sendRaw(0x40405858, 0xBC1740);
  base.sendRaw(0x90404040, 0xBC1890);
  base.sendRaw(0x40405858, 0xBC1940);
  base.sendRaw(0x90904040, 0xBC1A90);
  base.sendRaw(0x30305858, 0xBC1B30);
  base.sendRaw(0x90903030, 0xBC1C90);
  base.sendRaw(0x30305858, 0xBC1D30);
  base.sendRaw(0x90903030, 0xBC1E90);
  base.sendRaw(0x30305858, 0xBC1F30);
  base.sendRaw(0x90909030, 0xBC2090);
  base.sendRaw(0x10105858, 0xBC2110);
  base.sendRaw(0x90909010, 0xBC2290);
  base.sendRaw(0x00000000, 0xBC2300);
  base.sendRaw(0x00000000, 0xBC2400);
  base.sendRaw(0x00000000, 0xBC2500);
  base.sendRaw(0x00000000, 0xBC2600);
  base.sendRaw(0x00000000, 0xBC2700);
  base.sendRaw(0x00000000, 0xBC2800);
  base.sendRaw(0x00000000, 0xBC2900);
  base.sendRaw(0x00000000, 0xBC2A00);
  base.sendRaw(0x00000000, 0xBC2B00);
  base.sendRaw(0x00000000, 0xBC2C00);
  base.sendRaw(0x00000000, 0xBC2D00);
  base.sendRaw(0x00000000, 0xBC2E00);
  
  base.sendRaw(0x01320120, 0xBC2F30); // 0x32 = firmware version  
}
