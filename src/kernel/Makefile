ARCH := x86_64

# TODO: swap for crosscompiler
CC := gcc
LD := ld

override CFILES   := $(shell find . -type f -name '*.c')
override AS_FILES := $(shell find ../../include/ -type f -name '*.asm')
override OFILES := $(shell find . -type f -name '*.o')
override OFILES_C_LST := $(addprefix obj/, $(notdir $(patsubst %.c, %.o, $(CFILES))))
override OFILES_AS_LST := $(addprefix obj/, $(notdir $(patsubst %.asm, %.o, $(AS_FILES))))
override OFILES_LST := $(OFILES_C_LST) $(OFILES_AS_LST)

CFLAGS  := -O0 -mgeneral-regs-only -mno-red-zone -fno-stack-protector -fno-stack-check -ffreestanding \
				 	 -D__$(ARCH)__ -I../../include/ -nolibc
LDFLAGS := -nostdlib -z max-page-size=0x1000 -Ttext=0x01000000 \
					 -z noexecstack

all: build
.PHONY: all

build:
	@mkdir -p obj/
	@$(foreach file, $(AS_FILES), nasm -felf64 $(file) -o obj/$(notdir $(basename $(file))).o; echo AS $(file);)
	@$(foreach file, $(CFILES), $(CC) $(CFLAGS) -c $(file) -o obj/$(basename $(notdir $(file))).o; echo CC $(file);)
	@echo "Linking to ELF..."
	@$(LD) $(LDFLAGS) $(OFILES_LST) -o kernel.elf

clean:
	rm -rf obj/ kernel.elf
