/**
 * frogger.c
 *
 *
 */

#include <unistd.h>

// _start denotes program entry point when using -nostartfiles
// avoids overhead of normal OS main() setup
int _start(void) {
    // TODO: replace read/write with assembly syscalls?
    write(1, "Hello, World!\n", 14);

    // Use _exit to avoid process cleanup code (exit()) that may not be available and go straight to OS's shutdown routine
    _exit(0);
}