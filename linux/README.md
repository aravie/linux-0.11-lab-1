Linux-0.11
==========

The old Linux kernel source ver 0.11 which has been tested under modern Linux, Mac OSX and Windows.

## 1. Build on Linux

## 1.1. Linux Setup

* a linux distribution: debian , ubuntu and mint are recommended
* some tools: gcc gdb qemu
* a linux-0.11 hardware image file: hd0.img, please run create-hd0.sh under out/ to create it.
* Now, This version already support the Ubuntu 16.04, enjoy it.

## 1.2. hack linux-0.11

    $ make help		// get help
    $ make
    $ make bochs-debug
    $ gdb tools/system
    (gdb) target remote :1234
    (gdb) b _main
    (gdb) c

##2. Build on Mac OS X (Not tested)

##2.1. Mac OS X Setup

* install cross compiler gcc and binutils
* install qemu
* install gdb. you need download the gdb source and compile it to use gdb because port doesn't provide i386-elf-gdb, or you can use the pre-compiled gdb in the tools directory.
* a linux-0.11 hardware image file: hd0.img

#
    $ sudo port install qemu
    $ sudo port install i386-elf-binutils i386-elf-gcc

optional

    $ wget ftp://ftp.gnu.org/gnu/gdb/gdb-7.4.tar.bz2
    $ tar -xzvf gdb-7.4.tar.bz2
    $ cd gdb-7.4
    $ ./configure --target=i386-elf
    $ make

##2.2. hack linux-0.11

    same as section 1.2

##3. Build on Windows

Build `build\target\generic_x86\win32\msvc\v14\linux.sln` with Visual C++ 2015.
