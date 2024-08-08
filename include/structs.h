#pragma once

#include <stdint.h>


typedef struct {
  uint64_t addr;
  uint64_t bufsize;
  uint32_t pitch;     // sizeof(uint32_t) * ppsl;

  uint32_t height;
  uint32_t width;
} framebuffer_t;

/* Memory map */

/// Each block is a part of the memory map.
/// It contains data on whether or not it is free,
/// its location, etc.
typedef struct {
  uint64_t  location; // Physical
  uint64_t  size;     // How big is this block?
  uint8_t   free;     // 0 - Free, 1 - Used
} block_t;


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


/* Bootloader stuff (stuff to pass to the bootloader) */
typedef struct {
  framebuffer_t* fb;
  ssfn_font_t* font;
  block_t blocks[256];
} bootinfo_t;