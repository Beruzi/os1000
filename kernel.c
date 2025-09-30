// Aliasing some types
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef uint32_t size_t;

// state that these are defined elsewhere... (kernel.ld)
extern char __bss[], __bss_end[], __stack_top[];


/* 
 * Getting into some C stuff... 
 * - C does not have templates, so in order to generalize types we 
 *   return or pass in void*. These are pointers to some address with 
 *   an unknown type. Can later cast the pointer into the proper type
 */


void* memset(void* buf, char c, size_t n) {
	// Cast pointer of buffer to uint8_t type... this allows for 
	// arithmetic to be performed. Since the pointer is 8bits, incrementation goes by byte
	uint8_t* p = (uint8_t* )buf;

	// dereference and assign the pointer p to the value c n times.
	while(n--) { 
		*p = c;
		++p;
		// could combine lines into *p++ = c;
		// Order of OP: postincrement, dereference (before increment) assign to c;
	}
	return buf;
}


void kernel_main(void) {
	// calling the memset functino above using the 
	memset(__bss, 0, (size_t) __bss_end - (size_t) __bss);

	for(;;); // infinte loop
}

/* Some compiler extensions... 
 * __attribute__(<something>) --> tells the of a special attritbute or
 * configuration you want to apply to something
 *
 *__asm__ __volatile__ --> this is inline assembly code 
 *
*/

// Here we explictly tell the compiler what "named" section we want
// to store data in based on the .ld section (.text.tboot)
// Recall that in .ld we explictly used KEEP() to include the boot() 
// function in our .text section
//
// (naked) is used to tell the compiler to not generate any extra 
// instructions, guaranteeing that the boot() function contains only
// the specified assembly
__attribute__((section(".text.tboot")))
__attribute__((naked))
void boot(void) {
	__asm__ __volatile__(
		"mv sp, %[stack_top]\n" // raw asm to set stack pointer (sp)
		"j kernel_main\n"		// Jump to kernel main function
		:
		: [stack_top] "r" (__stack_top) // Pass stack address  
	);
}





