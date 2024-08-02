#pragma once

#include <stdint.h>


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


/* Bootloader stuff (stuff to pass to the bootloader) */
typedef struct {
  framebuffer_t* fb;
  ssfn_font_t* font;
} bootinfo_t;