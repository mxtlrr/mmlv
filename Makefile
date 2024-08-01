all: build
.PHONY: all

build:
	@mkdir -p obj/ bin/ iso/
	make -C src/boot
	make -C src/kernel

mk_img:
	mkdir -p obj/
	dd if=/dev/zero of=mmlv.img bs=512 count=93750
	mformat -i mmlv.img ::
	mmd -i mmlv.img ::/EFI
	mmd -i mmlv.img ::/EFI/BOOT
	mcopy -i mmlv.img bin/BOOTX64.efi ::/EFI/BOOT
	mcopy -i mmlv.img src/kernel/kernel.elf ::

run_qemu: mmlv.img
	qemu-system-x86_64 -cpu qemu64 \
	-drive if=pflash,format=raw,unit=0,file=ovmf/OVMF_CODE.fd,readonly=on \
	-drive if=pflash,format=raw,unit=1,file=ovmf/OVMF_VARS.fd -drive file=$^,if=ide \
	-net none


clean:
	rm *.img *.iso bin/ obj/ iso/ -rf
	make -C src/boot clean
	make -C src/kernel clean