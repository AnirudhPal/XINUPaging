// Import Libs
#include<xinu.h>

// Global Var
frame frametab[NFRAMES];

// Initialize Frames
syscall initFrames() {
  // Disable Interrupts
  intmask mask = disable();

  // Loop and Set Free
  int i;
  for(i = 0; i < NFRAMES; i++) {
    frametab[i].type = FREE_FRAME;                // Set as Free
    frametab[i].fnum = i + FRAME0;                // Actual Frame Number
    frametab[i].pid = 0;                          // Default PID
    frametab[i].addr = frametab[i].fnum * NBPG;   // Physical Address
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

  // Loop and Set Free
  int i;
  for(i = NDSFRAMES; i < NFRAMES; i++) {
    // If not FREE
    if(frametab[i].type == FREE_FRAME){
      // Set as Page
      frametab[i].type = PG_FRAME;

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
