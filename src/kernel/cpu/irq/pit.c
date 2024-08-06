#include "cpu/irq/pit.h"

void hdlr(registers_t r){
  printf("INT 20");
}

void register_pit(){
  register_irq(32, &hdlr);
}