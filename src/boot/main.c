#include <uefi.h>


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



int main(){
  // Try and read the kernel
  FILE* f; char* buff; long int size;

  /// First we need to obviously read the file
  if(f = fopen("kernel.elf", "r")){
    // Get size
    fseek(f, 0, SEEK_END);
    size = ftell(f);
    fseek(f, 0, SEEK_SET);

    printf("File buffer size: %ld\n", size);

    // Then read into buff
    buff = malloc(size+1); // Allocate the size of our buffer
                           // in terms of memory.
    if(!buff){
      printf("Couldn't allocate memory to read file...\n");
      return 1;
    }

    // Memory is allocated, read.
    fread(buff, size, 1, f);
    fclose(f);
  } else {
    printf("Can't read file... :(\n");
    return 2;
  }

  // Validate if ELF is valid
  Elf64_Ehdr* elf; uintptr_t entry; int i;
  Elf64_Phdr* phdr;

  elf = (Elf64_Ehdr *)buff;
  if(elf->e_ident[EI_CLASS] == ELFCLASS64 && elf->e_type == ET_EXEC && elf->e_phnum > 0) {
    /* load segments */
    for(phdr = (Elf64_Phdr *)(buff + elf->e_phoff), i = 0; i < elf->e_phnum; i++, phdr = (Elf64_Phdr *)((uint8_t *)phdr + elf->e_phentsize)) {
      if(phdr->p_type == 1) {
        memcpy((void*)phdr->p_vaddr, buff + phdr->p_offset, phdr->p_filesz);
        memset((void*)(phdr->p_vaddr + phdr->p_filesz), 0, phdr->p_memsz - phdr->p_filesz);
      }
    }
    entry = elf->e_entry;
  } else {
    printf("Invalid ELF!\n");
    return 0;
  }
  /* free resources */
  free(buff);

  printf("Kernel is at 0x%p\n", entry);
  i = (*((int(* __attribute__((sysv_abi)))(void))(entry)))();
  printf("Return value: %d\n", i);

  while(1);
  return 0;
}