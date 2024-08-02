#include "cpu/exception_hdlr.h"

void exception_handler(registers_t r){
  printf("[ERR] ISR %d triggered! Info:\n", r.int_no);
  printf("\tInstruction pointer: 0x08:%x\n", r.rip-1);
  printf("\tError code -> %d\n\n", r.err_code);
  printf("System halted...");
  while(1) asm("cli; hlt");
}