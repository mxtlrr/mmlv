#include "io.h"

uint8_t inb(uint16_t port){
  uint8_t ret;
  __asm__ volatile ( "inb %w1, %b0" : "=a"(ret) : "Nd"(port): "memory");
  io_wait();
  return ret;
}

void outb(uint16_t port, uint8_t val){
  __asm__ volatile ( "outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
  io_wait();
}

void io_wait(){
  asm volatile ("outb %%al, $0x80" : : "a"(0));
}