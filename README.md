Lab 5 Part 1 - Anirudh Pal
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

### Modified

* include/paging.h
* include/prototypes.h
* system/main.c
* system/create.c
* system/initialize.c
* include/process.h
* system/resched.c
* system/kill.c

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
