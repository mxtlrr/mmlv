#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "libc/output.h"
#include "io.h"

#define KCS 0x08
#define IDT_MAX_DESCRIPTORS 256

typedef struct {
	uint16_t    isr_low;      // The lower 16 bits of the ISR's address
	uint16_t    kernel_cs;    // The GDT segment selector that the CPU will load into CS before calling the ISR
	uint8_t	    ist;          // Set to zero for now
	uint8_t     attributes;   // Type and attributes; see the IDT page
	uint16_t    isr_mid;      // The higher 16 bits of the lower 32 bits of the ISR's address
	uint32_t    isr_high;     // The higher 32 bits of the ISR's address
	uint32_t    reserved;     // Set to zero
}__attribute__((packed)) idt_entry_t;

typedef struct {
  uint16_t limit;
  uint64_t base;
}__attribute__((packed)) idtr_t;

typedef struct registers {
	uint64_t rdi, rsi, rbp, rsp, rbx, rdx, rcx, rax;
	uint64_t r8, r9, r10, r11, r12, r13, r14, r15;
	uint64_t ds, int_no, err_code;
	uint64_t rip;
} registers_t;

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags);
void idt_init();
void init_irqs();

typedef void (*isr_t)(registers_t);
void irq_handler(registers_t r);
void register_irq(uint8_t n, isr_t handler);