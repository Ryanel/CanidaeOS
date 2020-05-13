qemu-system-x86_64 -cdrom build/bootable.iso \
-machine type=q35 \
-cpu qemu64,+ssse3,+sse4.1,+sse4.2,+x2apic,+pdpe1gb \
-smp 1 \
-m 4096 \
-usb -vga std \
-device sga \
-no-shutdown -no-reboot \
#-serial mon:stdio \
#-nographic \