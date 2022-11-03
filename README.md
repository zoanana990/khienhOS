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

install the [cross compiler](https://ftp.lip6.fr/pub/gcc/releases/gcc-10.2.0/)


## Getting Start
There are some notes in [Note](./note) folders, you can check the file and getting the background knowledge to develop this kernel

## Some Resources
- [Ralf Brown's Interrupt List](https://www.ctyme.com/rbrown.htm)
- [OSDev.org](https://wiki.osdev.org/Main_Page)
- [x86 Assembly Guide](https://www.cs.virginia.edu/~evans/cs216/guides/x86.html)