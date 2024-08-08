#include "cpu/idtr.h"
#include "cpu/irq/pit.h"

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
  
  setcolor(0x00ff00);
  printf("[test] testing PIT interrupt.\n");

  register_pit();

  while(1) {
    asm("hlt");
  }

  for(;;);
  return 0;
}
