
// Aliasing some types
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef uint32_t size_t;

// state that these are defined elsewhere... (kernel.ld)
extern char __bss[], __bss_end[], __stack_top[];

// Set up SBI (ecall)
// Since openSBI interface operates at a very low level, we comply with standards/
// convetions and can't really rely on syscalls, libraries, virtual memory stuff...
struct sbiret sbi_call(long arg0, long arg1, long arg2, long arg3, long arg4,
                       long arg5, long fid, long eid) {
    register long a0 __asm__("a0") = arg0;
    register long a1 __asm__("a1") = arg1;
    register long a2 __asm__("a2") = arg2;
    register long a3 __asm__("a3") = arg3;
    register long a4 __asm__("a4") = arg4;
    register long a5 __asm__("a5") = arg5;
    register long a6 __asm__("a6") = fid;
    register long a7 __asm__("a7") = eid;


	// this is like a firmware syscall... RISCV M-mode will trap this and delegate
	// to proper handler
    __asm__ __volatile__("ecall"
                         : "=r"(a0), "=r"(a1)
                         : "r"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5),
                           "r"(a6), "r"(a7)
                         : "memory");
    return (struct sbiret){.error = a0, .value = a1};
}



// Fill Memory
void* memset(void* buf, char c, size_t n) {
	uint8_t* p = (uint8_t* )buf;

	while(n--) { 
		*p = c;
		++p;
		// *p++ = c is the same as both lines above
	}
	return buf;
}


void kernel_main(void) {
	// 0 out the .bss section using memset()
	memset(__bss, 0, (size_t) __bss_end - (size_t) __bss);

	for(;;); // infinte loop; hence why after ./run.sh, nothing happens
}


// Compiler Directives
// 1. Force the "boot" function into the .text.boot subsection
// 2. Don't add any extra asm... this will avoid the usual function prolouge and epilouge... both of which attempt to use the SP.
__attribute__((section(".text.boot")))
__attribute__((naked))
void boot(void) {
	__asm__ __volatile__(
		"mv sp, %[stack_top]\n"	// raw asm to load stack top address to sp reg 
		"j kernel_main\n"		// Jump to kernel main function
		:
		: [stack_top] "r" (__stack_top)
	);
}

