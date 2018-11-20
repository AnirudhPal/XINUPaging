Lab 5 Part 1 - Anirudh Pal
===

Modified/Created Files
---

### Created

* system/createApps.c
* paging/frame.c
* paging/page.c
* paging/pregs.c

### Modified

* include/paging.h
* include/prototypes.h
* system/main.c
* system/create.c
* system/initialize.c
* include/process.h
* system/resched.c

Implementations
---

Testing
---

Commit Log

1. Set good structure, macros and global vars.
2. Removed backing store and shell from main. Added a normal app that uses heap.
3. Added support for initialize, get, set, print and free frames.
4. Testing frames functions.
5. Added support for initialize, get, set and print PDs and PTs.
6. Testing pages functions.
7. Turning On Paging and Adding all necessary DS to Procs.
