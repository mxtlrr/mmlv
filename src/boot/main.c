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
    .fb = &fb
  };

  printf("Kernel is at 0x%p\n", entry);
  i = (*((int(* __attribute__((sysv_abi)))(bootinfo_t*))(entry)))(&bootp);
  printf("Return value: %d\n", i);

  while(1);
  return 0;
}