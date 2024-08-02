#pragma once

#include <stdint.h>

#include "structs.h"

// Draws a pixel at (x,y) with color 0xRRGGBB.
void putpixel(framebuffer_t* fb_gop, int x, int y, uint32_t color);

extern unsigned char font_data[8283];
extern ssfn_font_t* font;

extern bootinfo_t* bootp;
void set_bootp(bootinfo_t* param);
