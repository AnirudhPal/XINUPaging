// Import Libs
#include<xinu.h>

// Macro Vars
#define TST_HEX 0x1234ABCD
#define TST_BYT 4

// Proc
void testP(unsigned pg) {
	// Print
	intmask mask = disable();
	kprintf("PID: %d -> Using %d Pages\n", currpid, pg);
	restore(mask);

  // Get
	unsigned long i;
	unsigned long* ptr = (unsigned long*)vgetmem(pg * NBPG);

  // Set
	for(i=0; i < (pg * NBPG) / TST_BYT; i++) {
		*(ptr + i) = TST_HEX;
	}

  // Check
	for(i=0; i < (pg * NBPG) / TST_BYT; i++) {
		if(ptr[i] != TST_HEX) {
			// Print
			mask = disable();
			kprintf("PID: %d -> Something is Wrong Val: 0x%x at 0x%x\n", currpid, *(ptr + i), ptr + i);
			restore(mask);
		}
	}

	// Print
	mask = disable();
	kprintf("PID: %d -> Used All Pages, First Add: 0x%x, First Val: 0x%x, pfCount: %d\n", currpid, ptr, *(ptr), get_faults());
	restore(mask);
}
