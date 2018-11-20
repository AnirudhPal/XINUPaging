// Import Libs
#include<xinu.h>

// Macro Vars
#define STK_MEM 1024
#define TST_HEX 0x1234ABCD
#define TST_BYT 4

// Test Stack
void testHeap() {
	// Print
	intmask mask = disable();
	kprintf("PID: %d -> Using Stack\n", currpid);
	restore(mask);

  // Get
	int i;
	unsigned long* ptr = (unsigned long*)getmem(STK_MEM);

  // Set
  for(i = 0; i < STK_MEM / TST_BYT; i++) {
		*(ptr + i) = 0x123ABCEF;
	}

  // Check
	for(i = 0; i < STK_MEM / TST_BYT; i++) {
		if(*(ptr + i) != 0x123ABCEF) {
			// Print
			mask = disable();
			kprintf("PID: %d -> Something is Wrong, Val: 0x%x\n", currpid, *(ptr + i));
			restore(mask);
		}
	}

	// Print
	mask = disable();
	kprintf("PID: %d -> Used Stack, First Add: 0x%x, First Val: 0x%x\n", currpid, ptr, *(ptr));
	restore(mask);
}
