# khienhOS
Developed Record clicks [HERE](https://hackmd.io/@zoanana990/KERNEL_DEV)

## Environment Setup
Operating System: Ubuntu Linux 20.04

Install some repositories:
```shell
sudo apt update
sudo apt install nasm qemu-system-x86
```
Check the `nasm` is installed:
```
nasm -v
```
Expect:
```
NASM version 2.14.02
```
Check the `qemu` is installed:
```
qemu-system-x86_64
```
Expect: A qemu windows is opened

### Install Cross-Compiler for developing kernel in C
we need to install some repositpories. [links](https://wiki.osdev.org/GCC_Cross-Compiler)

In linux debian:
```shell
$ sudo apt install build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo libisl-dev
```
Download the [bunutils](https://sourceware.org/pub/binutils/snapshots/)
Download the [cross compiler](https://ftp.lip6.fr/pub/gcc/releases/gcc-10.2.0/)

Install the `bunutils`:
```shell
export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"
cd $HOME/Env
mkdir build-binutils && cd build-binutils
../binutils-2.35.90/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make
make install
```

Install the `gcc`
```shell
export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"
cd $HOME/Env
 
# The $PREFIX/bin dir _must_ be in the PATH. We did that above.
which -- $TARGET-as || echo $TARGET-as is not in the PATH
 
mkdir build-gcc
cd build-gcc
../gcc-10.2.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc
```
Using the new compiler
```shell
$HOME/opt/cross/bin/$TARGET-gcc --version
```

### GDB in this project
```shell
gdb

(gdb) target remote | qemu-system-x86_64 -hda ./boot.bin -S -gdb stdio
```

## Getting Start
There are some notes in [Note](./note) folders, you can check the file and getting the background knowledge to develop this kernel

compile the code by yourself
```shell
sudo chmod u+x ./build.sh
```

## Some Resources
- [Ralf Brown's Interrupt List](https://www.ctyme.com/rbrown.htm)
- [OSDev.org](https://wiki.osdev.org/Main_Page)
- [x86 Assembly Guide](https://www.cs.virginia.edu/~evans/cs216/guides/x86.html)