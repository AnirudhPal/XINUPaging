// Modifed getmem() - Anirudh Pal
#include <xinu.h>

/*------------------------------------------------------------------------
 *  vgetmem  -  Allocate heap storage, returning lowest word address
 *------------------------------------------------------------------------
 */
char  	*vgetmem(
	  uint32	nbytes		/* Size of memory requested	*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	heapblk	*prev, *curr, *leftover;

	mask = disable();
	if (nbytes == 0) {
		restore(mask);
		return (char *)SYSERR;
	}

	nbytes = (uint32) roundmb(nbytes);	/* Use memblk multiples	*/

  // Changed - Anirudh Pal
	prev = &(proctab[currpid].prvheap);
	curr = prev->mnext;

  // Initialize
  if(!proctab[currpid].prhasheap) {
		proctab[currpid].prhasheap = TRUE;
		curr->mnext = NULL;
		curr->mlength = prev->mlength;	
	}

	while (curr != NULL) {			/* Search free list	*/

		if (curr->mlength == nbytes) {	/* Block is exact match	*/
			prev->mnext = curr->mnext;
			memlist.mlength -= nbytes;
			restore(mask);
			return (char *)(curr);

		} else if (curr->mlength > nbytes) { /* Split big block	*/
			leftover = (struct heapblk *)((uint32) curr +
					nbytes);
			prev->mnext = leftover;
			leftover->mnext = curr->mnext;
			leftover->mlength = curr->mlength - nbytes;
			memlist.mlength -= nbytes;
			restore(mask);
			return (char *)(curr);
		} else {			/* Move to next block	*/
			prev = curr;
			curr = curr->mnext;
		}
	}
	restore(mask);
	return (char *)SYSERR;
}
