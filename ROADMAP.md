# Phase 0: Bare Bones
- [ ] Bootloader
  - [X] loading ELF off the FAT disk
  - [X] Passing important info to the kernel (GOP, etc.)
  - [X] Setting up framebuffer from UEFI services
  - [ ] Exit boot services, pass complete control to the kernel.
- [X] Kernel
  - [X] `puts`, `putc,`, `printf`

# Phase 1: Stuff from the CPU
- [ ] Redo GDT, since I'm not entirely sure UEFI gives us a valid one
- [ ] Interrupts
  - [ ] Create valid 64-bit IDT
  - [ ] Implement ISRs
  - [ ] IRQs
    - [ ] PIT
    - [ ] PS/2 Keyboard
