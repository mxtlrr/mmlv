#include "libc/output.h"
#include "fb.h"

int _start(bootinfo_t* bootp){
  set_bootp(bootp);

  puts("Wow\nThis operating system sucks...\n\n");
  puts(__DATE__);
  while(1);
  return 0;
}