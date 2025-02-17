// Modifed freemem() - Anirudh Pal
#include <xinu.h>

/*------------------------------------------------------------------------
 *  vfreemem  -  Free a memory block, returning the block to the free list
 *------------------------------------------------------------------------
 */
syscall	vfreemem(
	  char		*blkaddr,	/* Pointer to memory block	*/
	  uint32	nbytes		/* Size of block in bytes	*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	heapblk	*next, *prev, *block;
	uint32	top;

	mask = disable();
	if (!proctab[currpid].prhasheap || (nbytes == 0) || ((uint32) blkaddr < (uint32) (V_FRAME * NBPG)) || ((uint32) blkaddr > (uint32) (V_FRAME + proctab[currpid].prpages) * NBPG)) {
		kprintf("vfreemem(): Error.\n");
		restore(mask);
		return SYSERR;
	}

	nbytes = (uint32) roundmb(nbytes);	/* Use memblk multiples	*/
	block = (struct heapblk *)blkaddr;

  prev = &(proctab[currpid].prvheap);
	next = prev->mnext;
	while ((next != NULL) && (next < block)) {
		prev = next;
		next = next->mnext;
	}

	if (prev == &(proctab[currpid].prvheap)) {		/* Compute top of previous block*/
		top = (uint32) NULL;
	} else {
		top = (uint32) prev + prev->mlength;
	}

	/* Ensure new block does not overlap previous or next blocks	*/

	if (((prev != &(proctab[currpid].prvheap)) && (uint32) block < top)
	    || ((next != NULL)	&& (uint32) block+nbytes>(uint32)next)) {
		restore(mask);
		return SYSERR;
	}

	memlist.mlength += nbytes;

	/* Either coalesce with previous block or add to free list */

	if (top == (uint32) block) { 	/* Coalesce with previous block	*/
		prev->mlength += nbytes;
		block = prev;
	} else {			/* Link into list as new node	*/
		block->mnext = next;
		block->mlength = nbytes;
		prev->mnext = block;
	}

	/* Coalesce with next block if adjacent */

	if (((uint32) block + block->mlength) == (uint32) next) {
		block->mlength += next->mlength;
		block->mnext = next->mnext;
	}
	restore(mask);
	return OK;
}
