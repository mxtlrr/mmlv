#include "libc/output.h"
#include "fb.h"

extern void load_gdt(); // gdt.asm

int _start(bootinfo_t* bootp){
  set_bootp(bootp);

  load_gdt();
  printf("[init] GDT loaded:)\n");
  while(1);
  return 0;
}