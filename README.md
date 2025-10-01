### Mini OS

Some notes for myself....
- Once executing, use ctrl + a to escape into QEMU debugging (QEMU monitor). I think this is only possible because we configure QEMU to route its standard input and output to the machine's serial port? 
- After producing a `.elf` file, you can `$llvm-objdump` to see the assembly created. This is the `.text` segment, ie the actual instructions (duh)

Examining low level stuff:
- `llvm-objdump` to see assembly of .text
- `llvm-nm file.elf` to see the symbol table


# Summary & Notable Parts

### SBI & OpenSBI
Supervisor Binary Interfac (SBI) is an API for OS kernels. Recall that kernels CPUs run in different modes... in RISCV, this includes:
- M-mode - machine mode, highest privillege, used by bootloader/firmware
- S-mode - supervisor mode... aka just kernel mode. For OS
- U-mode - user mode

Based on our QEMU flags, we use openSBI (an open source API for SBI) to actually enable us to switch between modes. 

Alongisde this, openSBI provides the `ecall` instruction... this is analgous to syscalls. 
- Process invokves syscall to OS to do something on the process's behalf that it doesn't have permissions for
- OS invokes ecall to openSBI to do something on the OS's behalf that it doesn't have permissions for. 

### Linker File (.ld)
The `.ld` file is used to specify any sort explicit linking configurations that you want your project to compile with. In this case, we use the `.ld` file to manually layout the memory of our OS rather than let the compiler handle it for us. More specifically, we define the OS's memory to be a *contiguous*, logically segmented memory space... so nothing fancy like base & bounds or paging... yet.

The "normal" segments that processes define are text, read only data, 
- `.text` - colloquially holds the source of a process... in this case the actual kernel's source (binary, since its compiled)
- `.rodata` - "read-only" data; ie constants and string literals
- `.data` - initialized global and static variables
- `.bss` - uninitialized global and static variables... these are actually initialized to 0 (hence why declarations of var in global scope actually have a value of 0. 
- ~~`heap` - dynamically allocated memory~~
- `stack` - stack frames


Now the linker scripting language is a little peculiar at first, but some reading it at face value:
- Declares that the entry point of the executable is boot()
- Defines various "sections"... our literal memory segments
    - There's five of them... four explicity stated and the fifth (stack) is is allocated implicity via a giant jump in the address space before defininng the stack pointer 
    - there's some *implementation identifiers* -- variables that start with `__` that are defined. These are kernel level variables that define the beginning of the `.bss` segment. The linker will reserve the space for these values... but won't actually popualte them (duh they are undefined... but wait! I thought we said they go 0-init?, well use they do, but thats at runtime!)
        - **TODO**... some online tutorials show `.data` defining start and end variables.... i probably should do this. 
        - `.rodata` doesn't need this since the binary will contain the values directly
        - same for `.text`... the compiler obviously knows what the instructions are.


### .elf file?
An `.elf` file or **Executable and Linkable Format** File is a a standard format for executable files. Basically, when you go off and compile some 20 line `main.cpp` file, the produced `a.out` is a `.elf` file. 
 
> running `$ file a.out` will reveal that it is indeed a ELF file

In the broader scope of things, during the compilation process after creating object files (`.o`), the linker will link all of them as an ELF. Now technically `.o` files are already in ELF, they just aren't executable. Its at this point that the linker will do a bunch of magic... or rather in our case, the linker script will decide memory layout, entry point, etc.








# Other Resource for myself to refer to
- https://sourceware.org/binutils/docs/ld/index.html#SEC_Contents

