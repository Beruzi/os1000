### Mini OS

Some notes for myself....



Once executing, use ctrl + a to escape into QEMU debugging (QEMU monitor). I think this is only possible because we configure QEMU to route its standard input and output to the machine's serial port? 





# Summary & Notable Parts

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
        - **TODO**... some online tutorials show `.data` being defined, i probably should do this. 
        - `.rodata` doesn't need this since the binary will contain the values directly
        - same for `.text`... the compiler obviously knows what the instructions are.


### 






# Other Resource for myself to refer to
- https://sourceware.org/binutils/docs/ld/index.html#SEC_Contents

