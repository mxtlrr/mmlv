# Phase 0: Bare Bones
- [ ] Bootloader
  - [X] loading ELF off the FAT disk
  - [X] Passing important info to the kernel (GOP, etc.)
  - [X] Setting up framebuffer from UEFI services
  - [ ] Exit boot services, pass complete control to the kernel.
- [X] Kernel
  - [X] `puts`, `putc,`, `printf`

# Phase 1: Stuff from the CPU
- [X] Redo GDT, since I'm not entirely sure UEFI gives us a valid one
- [X] Interrupts
  - [X] Create valid 64-bit IDT
  - [X] Implement ISRs
  - [X] IRQs
    - [X] PIT

*Note: PS/2 Keyboard driver comes later :^)*
# Phase 2: FS / User utility
- [ ] Primitive file system
  - [ ] Initrd (`initrd/` directory)
- [ ] Switch to user mode
  - [ ] System calls
  - [ ] Implement either COFF or ELF

# Phase 3: Memory / Paging etc
*Maybe this should come before phase 2..*

- [ ] Setup a kernel heap