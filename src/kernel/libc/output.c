#include "libc/output.h"
#include "libc/string.h"
#include "fb.h"

int x = 0; int y = 0;

void putc(char c){
  char f[2] = { c, '\0' };
  writestring(f); // Eugh.
}

void puts(char* fmt){
  writestring(fmt);
}


void printf(char* fmt, ...){
  va_list ap;
  va_start(ap, fmt);

  char* ptr;
  	for (ptr = fmt; *ptr != '\0'; ++ptr) {
		if (*ptr == '%') {
			++ptr;
			switch (*ptr) {
				case 's':	// string
					puts(va_arg(ap, char*));
					break;
				case 'd': // integer
					puts(tostring(va_arg(ap, int), 10));
					break;
				case 'x': // hexadecimal
					puts(tostring(va_arg(ap, uint32_t), 16));
					break;
				case 'c':
					putc(va_arg(ap, int));
			}
		} else {
			char t[2] = { *ptr, 0 };
			puts(t);
		}
	}
}

// Internal function -- Never should be used by itself
void writestring(char* s){
  unsigned char *ptr, *chr, *frg;
  unsigned int c;
  unsigned long long int o, p;
  int i, j, k, l, m, n;
  while(*s) {
    if((*s & 128) != 0) {
      if(!(*s & 32)) { c = ((*s & 0x1F)<<6)|(*(s+1) & 0x3F); s += 1; } else
      if(!(*s & 16)) { c = ((*s & 0xF)<<12)|((*(s+1) & 0x3F)<<6)|(*(s+2) & 0x3F); s += 2; } else
      if(!(*s & 8)) { c = ((*s & 0x7)<<18)|((*(s+1) & 0x3F)<<12)|((*(s+2) & 0x3F)<<6)|(*(s+3) & 0x3F); s += 3; }
      else c = 0;
    } else c = *s;
    s++;


    // TODO: implement scrolling
    // This one's kinda shitty.
    if(c == '\n'){
      int tmp = y + font->height;
      if(y != 1024 || tmp < 1024){
        x = 0;
        y += font->height;
      } else {
        y = 0;
      }
      continue;
    } else if(c == '\t'){
      x += chr[4]+1;
      x += chr[4]+1;
      x += chr[4]+1;
      x += chr[4]+1;
      continue;
    }


    for(ptr = (unsigned char*)font + font->characters_offs, chr = 0, i = 0; i < 0x110000; i++) {
      if(ptr[0] == 0xFF) { i += 65535; ptr++; }
      else if((ptr[0] & 0xC0) == 0xC0) { j = (((ptr[0] & 0x3F) << 8) | ptr[1]); i += j; ptr += 2; }
      else if((ptr[0] & 0xC0) == 0x80) { j = (ptr[0] & 0x3F); i += j; ptr++; }
      else { if((unsigned int)i == c) { chr = ptr; break; } ptr += 6 + ptr[1] * (ptr[0] & 0x40 ? 6 : 5); }
    }
    if(!chr) continue;
    ptr = chr + 6; o = (unsigned long long int)bootp->fb->addr + y * bootp->fb->pitch + x * 4;
    for(i = n = 0; i < chr[1]; i++, ptr += chr[0] & 0x40 ? 6 : 5) {
      if(ptr[0] == 255 && ptr[1] == 255) continue;
      frg = (unsigned char*)font + (chr[0] & 0x40 ? ((ptr[5] << 24) | (ptr[4] << 16) | (ptr[3] << 8) | ptr[2]) :
        ((ptr[4] << 16) | (ptr[3] << 8) | ptr[2]));
      if((frg[0] & 0xE0) != 0x80) continue;
      o += (int)(ptr[1] - n) * bootp->fb->pitch; n = ptr[1];
      k = ((frg[0] & 0x1F) + 1) << 3; j = frg[1] + 1; frg += 2;
      for(m = 1; j; j--, n++, o += bootp->fb->pitch)
        for(p = o, l = 0; l < k; l++, p += 4, m <<= 1) {
          if(m > 0x80) { frg++; m = 1; }
          if(*frg & m) *((unsigned int*)p) = 0xFFFFFF;
        }
    }
    x += chr[4]+1; y += chr[5];
  }
}
