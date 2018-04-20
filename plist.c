#include "plist.h"

// Maximum list size.
#define MAX_LIST        4

/*
 *******************************************************************************
 *                              Global Variables                               *
 *******************************************************************************
*/

// List buffer, list pointer.
static int list[MAX_LIST], lp;

/*
 *******************************************************************************
 *                                  Routines                                   *
 *******************************************************************************
*/

// Inserts pid into list. Aborts if list is full.
void insertProcess (int pid) {
    if (lp >= MAX_LIST) {
        fprintf(stderr, "Error: List is at capacity!\n");
        exit(EXIT_FAILURE);
    }
    list[lp++] = pid;
}

// Removes pid from list. 
void removeProcess (int pid) {
    int i, j;
    for (i = 0, j = 0; j < lp; i++, j++) {
        if (list[j] == pid) {
            continue;
        }
        list[i] = list[j];
    }
    lp = j;
}

// Returns process list.
const int *getProcesses (void) {
    return list;
}

// Returns the list size.
int getProcessCount (void) {
    return lp;
}