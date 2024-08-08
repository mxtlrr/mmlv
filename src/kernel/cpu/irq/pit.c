#include "cpu/irq/pit.h"
#define FREQ 50

uint64_t tick = 0;

void hdlr(registers_t* r){
  tick++;
}

void register_pit(){
  register_irq(32, hdlr);

  uint32_t div = 1193180 / FREQ;
  outb(0x43, 0x36);

  uint8_t l = (uint8_t)(div & 0xff);
  uint8_t h = (uint8_t)((div>>8)&0xff);

  outb(0x40, l);
  outb(0x40, h);
}
