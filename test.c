#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "pmap.h"

ProcMap pm;

char b[1024];

int main (void) {
	int pid = getpid();

	printf("I am a new process, with PID = %d\n", pid);

	printf("Printing Scanned Contents...\n\n");

	if (openProcMaps(pid) == 0) {
		return 0;
	}

	while (parseNext(&pm) == 1) {
		printf("startAddress = %llu\n", pm.startAddress);
		printf("endAddress = %llu\n", pm.endAddress);
		printf("offset = %llu\n", pm.offset);
		printf("perms = %s\n", pm.perms);
		printf("devMajor = %d\n",pm.devMajor);
		printf("devMinor = %d\n", pm.devMinor);
		printf("inode = %d\n", pm.inode);
		printf("path = \"%s\"\n", pm.filePath);
		putchar('\n');
	}

	closeProcMaps();

	printf("Now dumping contents...\n");
	openProcMaps(pid);

	while (fgets(b, 1024, fp) != NULL) {
		printf("%s", b);
	}
	closeProcMaps();
	printf("\nDone.\n");

	return 0;
}