#include "cpu/idtr.h"

#include "libc/output.h"
#include "fb.h"

extern void load_gdt(); // gdt.asm

int _start(bootinfo_t* bootp){
  set_bootp(bootp);

  asm("cli");
  load_gdt();
  printf("[init] GDT loaded.\n");

  idt_init();
  printf("[init] IDT up and running.\n");

  init_irqs();
  printf("[init] Set up IRQs...\n");

  while(1) {
    asm("hlt");
  }
  return 0;
}