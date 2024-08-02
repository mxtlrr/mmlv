#include "cpu/idtr.h"

#include "libc/output.h"
#include "fb.h"

extern void load_gdt(); // gdt.asm

int _start(bootinfo_t* bootp){
  set_bootp(bootp);

  load_gdt();
  printf("[init] GDT loaded.\n");

  idt_init();
  printf("[init] IDT up and running.\n\n");
  asm("int $0x3");  // Should cause ISR to trigger.
  while(1) asm("hlt");
  return 0;
}