Lab 5 Part 2 - Anirudh Pal
===

Modified/Created Files
---

### Created

* system/createApps.c
* paging/frame.c
* paging/page.c
* paging/pregs.c
* system/vcreate.c
* system/vgetmem.c
* system/vfreemem.c
* system/pfisr.S
* system/pfhandler.c
* system/createVApps.c
* system/hooks.c
* paging/bsdent.c

### Modified

* include/paging.h
* include/prototypes.h
* system/main.c
* system/create.c
* system/initialize.c
* include/process.h
* system/resched.c
* system/kill.c
* include/rdisksys.h
* rdserver/rdisksys.h

Implementations
---

The implementation is completed with no parts missing the following is a list of major files with a break down of all the features that they help support and an explanation of how that enables demand paging. I have noticed some odd behavior with concurrent processes. Please contact me if you run into that.

### frame.c

It contains all functions related with manipulating the data structure that tracks the frames called frametab[]. Here are some of the functions:

#### initFrames()

This function initialize the entries of frametab[]. All of them are thus available.

#### getDSFrame()

This function is used to get a frame for page tables and page directories. It obtains frames from the first 1000 in frametab[]. It iterates through the frametab[] looking for an empty frame.

#### getPFrame()

This function is used to get a frame for pages. It obtains frames from the last 2072 in frametab[]. It iterates through the frametab[] looking for an empty frame. In the backing store version it will defer to a FIFO list if it cant find an empty frame.

#### freeFrames()

This function sets all frames in frametab[] associated to a pid to free so they can be used by others. It also pulls them from the FIFO.

#### addFifo()

This function is used to add a frame to the FIFO.

#### removeFifo()

This function is used to get a frame from FIFO.

#### deleteFifo()

This function is used to remove a specific frame from the FIFO.

#### removeRand()

This function gets a random frame from the frametab[].

### page.c

It contains functions that directly manipulating the frame location and set up page tables and page directories.

#### setSharedPTs()

This function is used to initialize 5 page tables that do the identity map up to frame 4096 and also the devices section.

#### getPD()

This function is used to get a page directory setup with the shared page directory entries already pre populated.

#### getPT()

This function is used to get an empty page table.

#### updatePT()

This function sets an entry of a page table to not present when it is being switched out.

#### isDirty()

This function returns true if a page is dirty.

### bsdent.c

It contains functions associated with interacting and mapping the backing store. We use a table called bsdtab[] to make this mapping happen.

#### initBsds()

Initializes the data structure with empty entries. Each entry has an associated bsd_t, pid, page count and page presence table.

#### addMapping()

This sets one entry up with a process.

#### removeMapping()

This removes that association.

#### sendBs()

This function is used to send a particular frame to the backing store and update the bsdtab[].

#### getBs()

This function is used to get a particular frame from the backing store.

### pregs.c

#### enablePaging()

This function is used to set the PG bit of CR0 to enable paging.

#### setPDBR()

This function is used to set the page directory of the current process. It also flushes the TLB.

#### getCR2()

This function is used to get the CR2 value which store that fault causing virtual address during the page fault.

#### pgrpolicyC()

System call used to change page replacement policy. Had to be renamed due to name conflict.

### pfisr.c

The assembly code gets the page fault error code, save the stack, calls the high level handler, restores the stack and then returns to instruction that caused the page fault.

### pfhandler.c

This is run when there is a page fault. It uses the CR2 to figure out what page directory entries, page table, page table entries and pages need to be set and then sets them allowing the virtual address to work.

### vcreate.c

Modified create() to support private heap. It sets the dummy head of the private heap.

### vgetmem.c

Modified getmem() that operates in the virtual heap area.

### vfreemem.c

Modified freemem() that operates in the virtual heap area.

### initialize.c

This is where we can first turn on paging. It sets up the null process.

#### initialize_paging()

Initializes frametab[], shared page tables, page directory for null process, sets the CR3 and CR0.

### resched.c

Only addition was to switch the CR3 value to the page directory of the process that is being context switched in.

### kill.c

Only addition was to delete all frames that have been obtained by the dying process.

Backing Store Mapping
---

