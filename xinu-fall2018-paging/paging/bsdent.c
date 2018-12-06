// Import Libs
#include<xinu.h>

// Global Var
bsdent bsdtab[NBSDS];

// Intialize Bsdents
syscall initBsds() {
  // Disable Interrupts
  intmask mask = disable();

  // Loop and Set Empty
  int i, j;
  for(i = 0; i < NBSDS; i++) {
    bsdtab[i].pid = 0;
    bsdtab[i].npages = 0;
    bsdtab[i].pages[BSD_PAGES];
    for(j = 0; j < BSD_PAGES; j++) {
      bsdtab[i].pages[j] = BS_IGNORE;
    }
  }

  // Restore and Return
  restore(mask);
  return OK;
}

// Add Maping
bsd_t addMapping(unsigned int npages, pid32 pid) {
  // Disable Interrupts
  intmask mask = disable();

  // Allocate
  bsd_t bsd = allocate_bs(npages);

  // Error
  if(bsd == SYSERR) {
    // Restore and Return
    kprintf("addMapping(): Couldn't Create BSD. npages: %d, pid: %d\n", npages, pid);
    restore(mask);
    return SYSERR;
  }

  // Add Entry
  bsdtab[bsd].pid = pid;
  bsdtab[bsd].npages = npages;
  int i;
  for(i = 0; i < npages; i++) {
    bsdtab[bsd].pages[i] = BS_NPRES;
  }

  // Restore and Return
  restore(mask);
  return bsd;
}

// Send to BS (Dirty Case Missing)
syscall sendBs(unsigned int fid) {
  // Disable Interrupts
  intmask mask = disable();

  // Open BSD
  bsd_t bsd = open_bs(proctab[frametab[fid].pid].prbsd);

  // Error
  if(bsd == SYSERR) {
    // Restore and Return
    kprintf("sendBs(): Couldn't Open BSD. fid: %d\n", fid);
    restore(mask);
    kill(frametab[fid].pid);
    return SYSERR;
  }

  // Send to BSD
  if(write_bs((char*)frametab[fid].addr, bsd, frametab[fid].vpn) == SYSERR) {
    // Restore and Return
    kprintf("sendBs(): Couldn't Write BSD. fid: %d\n", fid);
    restore(mask);
    kill(frametab[fid].pid);
    return SYSERR;
  }

  // Close BSD
  if(close_bs(proctab[frametab[fid].pid].prbsd) == SYSERR) {
    // Restore and Return
    kprintf("sendBs(): Couldn't Close BSD. fid: %d\n", fid);
    restore(mask);
    kill(frametab[fid].pid);
    return SYSERR;
  }

  // Modify Table
  bsdtab[proctab[frametab[fid].pid].prbsd].pages[frametab[fid].vpn] = BS_PRES;

  // Restore and Return
  restore(mask);
  return OK;
}

// Get from BS
syscall getBs(unsigned int fid) {
  // Disable Interrupts
  intmask mask = disable();

  // Error
  if(frametab[fid].pid != currpid) {
    // Restore and Return
    kprintf("getBs(): Not Owner. fid: %d, currpid: %d\n", fid, currpid);
    restore(mask);
    kill(currpid);
  }

  // Not Present
  if(bsdtab[proctab[currpid].prbsd].pages[frametab[fid].vpn] == BS_NPRES) {
    // Restore and Return
    restore(mask);
    return OK;
  }

  // Open BSD
  bsd_t bsd = open_bs(proctab[currpid].prbsd);

  // Error
  if(bsd == SYSERR) {
    // Restore and Return
    kprintf("getBs(): Couldn't Open BSD. fid: %d\n", fid);
    restore(mask);
    kill(currpid);
  }

  // Get from BSD
  if(read_bs((char*)frametab[fid].addr, bsd, frametab[fid].vpn) == SYSERR) {
    // Restore and Return
    kprintf("getBs(): Couldn't Read BSD. fid: %d\n", fid);
    restore(mask);
    kill(currpid);
  }

  // Close BSD
  if(close_bs(proctab[currpid].prbsd) == SYSERR) {
    // Restore and Return
    kprintf("getBs(): Couldn't Close BSD. fid: %d\n", fid);
    restore(mask);
    kill(currpid);
  }

  // Restore and Return
  restore(mask);
  return OK;
}

// Free Bsdents
syscall removeMapping(pid32 pid){
  // Disable Interrupts
  intmask mask = disable();

  // Error
  if(proctab[pid].prbsd == SYSERR) {
    // Restore and Return
    kprintf("removeMapping(): No BSD. pid: %d\n", pid);
    restore(mask);
    return SYSERR;
  }

  // Modify Table
  bsdtab[proctab[pid].prbsd].pid = 0;
  bsdtab[proctab[pid].prbsd].npages = 0;
  int i;
  for(i = 0; i < bsdtab[proctab[pid].prbsd].npages; i++) {
    bsdtab[proctab[pid].prbsd].pages[i] = BS_IGNORE;
  }

  // Deallocate
  if(deallocate_bs(proctab[pid].prbsd) == SYSERR) {
    // Restore and Return
    kprintf("removeMapping(): Couldn't Remove BSD. pid: %d\n", pid);
    restore(mask);
    return SYSERR;
  }

  // Restore and Return
  restore(mask);
  return OK;
}
