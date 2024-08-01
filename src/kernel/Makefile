ARCH := x86_64

# TODO: swap for crosscompiler
CC := gcc
LD := ld

CFLAGS  := -fno-stack-protector -fno-stack-check -ffreestanding \
				 	 -D__$(ARCH)__
LDFLAGS := -nostdlib -z max-page-size=0x1000 -Ttext=0x01000000

all: build
.PHONY: all

build:
	mkdir -p obj/
	$(CC) $(CFLAGS) -c kernel.c -o obj/kernel.o
	$(LD) $(LDFLAGS) obj/kernel.o -o kernel.elf

clean:
	rm -rf obj/ kernel.elf