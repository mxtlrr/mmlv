#include "cpu/idtr.h"
#include "cpu/irq/pit.h"

#include "libc/output.h"
#include "fb.h"

#include "heap/kheap.h"

extern void load_gdt(); // gdt.asm

int _start(bootinfo_t* bootp){
  set_bootp(bootp);

  asm("cli");
  load_gdt();
  printf("[init] GDT loaded.\n");

  idt_init();
  printf("[init] IDT up and running.\n");

  set_up_kheap(bootp->usable);
  setcolor(0x123456);
  printf("Testing malloc...");

  uintptr_t mctest = malloc(4); // Allocate four bytes of memory.
  printf("mem val is %x.\n", mctest);

  free(mctest);
  printf("freed %x.\n", mctest);

  setcolor(0xffffff);
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
