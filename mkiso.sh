#!/bin/bash
cp mmlv.img iso
xorriso -as mkisofs -R -f -e mmlv.img -no-emul-boot -o MMLV.iso iso || exit

rm -rf iso