ARCH := x86_64

# TODO: swap for crosscompiler
CC := gcc
LD := ld

override CFILES := $(shell find . -type f -name '*.c')
override OFILES := $(shell find . -type f -name '*.o')


CFLAGS  := -fno-stack-protector -fno-stack-check -ffreestanding \
				 	 -D__$(ARCH)__ -I../../include/
LDFLAGS := -nostdlib -z max-page-size=0x1000 -Ttext=0x01000000

all: build
.PHONY: all

build:
	@mkdir -p obj/
	@$(foreach file, $(CFILES), $(CC) $(CFLAGS) -c $(file) -o obj/$(basename $(notdir $(file))).o; echo CC $(file);)
	@echo "Linking to ELF..."
	@$(LD) $(LDFLAGS) $(OFILES) -o kernel.elf

clean:
	rm -rf obj/ kernel.elf