qemu-system-x86_64 -cdrom build/bootable.iso \
-cpu qemu64,+ssse3, -smp 1 \
-m 1024 \
-usb -vga std \
-device sga \
-no-shutdown -no-reboot -d cpu_reset \
#-serial mon:stdio \
#-nographic \