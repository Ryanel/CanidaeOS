qemu-system-x86_64 -cdrom build/bootable.iso \
-cpu qemu64,+ssse3, -smp 1 \
-m 128 \
-usb -vga std \
-device sga \
-no-shutdown -no-reboot \
-s \
-monitor stdio \
#-d int \
#-nographic \