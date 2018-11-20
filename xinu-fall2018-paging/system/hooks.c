// Import Lib
#include<xinu.h>

void hook_ptable_create(unsigned int pagenum) {
	kprintf("Created page table %d\n\n", pagenum);
}

void hook_ptable_delete(unsigned int pagenum) {
	kprintf("Deleted page table %d\n\n", pagenum);
}

// Print Fault Address
void hook_pfault(char *addr) {
	kprintf("Page fault for address %d\n\n", addr);
}

// Not Used
void hook_pswap_out(unsigned int pagenum, int framenb) {
	kprintf("Replacing frame number %d, virtual page %d\n\n", framenb, pagenum);
}

// Return Page Fault Count
int get_faults() {
	// Return
	return pfErrCode; 
}
