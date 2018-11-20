// Import Libs
#include <xinu.h>

// High Level Page Fault Handler
void pfhandler() {
	// Find the Faulting Address
	unsigned long fadd = getCR2();

	// Hook
	#ifdef TALK
	//hook_pfault((char*)fadd);
	#endif

	// Increment Count
	pfCount++;

	// Compute PD
	pd_t* procPD = (pd_t*)(proctab[currpid].prpd * NBPG);

	// Check if PD has PDE
	unsigned int PDEInd = (vadd_t)(fadd).pde;
	if(procPD[PDEInd].pd_pres == 0) {
		procPD[PDEInd].pd_pres = 1;
		procPD[PDEInd].pd_write = 1;
		procPD[PDEInd].pd_base = getPT();

		// Hook
		#ifdef TALK
		//hook_ptable_create((unsigned int)(frametab[procPD[PDEInd].pd_base - FRAME0].loc));
		#endif
	}

	// Compute PT
	pt_t* procPT = (pt_t*)(procPD[PDEInd].pd_base * NBPG);

	// Check if PT has PTE
	unsigned int PTEInd = (vadd_t)(fadd).pte;
	if(procPT[PTEInd].pt_pres == 0) {
		procPT[PTEInd].pt_pres = 1;
		procPT[PTEInd].pt_write = 1;
		procPT[PTEInd].pt_base = getPFrame();
	}
}
