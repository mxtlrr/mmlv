#include "libc/output.h"
#include "fb.h"

int _start(bootinfo_t* bootp){
  set_bootp(bootp);

  printf("Testing %d. Hex address 0x%x", 123, 0xDEADBEEF);
  while(1);
  return 0;
}