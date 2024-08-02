#pragma once

#include <stdint.h>
#include <stdarg.h>

extern int x;
extern int y;

void putc(char c);
void puts(char* fmt);
void printf(char* fmt, ...);

void writestring(char* s);