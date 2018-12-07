// Import Libs
#include<xinu.h>

// Macro Vars
#define TST_HEX 0x1234ABCD
#define TST_BYT 4
#define TST_IT	100

// Proc
void testPP(unsigned int pg) {
	// Print
	intmask mask = disable();
	kprintf("PID: %d -> Using %d Pages\n", currpid, pg);
	restore(mask);

  // Get
	unsigned long i;
	unsigned long* ptr = (unsigned long*)vgetmem(pg * NBPG);

  // Set
	for(i=0; i < (pg * NBPG) / TST_BYT; i++) {
		*(ptr + i) = i;
	}

  // Check
	for(i=0; i < (pg * NBPG) / TST_BYT; i++) {
		if(ptr[i] != i) {
			// Print
			mask = disable();
			kprintf("PID: %d -> Something is Wrong Val: 0x%x at 0x%x\n", currpid, *(ptr + i), ptr + i);
			restore(mask);
		}
	}

	// Print
	mask = disable();
	kprintf("PID: %d -> Used All Pages, First Add: 0x%x, First Val: 0x%x, pfCount: %d\n", currpid, ptr, *(ptr), get_faults());
	kprintf("PID: %d -> Used All Pages, Last Add: 0x%x, First Val: 0x%x, pfCount: %d\n", currpid, ptr + ((pg * NBPG) / TST_BYT) - 1, *(ptr + ((pg * NBPG) / TST_BYT) - 1), get_faults());
	restore(mask);
}

// Proc
void testP(unsigned int pg) {
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

// Proc
void testPGF(unsigned int pg) {
	// Print
	intmask mask = disable();
	kprintf("PID: %d -> Using %d Pages with freemem()\n", currpid, pg);
	restore(mask);

	// Print
	kprintf("Iteration: ");

	// Iterate
	unsigned int j;
	for(j = 0; j < TST_IT; j++) {
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
		kprintf("%d", j);
		if(j < 10)
			kprintf("\b");
		else
			kprintf("\b\b");

		// Free
		vfreemem((char*)ptr, pg * NBPG);
	}

	// Print
	mask = disable();
	kprintf("\nPID: %d -> Done Using Heap\n", currpid);
	restore(mask);
}

// Proc
void testA() {
	// Print
	intmask mask = disable();
	kprintf("PID: %d -> Using 1 Byte\n", currpid);
	restore(mask);

	char* ptr = (char*)vgetmem(1);
	*ptr = 'A';

	// Print
	mask = disable();
	kprintf("PID: %d -> Used 1 Byte, First Add: 0x%x, First Val: %c\n", currpid, ptr, *(ptr));
	restore(mask);

	while(TRUE) {
		if(*ptr != 'A') {
			// Print
			mask = disable();
			kprintf("PID: %d -> Something is Wrong Val: %c\n", currpid, *(ptr));
			restore(mask);
		}
	}
}

// Proc
void testB() {
	// Print
	intmask mask = disable();
	kprintf("PID: %d -> Using 1 Byte\n", currpid);
	restore(mask);

	char* ptr = (char*)vgetmem(1);
	*ptr = 'B';

	// Print
	mask = disable();
	kprintf("PID: %d -> Used 1 Byte, First Add: 0x%x, First Val: %c\n", currpid, ptr, *(ptr));
	restore(mask);

	while(TRUE) {
		if(*ptr != 'B') {
			// Print
			mask = disable();
			kprintf("PID: %d -> Something is Wrong Val: %c\n", currpid, *(ptr));
			restore(mask);
		}
	}
}
