#include "cpu/exception_hdlr.h"

// Then, we make a little look up table for common opcodes, i.e. interrupts
char faulty_opcodes[] = {
  0xcc
};

char* meaning[] = {
  "INT 3 BREAKPOINT."
};


// Returns current value at that memory address.
uint32_t get_opcode(uint32_t memadrr){
  // This returns the next opcode aswell, e.g. B8CC. We only want the
  // CC part, so we get the lower half.
  return (*(uint32_t*)memadrr) & 0xFF;
}

void exception_handler(registers_t r){
  int found = 0;
  setcolor(0xFF0000);
  printf("[ERR] ISR %d triggered! Info:\n", r.int_no);
  printf("\tInstruction pointer: 0x08:%x (opcode: %x)\n\tFinding potential descriptor...", r.rip-1,
        get_opcode(r.rip-1));
  for(int i = 0; i < 1; i++){
    if((char)get_opcode(r.rip-1) == faulty_opcodes[i]){
      printf("found! %s\n", meaning[i]);
      found = 1;
    }
  }

  if(found != 1) printf("not found. :(\n");
  printf("\tError code -> %d\n\n", r.err_code);
  printf("System halted...");
  while(1) asm("cli; hlt");
}
