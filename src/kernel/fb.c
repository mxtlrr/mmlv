#include "fb.h"

void putpixel(framebuffer_t* fb_gop, int x, int y, uint32_t color){
  *((uint32_t*)(fb_gop->addr + 4 * fb_gop->ppsl * y + 4 * x)) = color;
}