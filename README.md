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


**Tangent on 0 Init .bss**
```C++
int var; // global
int main() {
    // static
    static y;
    
    // both print 0
    cout << "x = " << x << endl;
    cout << "y = " << y << endl;
    
    return 0;
}
```