We find the pid and virtual page number of a given frame using the inverted page table data structure called frametab[]. The we use the pid to find the associated entry in bsdtab[] by looking up the entry number in the proctab[]. Then we use the virtual page number to get the associated offset in the store associated with the pid. There are multiple structures that help achieve this efficiently which include frametab[], bsdent[] and proctab[]. This is done without searching and by indexing the structures.

BONUS
---

I have made an alternative page replacement policy which basically just pics a random entry to replace. This is actually as effective as FIFO since both can lead to multiple page faults, but the random policy has a more predictable distribution. It also has the added advantage of being a lot smaller that FIFO and also require little to no mutual exclusion to achieve the page replacement. You can uncomment the policy setting portion in initialize_paging() in system/initialize.c. I would avoid using the pgrpolicyC() function since their are inconsistencies between the handout and the provided skeleton code. An OS has to run between multiple user applications and a concise and simple page replacement policy might be ideal in many situtations. 

Testing
---

As stated in the last report I don't have sti or cli in my pfisr(). I use semaphores to ensure paging data structures are not mangled because of blocking backing store calls.

Here is the break down of the test I performed:

1. I tested procs that don't require backing store.

2. I tested procs that did require the backing store of varying page sizes.

3. I also performed a memory thrashing test that would only have 1 frame available to it.

4. Finally all of these tests were run together so as to ensure that multiple processes can use the backing store.

5. All testing would get, set and check values in the virtual address space to pass.

Note
---

I enable hooks with a macro called VERBOSE in include/paging.h. Uncomment it to get hook output.

Commit Log
---

1. Set good structure, macros and global vars.
2. Removed backing store and shell from main. Added a normal app that uses heap.
3. Added support for initialize, get, set, print and free frames.
4. Testing frames functions.
5. Added support for initialize, get, set and print PDs and PTs.
6. Testing pages functions.
7. Turning on paging and adding all necessary DS to procs.
8. Added support for heap.
9. Testing heap without hooks.
10. Testing heap with hooks (STI/CLI problem).
11. More heap testing.
12. Added kill free frames.
13. Enable all hooks.
14. Added vfreemem() and tests.
15. Added more testing.
16. Added address check in pfhandler().
17. Added fifo structure and functions.
18. Changed and enabled backing store server.
19. Added backing store id to procs.
20. Made sll for fifo a dll.
21. Add vpn to frametab[].
22. Add bsdtab[] and functions.
23. Crude implementation of entire lab.
24. Added error prints.

Virtual Heap Test Output
---

```
Testing 1 Page Usage
PID: 5 -> Using 1 Pages
PID: 5 -> Used All Pages, First Add: 0x1000000, First Val: 0x0, pfCount: 1
PID: 5 -> Used All Pages, Last Add: 0x1000ffc, First Val: 0x3ff, pfCount: 1

Testing 2 Page Usage
PID: 6 -> Using 2 Pages
PID: 6 -> Used All Pages, First Add: 0x1000000, First Val: 0x0, pfCount: 3
PID: 6 -> Used All Pages, Last Add: 0x1001ffc, First Val: 0x7ff, pfCount: 3

Testing 100 Page Usage
PID: 7 -> Using 100 Pages
PID: 7 -> Used All Pages, First Add: 0x1000000, First Val: 0x0, pfCount: 203
PID: 7 -> Used All Pages, Last Add: 0x1063ffc, First Val: 0x18fff, pfCount: 204

Testing 200 Page Usage
PID: 8 -> Using 200 Pages
PID: 8 -> Used All Pages, First Add: 0x1000000, First Val: 0x0, pfCount: 604
PID: 8 -> Used All Pages, Last Add: 0x10c7ffc, First Val: 0x31fff, pfCount: 605

Testing 200 Page Usage
PID: 9 -> Using 200 Pages
PID: 9 -> Used All Pages, First Add: 0x1000000, First Val: 0x0, pfCount: 1005
PID: 9 -> Used All Pages, Last Add: 0x10c7ffc, First Val: 0x31fff, pfCount: 1006

Testing 200 Page Usage
PID: 10 -> Using 200 Pages

Testing 200 Page Usage
PID: 11 -> Using 200 Pages
PID 10 -> Done.
PID 11 -> Done.
```
