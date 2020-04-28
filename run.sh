qemu-system-x86_64 -cdrom build/bootable.iso \
-cpu qemu64,+ssse3, -smp 1 \
-m 64 \
-usb -vga std \
-device sga \
-no-shutdown -no-reboot \
-serial mon:stdio \
#-nographic \