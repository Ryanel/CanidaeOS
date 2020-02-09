qemu-system-x86_64 -cdrom build/bootable.iso \
-cpu qemu64,+ssse3, -smp 1 \
-m 128 \
-usb -vga std \
-serial stdio \
-no-shutdown -no-reboot \
#-s -S \