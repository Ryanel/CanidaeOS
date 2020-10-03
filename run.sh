qemu-system-x86_64 -cdrom build/bootable.iso \
-machine type=q35 \
-cpu qemu64 \
-smp 6 \
-m 1024 \
-usb -vga std \
-device sga \
-no-shutdown -no-reboot \
-serial stdio

#-nographic \
