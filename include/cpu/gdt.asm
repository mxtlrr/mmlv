[global load_gdt]

codeseg equ 0x08    ;; From qemu logs.
dataseg equ 0x10

gdt:
  ;; Null descriptor
	dq 0x0000000000000000
  ;; Base (0x00) - Flags (0xA) - Access byte (0x9a)
  dq 0x00A09A0000000000
  ;; Base (0x00) - Flags (0xc) - Access byte (0x92)
  dq 0x00C0920000000000

gdt_end:
  dq 0

gdtr:
	dw gdt_end - gdt - 1
  dq gdt

load_gdt:
  cli
  mov rax, gdtr
  lgdt [rax]

  mov ax, dataseg
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax

  pop rdx
  mov rax, codeseg
  push rax
  push rdx
  retfq ;; 64-bit return