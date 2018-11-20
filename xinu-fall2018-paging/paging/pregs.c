// Import Libs
#include<xinu.h>

// Staging Vars for Regs
unsigned long val;

// Enable Paging
syscall	enablePaging() {
  // Disable Interrupts
  intmask mask = disable();

  // Disable Interrupts
	intmask mask;
  	mask = disable();

	// Get CR0
	asm("pushl %eax;"
	    "movl %cr0, %eax;"
	    "movl %eax, val;"
	    "popl %eax;");

	// Mask Value
	val = val | 0x80000000;

	// Set CR0
	asm("pushl %eax;"
	    "movl val, %eax;"
	    "movl %eax, %cr0;"
	    "popl %eax;");

  // Restore and Return
  restore(mask);
  return OK;
}

// Set CR3
syscall	setPDBR(unsigned int frameNum) {
  // Disable Interrupts
  intmask mask = disable();

	// Right Shift
	val = (unsigned long)frameNum;
  val = val << 12;

	// Set CR3
	asm("pushl %eax;"
	    "movl val, %eax;"
	    "movl %eax, %cr3;"
	    "popl %eax;");

  // Restore and Return
  restore(mask);
  return OK;
}
