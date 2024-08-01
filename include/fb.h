#pragma once

#include <stdint.h>

typedef struct {
  uint64_t addr;
  uint64_t bufsize;
  uint32_t ppsl;

  uint32_t height;
  uint32_t width;
} framebuffer_t;

// Draws a pixel at (x,y) with color 0xRRGGBB.
void putpixel(framebuffer_t* fb_gop, int x, int y, uint32_t color);