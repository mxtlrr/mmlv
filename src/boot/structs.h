#pragma once

#include <uefi.h>

/* GOP/Framebuffer stuff */
typedef struct {
  uint64_t addr;
  uint64_t bufsize;
  uint32_t pitch;     // sizeof(uint32_t) * ppsl;

  uint32_t height;
  uint32_t width;
} framebuffer_t;

typedef struct {
  unsigned char  magic[4];
  unsigned int   size;
  unsigned char  type;
  unsigned char  features;
  unsigned char  width;
  unsigned char  height;
  unsigned char  baseline;
  unsigned char  underline;
  unsigned short fragments_offs;
  unsigned int   characters_offs;
  unsigned int   ligature_offs;
  unsigned int   kerning_offs;
  unsigned int   cmap_offs;
} __attribute__((packed)) ssfn_font_t;

/* Memory map */

/// Each block is a part of the memory map.
/// It contains data on whether or not it is free,
/// its location, etc.
typedef struct {
  uint64_t  location; // Physical
  uint64_t  size;     // How big is this block?
  uint8_t   free;     // 0 - Free, 1 - Used
} block_t;

/* Bootloader stuff (stuff to pass to the kernel) */
typedef struct {
  framebuffer_t* fb;
  ssfn_font_t* font;
  block_t blocks[256];
} bootinfo_t;


/* ELF header */
#define ELFMAG      "\177ELF"
#define SELFMAG     4
#define EI_CLASS    4       /* File class byte index */
#define ELFCLASS64  2       /* 64-bit objects */
#define EI_DATA     5       /* Data encoding byte index */
#define ELFDATA2LSB 1       /* 2's complement, little endian */
#define ET_EXEC     2       /* Executable file */

typedef struct {
  uint8_t  e_ident[16];   /* Magic number and other info */
  uint16_t e_type;        /* Object file type */
  uint16_t e_machine;     /* Architecture */
  uint32_t e_version;     /* Object file version */
  uint64_t e_entry;       /* Entry point virtual address */
  uint64_t e_phoff;       /* Program header table file offset */
  uint64_t e_shoff;       /* Section header table file offset */
  uint32_t e_flags;       /* Processor-specific flags */
  uint16_t e_ehsize;      /* ELF header size in bytes */
  uint16_t e_phentsize;   /* Program header table entry size */
  uint16_t e_phnum;       /* Program header table entry count */
  uint16_t e_shentsize;   /* Section header table entry size */
  uint16_t e_shnum;       /* Section header table entry count */
  uint16_t e_shstrndx;    /* Section header string table index */
} Elf64_Ehdr;

typedef struct {
  uint32_t p_type;        /* Segment type */
  uint32_t p_flags;       /* Segment flags */
  uint64_t p_offset;      /* Segment file offset */
  uint64_t p_vaddr;       /* Segment virtual address */
  uint64_t p_paddr;       /* Segment physical address */
  uint64_t p_filesz;      /* Segment size in file */
  uint64_t p_memsz;       /* Segment size in memory */
  uint64_t p_align;       /* Segment alignment */
} Elf64_Phdr;


