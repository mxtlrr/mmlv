#pragma once

#include "cpu/idtr.h"
#include "libc/output.h"


void kbd_handler(registers_t regs){
	uint8_t k = inb(0x60);
	printf("%c", k);
}

void init_kbd(){
	register_irq(33, &kbd_handler);
}