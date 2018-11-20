/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>

process	main(void)
{

	/* Start the network */
	/* DO NOT REMOVE OR COMMENT THIS CALL */
	//netstart();

	/* Initialize the page server */
	/* DO NOT REMOVE OR COMMENT THIS CALL */
	//psinit();

	// Testing create() Proc
	kprintf("\nTesting Normal Heap\n");
	resume(create(testHeap, 1024, 50, "testHeap", 0));
	/**
	kprintf("\nTesting Frames\n");
	resume(create(testFrames, 1024, 50, "testFrames", 0));
	kprintf("\nTesting Pages\n");
	resume(create(testPages, 1024, 50, "testPages", 0));
	**/

	// Testing vcreate() Proc
	kprintf("\nTesting 1 Page Usage\n");
	resume(vcreate(testP, 3072, 1, 50, "testP", 1, 1));
	kprintf("\nTesting 2 Page Usage\n");
	resume(vcreate(testP, 3072, 2, 50, "testP", 1, 2));
	kprintf("\nTesting 1024 Page Usage\n");
	resume(vcreate(testP, 3072, 1024, 50, "testP", 1, 1024));
	kprintf("\nTesting 2072 Page Usage\n");
	resume(vcreate(testP, 3072, 2072, 50, "testP", 1, 2072));
	kprintf("\nTesting 2072 Page Usage\n");
	resume(vcreate(testP, 3072, 2072, 50, "testP", 1, 2072));

	/**
	kprintf("\n...creating a shell\n");
	recvclr();
	resume(create(shell, 8192, 50, "shell", 1, CONSOLE));
	**/
	/* Wait for shell to exit and recreate it */

	/**
	while (TRUE) {
		receive();
		sleepms(200);
		kprintf("\n\nMain process recreating shell\n\n");
		resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
	}
	return OK;
	**/
}
