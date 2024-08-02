#include "cpu/idtr.h"

__attribute__((aligned(0x10))) 
static idt_entry_t idt[256];
static idtr_t idtr;

static bool vectors[IDT_MAX_DESCRIPTORS];
extern void* isr_stub_table[];
extern void* irq_stub_table[];

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
  idt_entry_t* descriptor = &idt[vector];

  descriptor->isr_low        = (uint64_t)isr & 0xFFFF;
  descriptor->kernel_cs      = KCS;
  descriptor->ist            = 0;
  descriptor->attributes     = flags;
  descriptor->isr_mid        = ((uint64_t)isr >> 16) & 0xFFFF;
  descriptor->isr_high       = ((uint64_t)isr >> 32) & 0xFFFFFFFF;
  descriptor->reserved       = 0;
}

void idt_init() {
  idtr.base = (uintptr_t)&idt[0];
  idtr.limit = (uint16_t)sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;

  for (uint8_t vector = 0; vector < 32; vector++) {
    idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
    vectors[vector] = true;
  }

  // Actually load interrupts now:)
  __asm__ volatile ("lidt %0" : : "m"(idtr));
  __asm__ volatile ("sti");
}

void remap_pic(void){
  outb(0x20, 0x11);
  outb(0xA0, 0x11);
  outb(0x21, 0x20);
  outb(0xA1, 0x28);
  outb(0x21, 0x04);
  outb(0xA1, 0x02);
  outb(0x21, 0x01);
  outb(0xA1, 0x01);
  outb(0x21, 0x0);
  outb(0xA1, 0x0);
}

// TODO: write this, and implement outb/inb in io.h
void init_irqs(){
  remap_pic(); // Remap PIC so we can do stuff without the IRQs overlapping.
  for(uint8_t vector = 32; vector < 48; vector++){
    idt_set_descriptor(vector, irq_stub_table[vector], 0x8e);
    vectors[vector] = true;
  }
}

isr_t interrupt_handlers[256];
void irq_handler(registers_t r){
  // send reset signal to slave
  if(r.int_no >= 40) outb(0xa0, 0x20);
  // reset to master aswell
  outb(0x20, 0x20);

  if(interrupt_handlers[r.int_no] != 0){
    // go handle it
    isr_t hdlr = interrupt_handlers[r.int_no];
    hdlr(r);
  }
}

void register_irq(uint8_t n, isr_t handler){
  interrupt_handlers[n] = handler;
}