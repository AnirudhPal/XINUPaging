// Import Libs
#include <xinu.h>

// High Level Page Fault Handler
void pfhandler() {
  // Disable Interrupts
  intmask mask = disable();

  // Critical Section Start
  wait(prSem);

  // Find the Faulting Address
  unsigned long fadd = getCR2();

  // Hook
  #ifdef VERBOSE
  hook_pfault((char*)fadd);
  #endif

  // Error Code Handeling
  if((pfErrCode & 0x1) != 0) {
    kprintf("pfhandler(): Unexpected Error Code, Err: 0x%x\n", pfErrCode);
    restore(mask);
    kill(currpid);
    return;
  }

  // Error Handeling
  if(fadd < V_FRAME * NBPG || fadd > ((V_FRAME + proctab[currpid].prpages) * NBPG)) {
    kprintf("pfhandler(): Address out of Range, Add: 0x%x\n", fadd);
    restore(mask);
    kill(currpid);
    return;
  }

  // Increment Count
  pfCount++;

  // Compute PD
  pd_t* procPD = (pd_t*)(proctab[currpid].prpd * NBPG);

  // Check if PD has PDE
  unsigned int PDEInd = fadd >> 22;
  if(procPD[PDEInd].pd_pres == 0) {
    procPD[PDEInd].pd_pres = 1;
    procPD[PDEInd].pd_base = getPT();

    // Hook
    #ifdef VERBOSE
    hook_ptable_create((unsigned int)procPD[PDEInd].pd_base);
    #endif
  }

  // Compute PT
  pt_t* procPT = (pt_t*)(procPD[PDEInd].pd_base * NBPG);

  // Check if PT has PTE
  fadd = fadd << 10;
  unsigned int PTEInd = fadd >> (10 + 12);
  if(procPT[PTEInd].pt_pres == 0) {
    procPT[PTEInd].pt_pres = 1;
    procPT[PTEInd].pt_base = getPFrame(PTEInd);
  }

  // Flush TLB
  setPDBR(proctab[currpid].prpd);

  // Critical Section End
  signal(prSem);

  // Restore and Return
  restore(mask);
  return;
}
