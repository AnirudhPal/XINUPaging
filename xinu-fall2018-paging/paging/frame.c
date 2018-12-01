// Import Libs
#include<xinu.h>

// Global Var
frame frametab[NFRAMES];
frame fifoHead;

// Initialize Frames
syscall initFrames() {
  // Disable Interrupts
  intmask mask = disable();


  // Set Dummy Head
  if(pgrpolicy == FIFO) {
  		fifoHead.type = FIFO_HEAD;                  // Set as Head
      fifoHead.fnum = 0;                          // Ignore
      fifoHead.pid = 0;                           // Ignore
      fifoHead.addr = NULL;                       // Ignore
      fifoHead.next = NULL;                       // Nothing on Q
  }

  // Loop and Set Free
  int i;
  for(i = 0; i < NFRAMES; i++) {
    frametab[i].type = FREE_FRAME;                // Set as Free
    frametab[i].fnum = i + FRAME0;                // Actual Frame Number
    frametab[i].pid = 0;                          // Default PID
    frametab[i].addr = frametab[i].fnum * NBPG;   // Physical Address
    frametab[i].next = NULL;                      // Next Frame in FIFO
  }

  // Restore and Return
  restore(mask);
  return OK;
}

// Print Frames that are not FREE
syscall printFrames() {
  // Disable Interrupts
  intmask mask = disable();

  // Loop and Set Free
  int i;
  for(i = 0; i < NFRAMES; i++) {
    // If not FREE
    if(frametab[i].type != FREE_FRAME) {
      // Print Frames
      kprintf("frametab[%d]: .type = %d .fnum = %d .pid = %d .addr = 0x%x\n", i, frametab[i].type, frametab[i].fnum, frametab[i].pid, frametab[i].addr);
    }
  }

  // Restore and Return
  restore(mask);
  return OK;
}

// Get a Frame from first 1000
unsigned int getDSFrame() {
  // Disable Interrupts
  intmask mask = disable();

  // Loop and Set Free
  int i;
  for(i = 0; i < NDSFRAMES; i++) {
    // If not FREE
    if(frametab[i].type == FREE_FRAME) {
      // Set PID
      frametab[i].pid = currpid;

      // Restore and Return
      restore(mask);
      return frametab[i].fnum;
    }
  }

  // Restore and Return
  restore(mask);
  return SYSERR;
}

// Get a Frame from last 2072
unsigned int getPFrame() {
  // Disable Interrupts
  intmask mask = disable();

  // Loop and Set Used
  int i;
  for(i = NDSFRAMES; i < NFRAMES; i++) {
    // If not FREE
    if(frametab[i].type == FREE_FRAME){
      // Set as Page
      frametab[i].type = PG_FRAME;

      // Set PID
      frametab[i].pid = currpid;

      // Add to Page Replacement FIFO
      if(pgrpolicy == FIFO) {
        addFifo(&frametab[i]);
      }

      // Restore and Return
      restore(mask);
      return frametab[i].fnum;
    }
  }

  // Use Page Replacement
  if(pgrpolicy == FIFO) {
    // Remove Page
    removeFifo();
    // Set and Return Page
  }

  // Restore and Return
  restore(mask);
  return SYSERR;
}

// Set Frame Type
syscall setFrameType(unsigned int type, unsigned int frameNum) {
  // Disable Interrupts
  intmask mask = disable();

  // Error Handeling
  if((type != PD_FRAME && type != PT_FRAME) || frameNum < FRAME0 || frameNum >= NFRAMES + FRAME0) {
    // Restore and Return
    restore(mask);
    return SYSERR;
  }

  // Set Frame Type
  frametab[frameNum - FRAME0].type = type;

  // Restore and Return
  restore(mask);
  return OK;
}

// Free Frames
syscall	freeFrames(pid32 pid) {
  // Disable Interrupts
  intmask mask = disable();

  // Loop and Set Free
  int i;
  for(i = 0; i < NFRAMES; i++) {
    // PID Match
    if(frametab[i].pid == pid) {
      // PT Match
      if(frametab[i].type == PT_FRAME) {
        // Hook
    		#ifdef VERBOSE
    		hook_ptable_delete((unsigned int)frametab[i].fnum);
    		#endif
      }
      frametab[i].type = FREE_FRAME;                // Set as Free
      frametab[i].fnum = i + FRAME0;                // Actual Frame Number
      frametab[i].pid = 0;                          // Default PID
      frametab[i].addr = frametab[i].fnum * NBPG;   // Physical Address
    }
  }

  // Restore and Return
  restore(mask);
  return OK;
}

// Add FIFO
syscall addFifo(frame* pFrame) {
  // Disable Interrupts
  intmask mask = disable();

  // Error
  if(pFrame == NULL) {
    restore(mask);
    return SYSERR;
  }

  // Get Head
  frame* Head = &fifoHead;

  // Go to End (Optimize of Required)
  while(Head->next != NULL) {
    Head = Head->next;
  }

  // Add Frame
  Head->next = pFrame;

  kprintf("Adding Frame: %d\n", pFrame->fnum);

  // Restore and Return
  restore(mask);
  return OK;
}

// Remove FIFO
syscall removeFifo() {
  // Disable Interrupts
  intmask mask = disable();

  // Error
  if(fifoHead.next == NULL) {
    restore(mask);
    return SYSERR;
  }

  kprintf("Removing Frame: %d\n", fifoHead.next->fnum);

  // Empty Frame
  fifoHead.next->type = FREE_FRAME;

  // Move Head Ahead
  fifoHead.next = fifoHead.next->next;

  // Restore and Return
  restore(mask);
  return OK;
}
