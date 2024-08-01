#include <uefi.h>

int main(){
  printf("Hello world!\n");


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


  return 0;
}