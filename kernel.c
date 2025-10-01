
// Aliasing some types
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef uint32_t size_t;

// state that these are defined elsewhere... (kernel.ld)
extern char __bss[], __bss_end[], __stack_top[];


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

