#include <stdint.h>

#include "cpu/idtr.h"
#include "libc/output.h"

__attribute__((noreturn))
void exception_handler(registers_t *r);
