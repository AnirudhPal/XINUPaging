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

The implementation is completed with no parts missing the following is a list of major files with a break down of all the features that they help support and an explanation of how that enables demand paging.

### frame.c

It contains all functions related with manipulating the data structure that tracks the frames called frametab[]. Here are some of the functions:

#### initFrames()

This function initialize the entries of frametab[]. All of them are thus available.

#### getDSFrame()

This function is used to get a frame for page tables and page directories. It obtains frames from the first 1000 in frametab[]. It iterates through the frametab[] looking for an empty frame.

#### getPFrame()

This function is used to get a frame for pages. It obtains frames from the last 2072 in frametab[]. It iterates through the frametab[] looking for an empty frame.

#### freeFrames(pid32)

This function sets all frames in frametab[] associated to a pid to free so they can be used by others.

### page.c

It contains functions that directly manipulating the frame location and set up page tables and page directories.

#### setSharedPTs()

This function is used to initialize 5 page tables that do the identity map up to frame 4096 and also the devices section.

#### getPD()

This function is used to get a page directory setup with the shared page directory entries already pre populated.

#### getPT()

This function is used to get an empty page table.

### pregs.c

#### enablePaging()

This function is used to set the PG bit of CR0 to enable paging.

#### setPDBR()

This function is used to set the page directory of the current process. It also flushes the TLB.

#### getCR2()

This function is used to get the CR2 value which store that fault causing virtual address during the page fault.

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

Testing
---

As stated in the last report I don't have sti or cli in my pfisr(). I use

Here is the break down of the test I performed:

1. Tested a proc that uses the standard heap and not the virtual heap.

2. Tested the frame.c and page.c functions without actually having paging on with emulated requests. Output is at the end.

3. Tested a procs that uses virtual heap with vgetmem and freemem that get, set and check the values in the heap. I checked with 1 Page, 2 Pages, 1024 Pages and 2072 Pages.

4. Ran two procs that write to the same virtual address in the heap and made sure the value were as expected.

5. Output of the virtual heap tests are at the end.

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

```
