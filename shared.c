#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include "pmap.h"
#include "plist.h"

#define MAX_CHILDREN	3

/*
 *******************************************************************************
 *                              Global Variables                               *
 *******************************************************************************
*/


// Page size. 
int pagesize;

// Pointer to allocated memory page.
void *page;

// Process PID.
int pid;

// Aribiter PID.
int arbiter;

// Page owner PID.
int owner;

/*
 *******************************************************************************
 *                           Signal Handlers: Child                            *
 *******************************************************************************
*/

// Handler: Segmentation Fault.
void handler_sigsegv (int sig, siginfo_t *si, void *ignore) {
	const char *perms;

	// Extract permissions.
	if ((perms = getPerms(si->si_addr)) == NULL) {
		fprintf(stderr, "Error: handler_sigsegv: Can't locate permissions!\n");
		exit(EXIT_FAILURE);
	}

	// Verify permission. Okay if (read xor write) is true. 
	if (((perms[0] == 'r') ^ (perms[1] == 'w')) {
		fprintf(stderr, "Error: handler_sigsegv: Bad combo: \"%s\"!\n", perms);
		exit(EXIT_FAILURE); 
	}

	// If no-write: Set only write protections + freeze other processes.
	// If no-read: Set only read protections + resume other processes.
	if (perms[1] != 'w') {
		signal_takepage();
	} else {
		signal_droppage();
	}
	
}

/*
 *******************************************************************************
 *                          Signal Handlers: Arbiter                           *
 *******************************************************************************
*/

// Handler: SIGUSR1
void handler_sigusr1 (int sig, siginfo_t *si, void *ignore) {
	int i, n = getProcessCount(), sender = si->si_pid;
	const int *children = getProcesses();

	// Ensure sender is owner: Adjust for process abort case.
	if (sender != owner) {
		fprintf(stderr, "Error: SIGUSR1 sender isn't owner!\n");
		exit(EXIT_FAILURE);
	}
	
	// Read page from owner.
	// TODO: READ()
	

	// Write page to other processes, then resume them.
	for (i = 0; i < n; i++) {
		if (children[i] != owner) {
			// TODO: WRITE()
			kill(children[i], SIGCONT);
		}
	}

	// Set self back as owner.
	owner = arbiter;
}

// Handler: SIGUSR2
void handler_sigusr2 (int sig, siginfo_t *si, void *ignore) {
	int i, n = getProcessCount(), sender = si->si_pid;
	const int *children = getProcesses();

	// Set owner to sender.
	owner = sender;

	// Freeze other processes.
	for (i = 0; i < n; i++) {
		if (children[i] != owner) {
			kill(children[i], SIGSTOP);
		}
	}
}

/*
 *******************************************************************************
 *                               Child Routines                                *
 *******************************************************************************
*/ 

// Signals arbiter that process is now page owner. Sets write-only protection.
void signal_takepage (void) {

	// Set write-only permissions.
	if(mprotect(page, pagesize, PROT_WRITE) == -1) {
		fprintf(stderr, "Error: signal_takepage: Could not set permissions!\n");
		return;
	}
	
	// Signal arbiter.
	kill(arbiter, SIGUSR2);
}

// Signals arbiter that process has released page. Sets read-only protection.
void signal_droppage (void) {

	// Send page to arbiter.
	// TODO: WRITE()

	// Set read-only permissions.
	if (mprotect(page, pagesize, PROT_READ) == -1) {
		fprintf(stderr, "Error: signal_droppage: Could not set permissions!\n");
		return;
	}

	// Signal arbiter.
	kill(arbiter, SIGUSR1);
}

/*
 *******************************************************************************
 *                              Arbiter Routines                               *
 *******************************************************************************
*/

// Arbiter main process.
void aribter (void) {

	// Wait for child processes to finish. Remove them from list as neccessary.
	
	

}


/*
 *******************************************************************************
 *                           Signal Actions: Arbiter                           *
 *******************************************************************************
*/




// Returns permissions for given memory address. Returns NULL on failure.
const char *getPerms (void *addr) {
	char *perms = NULL;
	ProcMap pm;

	// Attempt to open process memory map.
	if (!openProcMaps(pid)) {
		return NULL;
	}

	// Locate region containing addr. 
	while (parseNext(&pm) != 0) {
		if (addr >= pm.startAddress && addr <= pm.endAddress) {
			return pm.perms;
		}
	}

	return NULL;	
}


