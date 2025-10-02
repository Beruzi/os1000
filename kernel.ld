/* 
 * This is a linker file (ld) that defines a script that specifies
 * how memory is laid out for a process. Since this is defining the OS/
 * kernel process, this is actually partitioning physical memory. 
 *
 * More specifically, this linker file will be defining the memory 
 * layout of the kernel -- it is a program after all that will execute 
 * after bootup. 
 * 		- states that the entry point of the kernel is the "boot" 
 * 		  function
 * 		- The base address of the kernel is 0x80200000
 * 		- Memory segments of the kernel are then defined afterwards:
 * 			- text: 	source
 * 			- rodata:	"read only" data... basically global/static
 * 			- data: 	writable globals w/ initial values
 * 			- .bss: 	writable glabls w/o intial values
 *
 * 	Other things. 
 * 		- the '.' character represents the current address and 
 * 		  automatically increments as data is placed. This means that
 * 		  the 4 segments listed above are stored consecutively in
 * 		  memory. 
 * 	    - the last 3 lines increment the '.' (current address) by 128KB
 * 	      and is defining the top of the stack!
 *
 * 	    - the .text segment includes the "KEEP()" directive. This tells
 * 	      the compiler to alwasy include the boot source code
 *
 * 	    - the align(4) directive makes sure the memory segments start
 * 	      on a word boundary, thus "alighing" the segments 
 */


/* linker knows that the upon execution the CPU should jump to the "boot" function */
ENTRY(boot)

SECTIONS {
    . = 0x80200000;

    .text :{
        KEEP(*(.text.boot));
        *(.text .text.*);
    }

    .rodata : ALIGN(4) {
        *(.rodata .rodata.*);
    }

    .data : ALIGN(4) {
        *(.data .data.*);
    }

    .bss : ALIGN(4) {
        __bss = .;
        *(.bss .bss.* .sbss .sbss.*);
        __bss_end = .;
    }
	

    . = ALIGN(4);
    . += 128 * 1024; /* 128KB */
    __stack_top = .;
}
