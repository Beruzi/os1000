#!/bin/bash
set -xue

# Qemu file path
QEMU=qemu-system-riscv32

# Path to Clang and Compiler Flags
CC=clang
CFLAGS="-std=c11 -O2 -g3 -Wall -Wextra --target=riscv32-unknown-elf -fno-stack-protector -ffreestanding -nostdlib"

# Build kernel
$CC $CFLAGS -Wl,-Tkernel.ld -Wl,-Map=kernel.map -o kernel.elf \
    kernel.c


# Start QEMU using the 'virt' machine, default BIOS (openSBI), no GUI (nographic), route input and output to virtual machine, if vm crashes don't restart
$QEMU -machine virt -bios default -nographic -serial mon:stdio --no-reboot \-kernel kernel.elf
