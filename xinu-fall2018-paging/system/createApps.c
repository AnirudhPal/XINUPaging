// Import Libs
#include<xinu.h>

// Macro Vars
#define STK_MEM 1024
#define TST_HEX 0x1234ABCD
#define TST_BYT 4

// Test Stack
void testHeap() {
	// Print
	kprintf("PID: %d -> Using Stack\n", currpid);

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
			kprintf("PID: %d -> Something is Wrong, Val: 0x%x\n", currpid, *(ptr + i));
		}
	}

	// Print
	kprintf("PID: %d -> Used Stack, First Add: 0x%x, First Val: 0x%x\n", currpid, ptr, *(ptr));
}

// Test Frames
void testFrames() {
  // Initialize Frames
  initFrames();

  // Get All DS Frames
  int i;
  kprintf("DS frameNum: ");
  for(i = 0; i < NDSFRAMES; i++) {
    // Get DS Frame
    unsigned int frameNum = getDSFrame();

    // Parse Frame Number
    kprintf("%d\b\b\b\b", frameNum);

    // Set as PD
    setFrameType(PD_FRAME, frameNum);
  }
  kprintf("\n");

  // Print Frames
  printFrames();

  // Get All P Frames
  int i;
  kprintf("P frameNum: ");
  for(i = NDSFRAMES; i < NFRAMES; i++) {
    // Get DS Frame
    unsigned int frameNum = getPFrame();

    // Parse Frame Number
    kprintf("%d\b\b\b\b", frameNum);
  }
  kprintf("\n");

  // Print Frames
  printFrames();

  // Free Frames
  freeFrames(currpid);

  // Print Frames
  printFrames();
}
