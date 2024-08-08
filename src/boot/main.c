#include <uefi.h>

#include "structs.h"

int main(){
  // Try and read the kernel
  FILE* f; char* buff; long int size = 0;

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


  // Get GOP
  efi_guid_t gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
  efi_gop_t* gop;

  efi_status_t s = BS->LocateProtocol(&gopGuid, NULL, (void**)&gop);
  if(EFI_ERROR(s)){
    printf("Cannot find GOP on this machine. :(\n");
    return 2;
  }

  // Get current mode
  efi_gop_mode_info_t* info;
  uintn_t sizeInfo, numModes, nativeMode;

  s = gop->QueryMode(gop, gop->Mode == NULL ? 0 : gop->Mode->Mode,
                    &sizeInfo, &info);
  if(s == EFI_NOT_STARTED){
    s = gop->SetMode(gop, 0);
  }

  // Error?
  if(EFI_ERROR(s)){
    printf("Can't get native mode...\n :/");
    return 3;
  } else {
    nativeMode = gop->Mode->Mode;
    numModes   = gop->Mode->MaxMode;
  }

  // Find the one that is 1024 by 768.
  int modeNumber = 0;
  for(uintn_t i = 0; i < numModes; i++){
    s = gop->QueryMode(gop, i, &sizeInfo, &info);
    if(info->HorizontalResolution == 1024
      && info->VerticalResolution == 768){
        modeNumber = i;
        break;
    }
  }

  // Memory map
  efi_status_t status;
  efi_memory_descriptor_t *memory_map = NULL, *mement;
  uintn_t memory_map_size=0, map_key=0, desc_size=0;

  int block_ctr = 0;

  status = BS->GetMemoryMap(&memory_map_size, NULL, &map_key, &desc_size, NULL);
  if(status != EFI_BUFFER_TOO_SMALL || !memory_map_size){
    printf("Can't get the memory map!\n");
    return 2;
  }

  /* in worst case malloc allocates two blocks, and each block might split a record into three, that's 4 additional records */
  memory_map_size += 4 * desc_size;
  memory_map = (efi_memory_descriptor_t*)malloc(memory_map_size);
  
  if(!memory_map) {
    fprintf(stderr, "unable to allocate memory\n");
    for(;;);
  }
  status = BS->GetMemoryMap(&memory_map_size, memory_map,
                            &map_key, &desc_size, NULL);
  if(EFI_ERROR(status)) {
    fprintf(stderr, "Unable to get memory map\n");
    for(;;);
  }

  mement = memory_map;
  block_t blocks[256];
  
  do {
    if(mement->Type == EfiConventionalMemory){
      blocks[block_ctr].free = 0;
      blocks[block_ctr].location = (uintptr_t)mement->PhysicalStart;
      blocks[block_ctr].size = mement->NumberOfPages;

      block_ctr++;
    }
    mement = NextMemoryDescriptor(mement, desc_size);
  } while((uint8_t*)mement < (uint8_t*)memory_map + memory_map_size);

  free(memory_map);

  // Now the fun part.
  // We can probe a random location in memory, i.e. 0x7FFFFFF_0000000
  // to 0x7FFFFFF_FFFFFFF and use that as our new block. In order to
  // prevent anything weird from happening, we make sure that:
  //    1. 0x7FFFFFF_0000000 is 0
  //    2. All values are set to zero
  uint64_t d = (*(uint64_t*)0x70000FFF);

  // Weirdly, I get a GPF if use the mentioned address in the comment above
  // So we'll use 0x70000FFF for now.
  if(d != 0){
    printf("Hm.... Open an issue on GitHub. I tried use extra memory for mmap.\n");
    for(;;);
  }
  uint64_t size_mmap = (0x7FFFFFFF - 0x70000FFF);


  printf("Got here.\n");
  // If we're still here, then we didn't get a GPF. Add this memory
  // location to the blocks
  blocks[block_ctr].free = 0;
  blocks[block_ctr].location = 0x70000FFF;
  blocks[block_ctr].size = size_mmap;

  block_ctr++;

  printf("We are going to set GOP mode %d...\n", modeNumber);
  s = gop->SetMode(gop, modeNumber);
  if(EFI_ERROR(s)){
    printf("Couldn't set... :(\n");
    return 4;
  }

  printf("FB addr -> %p, Size %d -- PPSL %d.\n",
          gop->Mode->FrameBufferBase,
          gop->Mode->FrameBufferSize,
          gop->Mode->Information->PixelsPerScanLine);

  // Set up to send to kernel.
  framebuffer_t fb = {
    .addr    = gop->Mode->FrameBufferBase,
    .bufsize = gop->Mode->FrameBufferSize,
    .pitch   = (sizeof(uint32_t))*gop->Mode->Information->PixelsPerScanLine,

    /* Also send some information relating to GOP's mode. */
    .height  = gop->Mode->Information->VerticalResolution,
    .width   = gop->Mode->Information->HorizontalResolution
  };

  bootinfo_t bootp = {
    .fb = &fb,
    .blocks = blocks // This raises a warning. Why?
  };

  printf("Kernel is at 0x%p\n", entry);
  i = (*((int(* __attribute__((sysv_abi)))(bootinfo_t*))(entry)))(&bootp);
  printf("Return value: %d\n", i);

  while(1);
  return 0;
}