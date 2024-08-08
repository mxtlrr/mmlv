#include "cpu/idtr.h"

__attribute__((aligned(0x10))) 
static idt_entry_t idt[256];
static idtr_t idtr;

static bool vectors[IDT_MAX_DESCRIPTORS];
extern void* isr_stub_table[];
extern void* irq_stub_table[];

void remap_pic(void){
  uint8_t a1, a2; 

  a1 = inb(PIC1_DATA);
  a2 = inb(PIC2_DATA);

  outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
  outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);

  outb(PIC1_DATA, 0x20);
  outb(PIC2_DATA, 0x28);

  outb(PIC1_DATA, 4);
  outb(PIC2_DATA, 2);

  outb(PIC1_DATA, ICW4_8086);
  outb(PIC2_DATA, ICW4_8086);

  outb(PIC1_DATA, a1);
  outb(PIC2_DATA, a2);
}

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

  remap_pic(); // Remap PIC so we can do stuff without the IRQs overlapping.

  // Turn on interrupts
  __asm__ volatile ("sti");
}

void init_irqs(){
  // 32 to 47
  for(uint8_t vector = 32; vector < 48; vector++){
    idt_set_descriptor(vector, irq_stub_table[vector-32], 0x8e);
    vectors[vector] = true;
  }
}

isr_t interrupt_handlers[256];
void irq_handler(registers_t r){

  if(interrupt_handlers[r.int_no] != 0){
    // go handle it
    isr_t hdlr = interrupt_handlers[r.int_no];
    hdlr(&r);
  }

  // Send EOI
  if(r.int_no >= 40) {
    outb(0xa0, 0x20);
  }
  // reset to master aswell
  outb(0x20, 0x20);
}

void register_irq(uint8_t n, isr_t handler){
  printf("Registered IRQ %d\n", n);
  interrupt_handlers[n] = handler;
}