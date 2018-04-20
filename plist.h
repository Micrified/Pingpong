#if !defined(PLIST_H)
#define PLIST_H

#include <stdlib.h>

/*
 *******************************************************************************
 *                                  Routines                                   *
 *******************************************************************************
*/

// Inserts pid into list. Aborts if list is full.
void insertProcess (int pid);

// Removes pid from list. 
void removeProcess (int pid);

// Returns process list.
const int *getProcesses (void);

// Returns the list size.
int getProcessCount (void);


#endif
