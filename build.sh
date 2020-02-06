cd projects

cd awd
make awd -j16
cd ..

cd lobo
make lobo -j16
cd ..

cd .. # Projects

mkdir -p build/sysroot/
cp -r data/iso/* build/sysroot/
cp build/awd/awd.elf build/sysroot/boot.elf
cp build/lobo/lobo.elf build/sysroot/kernel.elf

grub-mkrescue -o build/bootable.iso build/sysroot/ &> /dev/null