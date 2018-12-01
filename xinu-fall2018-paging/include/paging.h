/* paging.h */

//typedef unsigned int	 bsd_t;

/* Structure for a page directory entry */

typedef struct {

  unsigned int pd_pres	: 1;		/* page table present?		*/
  unsigned int pd_write : 1;		/* page is writable?		*/
  unsigned int pd_user	: 1;		/* is use level protection?	*/
  unsigned int pd_pwt	: 1;		/* write through cachine for pt?*/
  unsigned int pd_pcd	: 1;		/* cache disable for this pt?	*/
  unsigned int pd_acc	: 1;		/* page table was accessed?	*/
  unsigned int pd_mbz	: 1;		/* must be zero			*/
  unsigned int pd_fmb	: 1;		/* four MB pages?		*/
  unsigned int pd_global: 1;		/* global (ignored)		*/
  unsigned int pd_avail : 3;		/* for programmer's use		*/
  unsigned int pd_base	: 20;		/* location of page table?	*/
} pd_t;

/* Structure for a page table entry */

typedef struct {

  unsigned int pt_pres	: 1;		/* page is present?		*/
  unsigned int pt_write : 1;		/* page is writable?		*/
  unsigned int pt_user	: 1;		/* is use level protection?	*/
  unsigned int pt_pwt	: 1;		/* write through for this page? */
  unsigned int pt_pcd	: 1;		/* cache disable for this page? */
  unsigned int pt_acc	: 1;		/* page was accessed?		*/
  unsigned int pt_dirty : 1;		/* page was written?		*/
  unsigned int pt_mbz	: 1;		/* must be zero			*/
  unsigned int pt_global: 1;		/* should be zero in 586	*/
  unsigned int pt_avail : 3;		/* for programmer's use		*/
  unsigned int pt_base	: 20;		/* location of page?		*/
} pt_t;

/* Prototypes for required API calls */
//SYSCALL xmmap(int, bsd_t, int);
//SYSCALL xmunmap(int);

#define NBPG		4096	/* number of bytes per page	*/
#define FRAME0		1024	/* zero-th frame		*/
#define NFRAMES		1050//3072	/* number of frames		*/

#define MAP_SHARED 1
#define MAP_PRIVATE 2

#define FIFO 3
#define MYPOLICY 4

#define MAX_ID		7		/* You get 8 mappings, 0 - 7 */
#define MIN_ID          0

/** Anirudh Pal Stuff **/
/* Macro Vars */
// Frame Types
#define FREE_FRAME    0
#define PD_FRAME      1
#define PT_FRAME      2
#define PG_FRAME      3
#define FIFO_HEAD     4

// Frame Entries
#define FRAME_ENTRIES 1024

// Dev Section
#define DEV_PDE       576
#define DEV_FRAME     589824

// Virtual Frame
#define V_FRAME       4096

// Data Structure Frames
#define NDSFRAMES     1000

// Page Fault
#define PF_VECTOR     14

// Hooks
//#define VERBOSE       1

// Shared PT Count
#define SPTS          5

// Page Replacement Policies
#define FIFO          0
#define GLB_CLK       1
#define NONE          2

/* Structures */
// Frame Data Structure
typedef struct {
  unsigned int type;   // Type of Frame
  unsigned int fnum;   // Actual Frame Number
  pid32 pid;           // PID of Creater
  unsigned long addr;  // Physical Address
  struct frame* next;         // Next Frame in FIFO (Only in Virtual Pages)
} frame;

// Virtual Address Structure
typedef struct {
  unsigned int offset: 12; // Page Offset
  unsigned int pte : 10;   // Page Table Entry
  unsigned int pde : 10;   // Page Directory Entry
} vadd_t;

// CR0 Structure
typedef struct {
  unsigned long lsb : 31;  // Ignore
  unsigned int pg : 1;     // Paging
} cr0_t;

// CR3 Structure
typedef struct {
  unsigned int lsb : 12;     // Ignore
  unsigned long frame : 20;  // Frame Number
} cr3_t;

// Page Fault Error Code Structure
typedef struct {
  unsigned int p : 1;      // Present Bit
  unsigned long msb : 31;  // Ignore
} pfErrCode_t;

/* Global Vars */
// Page Fault Error Code
unsigned long pfErrCode;

// Page Fault Count
unsigned long pfCount;

// Page Replacement Policy
uint16 pgrpolicy;

// FIFO Dummy Head
frame fifoHead;

// Frame Table
extern frame frametab[NFRAMES];

// Shared PTs
extern unsigned int sharedPTs[SPTS];
