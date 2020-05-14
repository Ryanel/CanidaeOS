# Config
MAKE_OPTS=-j16

# Set build directory
if [ -z "$DIR_BUILD" ]
then
    DIR_BUILD=./build
fi
DIR_BUILD=$(readlink -f $DIR_BUILD)
mkdir -p $DIR_BUILD

# Start compiling projects
cd src

echo "----------+- Compiling libc -----------------------"
cd libc; 
DIR_BUILD=$DIR_BUILD make all $MAKE_OPTS;
DIR_BUILD=$DIR_BUILD make install $MAKE_OPTS; 
cd ..

echo "----------+- Compiling AWD ------------------------"
cd awd; DIR_BUILD=$DIR_BUILD make awd $MAKE_OPTS; cd ..

echo "----------+- Compiling Lobo -----------------------"
cd lobo; DIR_BUILD=$DIR_BUILD make lobo $MAKE_OPTS; cd ..

cd .. # Projects

# Build ISO
mkdir -p $DIR_BUILD/sysroot/
mkdir -p $DIR_BUILD/sysroot/os/
cp -r src/buildmedia/iso/* $DIR_BUILD/sysroot/

cp $DIR_BUILD/awd/awd.elf $DIR_BUILD/sysroot/boot/awd.elf
cp $DIR_BUILD/lobo/lobo.elf $DIR_BUILD/sysroot/os/kernel.elf

grub-mkrescue -o $DIR_BUILD/bootable.iso $DIR_BUILD/sysroot/ &> /dev/null

wc -c $DIR_BUILD/sysroot/boot/awd.elf
wc -c $DIR_BUILD/sysroot/os/kernel.elf
