# mmlv
mmlv is a monolithic UEFI kernel.

# Compilation

## 1: Getting POSIX-UEFI
```sh
$ git clone https://gitlab.com/bztsrc/posix-uefi.git
$ cd mmlv/src/boot
$ ln -s ../../../posix-uefi/uefi
```

## 2: Compiling
```
make
```

## 3: Booting
Run `make qemu` to run in QEMU -- Firmware is in the `ovmf/` directory. If you would
like to build an ISO, the `./build_iso.sh` script is available. Note that in order
to run that script, you need to have a `mmlv.img` existing (run `make`).