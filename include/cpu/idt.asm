%macro isr_err_stub 1
isr_stub_%+%1:
  push byte %1
  jmp isr_cstb
%endmacro

%macro isr_no_err_stub 1
isr_stub_%+%1:
  push byte 0
  push byte %1
  jmp isr_cstb
%endmacro

extern exception_handler
isr_no_err_stub 0
isr_no_err_stub 1
isr_no_err_stub 2
isr_no_err_stub 3
isr_no_err_stub 4
isr_no_err_stub 5
isr_no_err_stub 6
isr_no_err_stub 7
isr_err_stub    8
isr_no_err_stub 9
isr_err_stub    10
isr_err_stub    11
isr_err_stub    12
isr_err_stub    13
isr_err_stub    14
isr_no_err_stub 15
isr_no_err_stub 16
isr_err_stub    17
isr_no_err_stub 18
isr_no_err_stub 19
isr_no_err_stub 20
isr_no_err_stub 21
isr_no_err_stub 22
isr_no_err_stub 23
isr_no_err_stub 24
isr_no_err_stub 25
isr_no_err_stub 26
isr_no_err_stub 27
isr_no_err_stub 28
isr_no_err_stub 29
isr_err_stub    30
isr_no_err_stub 31

%macro PUSH_N 0
  push r8
  push r9
  push r10
  push r11
  push r12
  push r13
  push r14
  push r15
%endmacro

%macro POP_N 0
  pop r15
  pop r14
  pop r13
  pop r12
  pop r11
  pop r10
  pop r9
  pop r8
%endmacro

isr_cstb:
  push rdi
  push rsi
  push rbp
  push rsp
  push rbx
  push rdx
  push rcx
  push rax

  PUSH_N

  mov eax, ds
  push rax
  mov eax, es
  push rax
  push fs
  push gs

  mov eax, 0x10
  mov ds, eax
  mov es, eax
  mov fs, eax
  mov gs, eax

  mov rdi, rsp                 ; RDI = Pointer to registers_t

  call exception_handler

  pop gs
  pop fs
  pop rax
  mov es, rax
  pop rax
  mov ds, rax

  pop rax
  pop rcx
  pop rdx
  pop rbx
  pop rsp
  pop rbp
  pop rsi
  pop rdi

  POP_N

  add rsp, 16
  iretq


global isr_stub_table
isr_stub_table:
%assign i 0
%rep    32
  dq isr_stub_%+i
%assign i i+1
%endrep

;; we're gonna do the same thing for irqs

extern irq_handler

%macro IRQ 2
irq_stub_%1:
  push byte 0
  push byte %2
  jmp irq_cstb
%endmacro

IRQ   0,    32
IRQ   1,    33
IRQ   2,    34
IRQ   3,    35
IRQ   4,    36
IRQ   5,    37
IRQ   6,    38
IRQ   7,    39
IRQ   8,    40
IRQ   9,    41
IRQ  10,    42
IRQ  11,    43
IRQ  12,    44
IRQ  13,    45
IRQ  14,    46
IRQ  15,    47

irq_cstb:
  push rdi
  push rsi
  push rbp
  push rsp
  push rbx
  push rdx
  push rcx
  push rax

  PUSH_N

  mov eax, ds
  push rax
  mov eax, es
  push rax
  push fs
  push gs

  mov eax, 0x10
  mov ds, eax
  mov es, eax
  mov fs, eax
  mov gs, eax

  mov rdi, rsp                 ; RDI = Pointer to registers_t
  call irq_handler

  pop gs
  pop fs
  pop rax
  mov es, rax
  pop rax
  mov ds, rax

  POP_N

  pop rax
  pop rcx
  pop rdx
  pop rbx
  pop rsp
  pop rbp
  pop rsi
  pop rdi

  add rsp, 16
  iretq


global irq_stub_table
irq_stub_table:
%assign j 0
%rep 15
  dq irq_stub_%+j
%assign j j+1
%endrep
