#if !defined(PMAP_H)
#define PMAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Type decribing a single entry in a process memory map.
// Optional fields are only available if memory region mapped from file.
typedef struct {
    char *filePath;                     // Filepath [OPT].
    unsigned long long startAddress;    // Starting address of memory map.
    unsigned long long endAddress;      // Ending address of memory map.
    unsigned long long offset;          // Offset in file [OPT].
    char *perms;                        // Memory access permissions [rwxt].
    int devMajor, devMinor;             // File major/minor device num [OPT].
    int inode;                          // File inode [OPT].                                                 
} ProcMap;

/*
 *******************************************************************************
 *                            Routine Declarations                             *
 *******************************************************************************
*/

// Parse next line of map file. Set global variables. Return zero on failure.
int parseNext (ProcMap *mp);

// Opens a process's "maps" file. Returns zero on failure.
int openProcMaps (int pid);

// Closes the current open maps file.
void closeProcMaps (void);


#endif