# Set build directory
if [ -z "$DIR_BUILD" ]
then
    DIR_BUILD=./build
fi
DIR_BUILD=$(readlink -f $DIR_BUILD)
mkdir -p $DIR_BUILD

# Start compiling projects
cd projects

cd awd
DIR_BUILD=$DIR_BUILD make awd -j4
cd ..

cd lobo
DIR_BUILD=$DIR_BUILD make lobo -j4
cd ..

cd .. # Projects

# Build ISO
mkdir -p $DIR_BUILD/sysroot/
cp -r data/iso/* $DIR_BUILD/sysroot/
cp $DIR_BUILD/awd/awd.elf $DIR_BUILD/sysroot/boot.elf
cp $DIR_BUILD/lobo/lobo.elf $DIR_BUILD/sysroot/kernel.elf

grub-mkrescue -o $DIR_BUILD/bootable.iso $DIR_BUILD/sysroot/ &> /dev/null