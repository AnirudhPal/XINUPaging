// Import Libs
#include<xinu.h>

// Global Var
unsigned int sharedPTs[SPTS];
unsigned long pfErrCode;
unsigned long pfCount;

// Sets the Shared Page Tables
syscall	setSharedPTs() {
  // Disable Interrupts
  intmask mask = disable();

  // Set 4 PTs
  int i;
  for(i = 0; i < SPTS - 1; i++) {
    // Get Frame
    unsigned int frameNum = getDSFrame();

    // Error Handeling
    if(frameNum == (unsigned int)SYSERR) {
      // Restore and Return
      restore(mask);
      return SYSERR;
    }

    // Set as PT
    setFrameType(PT_FRAME, frameNum);

    // Get Pointer
    pt_t* sPT = (pt_t*) (frameNum * NBPG);

    // Set Values
    int j;
    for(j = 0; j < FRAME_ENTRIES; j++) {
      sPT[j].pt_pres = 1;
      sPT[j].pt_write = 1;
      sPT[j].pt_user = 0;
      sPT[j].pt_pwt = 0;
      sPT[j].pt_pcd = 0;
      sPT[j].pt_acc = 0;
      sPT[j].pt_dirty = 0;
      sPT[j].pt_mbz = 0;
      sPT[j].pt_global = 0;
      sPT[j].pt_avail = 0;
      sPT[j].pt_base = i * FRAME_ENTRIES + j;
    }

    // Set in Table
    sharedPTs[i] = frameNum;
  }

  // Set Dev PT
  // Get Frame
  unsigned int frameNum = getDSFrame();

  // Error Handeling
  if(frameNum == (unsigned int)SYSERR) {
    // Restore and Return
    restore(mask);
    return SYSERR;
  }

  // Set as PT
  setFrameType(PT_FRAME, frameNum);

  // Get Pointer
  pt_t* sPT = (pt_t*) (frameNum * NBPG);

  // Set Values
  int j;
  for(j = 0; j < FRAME_ENTRIES; j++) {
    sPT[j].pt_pres = 1;
    sPT[j].pt_write = 1;
    sPT[j].pt_user = 0;
    sPT[j].pt_pwt = 0;
    sPT[j].pt_pcd = 0;
    sPT[j].pt_acc = 0;
    sPT[j].pt_dirty = 0;
    sPT[j].pt_mbz = 0;
    sPT[j].pt_global = 0;
    sPT[j].pt_avail = 0;
    sPT[j].pt_base = DEV_FRAME + j;
  }

  // Set in Table
  sharedPTs[i] = frameNum;

  // Restore and Return
  restore(mask);
  return OK;
}

// Generates a PD with Shared PTEs
unsigned int getPD() {
  // Disable Interrupts
  intmask mask = disable();

  // Get Frame
  unsigned int frameNum = getDSFrame();

  // Error Handeling
  if(frameNum == (unsigned int)SYSERR) {
    // Restore and Return
    restore(mask);
    return SYSERR;
  }

  // Set as PD
  setFrameType(PD_FRAME, frameNum);

  // Get Pointer
  pd_t* pPD = (pd_t*) (frameNum * NBPG);

  // Set Values
  int i;
  for(i = 0; i < FRAME_ENTRIES; i++) {
    pPD[i].pd_pres = 0;
    pPD[i].pd_write = 1;
    pPD[i].pd_user = 0;
    pPD[i].pd_pwt = 0;
    pPD[i].pd_pcd = 0;
    pPD[i].pd_acc = 0;
    pPD[i].pd_mbz = 0;
    pPD[i].pd_fmb = 0;
    pPD[i].pd_global = 0;
    pPD[i].pd_avail = 0;
    pPD[i].pd_base = 0;
  }

  // Set Shared PTEs
  for(i = 0; i < SPTS - 1; i++) {
    pPD[i].pd_pres = 1;
    pPD[i].pd_base = sharedPTs[i];
  }
  pPD[DEV_PDE].pd_pres = 1;
  pPD[DEV_PDE].pd_base = sharedPTs[SPTS - 1];

  // Restore and Return
  restore(mask);
  return frameNum;
}

// Generate a PT
unsigned int getPT() {
  // Disable Interrupts
  intmask mask = disable();

  // Get Frame
  unsigned int frameNum = getDSFrame();

  // Error Handeling
  if(frameNum == (unsigned int)SYSERR) {
    // Restore and Return
    restore(mask);
    return SYSERR;
  }

  // Set as PT
  setFrameType(PT_FRAME, frameNum);

  // Get Pointer
  pt_t* pPT = (pt_t*) (frameNum * NBPG);

  // Set Values
  int i;
  for(i = 0; i < FRAME_ENTRIES; i++) {
    pPT[i].pt_pres = 0;
    pPT[i].pt_write = 1;
    pPT[i].pt_user = 0;
    pPT[i].pt_pwt = 0;
    pPT[i].pt_pcd = 0;
    pPT[i].pt_acc = 0;
    pPT[i].pt_dirty = 0;
    pPT[i].pt_mbz = 0;
    pPT[i].pt_global = 0;
    pPT[i].pt_avail = 0;
    pPT[i].pt_base = 0;
  }

  // Restore and Return
  restore(mask);
  return frameNum;
}

// Update PT
syscall updatePT(unsigned int fid) {
  // Disable Interrupts
  intmask mask = disable();

  // Get PD Pointer
  pd_t* pPD = (pd_t*)(proctab[frametab[fid].pid].prpd * NBPG);

  // Get PT Pointer (Is this Okay?)
  pt_t* pPT = (pt_t*)(pPD[SPTS].base * NBPG);

  // Change PT Entry
  pPT[frametab[fid].vpn].pt_pres = 0;

  // Restore and Return
  restore(mask);
  return OK;
}

// Print PDEs
syscall	printPD(unsigned int frameNum) {
  // Disable Interrupts
  intmask mask = disable();

  // Get Pointer
  pd_t* pPD = (pd_t*) (frameNum * NBPG);

  // Print
  kprintf("PD at Frame %d\n", frameNum);
  int i;
  for(i = 0; i < FRAME_ENTRIES; i++) {
    if(pPD[i].pd_pres == 1) {
      kprintf("pPD[%d]: .pd_pres = %d .pd_base = %d\n", i, pPD[i].pd_pres, pPD[i].pd_base);
    }
  }

    // Restore and Return
    restore(mask);
    return OK;
}

// Print PTEs
syscall	printPT(unsigned int frameNum) {
  // Disable Interrupts
  intmask mask = disable();

  // Get Pointer
  pt_t* pPT = (pt_t*) (frameNum * NBPG);

  // Print
  kprintf("PT at Frame %d\n", frameNum);
  int i;
  for(i = 0; i < FRAME_ENTRIES; i++) {
    if(pPT[i].pt_pres == 1) {
      kprintf("pPT[%d]: .pt_pres = %d .pt_base = %d\n", i, pPT[i].pt_pres, pPT[i].pt_base);
    }
  }

  // Restore and Return
  restore(mask);
  return OK;
}
