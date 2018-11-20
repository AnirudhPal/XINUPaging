// Import Libs
#include<xinu.h>

// Proc
void test2P() {
	// Print
	intmask mask = disable();
	kprintf("PID: %d -> Using All Pages\n", currpid);
	restore(mask);

	unsigned long i;
	unsigned long* ptr = (unsigned long*)vgetmem(2 * NBPG);
	for(i=0; i < (2 * NBPG) / 4; i++) {
		ptr[i] = 0x123ABCDE;
		kprintf("ptr[%d] = 0x%x\n", i, ptr[i]);
	}
	for(i=0; i < (2 * NBPG) / 4; i++) {
		if(ptr[i] != 0x123ABCDE) {
			// Print
			mask = disable();
			kprintf("PID: %d -> Something is Wrong Val: 0x%x at 0x%x\n", currpid, *(ptr + i), ptr + i);
			restore(mask);
		}
	}

	// Print
	mask = disable();
	kprintf("PID: %d -> Used All Pages, First Add: 0x%x, First Val: 0x%x\n", currpid, ptr, *(ptr));
	restore(mask);
}
