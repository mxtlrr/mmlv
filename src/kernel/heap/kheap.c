#include "heap/kheap.h"

uint64_t usable = 0;
block_t blocks[BLOCK_SIZES];


void set_up_kheap(int usable_n){
  // Note that usable_n is from the bootloader bootp.
  usable = usable_n;
  if(usable_n != 1) return;

  // The first block should be the "master" block.
  blocks[0].free      = FREE;
  blocks[0].location  = 0x70000FFF;
  blocks[0].size      = 0xFFFF000;  // Lasts until 0x7FFFFFFF.

  // Everything else should be set to 0 - including
  // the location should be 0
  for(int i = 1; i < 256; i++){
    blocks[i].free = FREE; blocks[i].location = 0;
    blocks[i].size = 0;
  }

  // Done:)
}

uintptr_t malloc(uint64_t n){
  if(usable != 1) return 0; // Can't do anything. Fuck!

  // Find blocks whose size are >=. TODO: do <.
  int index = 0;
  for(int i = 0; i < BLOCK_SIZES; i++){
    if(blocks[i].free == FREE && blocks[i].size >= n)
      index = i;
  }

  // If size is greater, split into two (this code
  // is definently going to be janky.)
  if(blocks[index].size > n){
    block_t new_block = {
      .free = ALLOC, .location = blocks[index].location,
      .size = n
    };

    block_t old_block = blocks[index];

    // Offset every other block by 1
    for(int i = index; i < BLOCK_SIZES; i++){
      // Don't overflow! Only do valid ones.
      // Hopefully we can safely overwrite the last block,
      // but that is a bad mindset to have.
      if(i < 255){
        blocks[i+1] = blocks[i];
      }
    }

    // Now, blocks[index] is free.

    // Then set the current block to the new block we made, and
    // set blocks[index] to blocks[index+1]. I think maybe I'm doing
    // this wrong.
    blocks[index]   = new_block;
    blocks[index+1] = old_block;

    printf("blocks val:\n\tloc -> %x\n\tsize: %d\n\tfree? %s\n",
          blocks[index].location,
          blocks[index].size,
          blocks[index].free ? "YES" : "NO");

    return blocks[index].location;
  }
}

uintptr_t calloc(uintptr_t n){
  uintptr_t loc = malloc(n);
  int indice = -1;
  for(int i = 0; i < 256; i++){
    if(blocks[i].location == loc){
      indice = i;
      break;
    }
  }

  if(indice == -1) return 2; // Not found! WTF are you doing!?
  for(int i = blocks[indice].location; i < blocks[indice].location+blocks[indice].size;
      i++){
    // Maybe a bad way to do this.
    *(uintptr_t*)i = (uintptr_t)0;
  }
  return loc;
}

void free(uint64_t addr){
  int indice = -1;
  for(int i = 0; i < 256; i++){
    if(blocks[i].location == addr){
      indice = i;
      break;
    }
  }

  // Ok set it to free.
  // TODO: Undo the splitting I did in malloc.
  blocks[indice].free = FREE;
}