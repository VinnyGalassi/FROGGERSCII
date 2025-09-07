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

    // ANSI escape code to clear screen
    write(1, "\e[1;1H\e[2J", 11);
    // system("clear"); // requires stdlib

    write(1, "Game\n", 5);

    // Read the arrow keys as user input
    char buf[3];
    read(0, buf, 3);

    // Check for escape sequence for arrow keys (ESC [ A/B/C/D)
    if (buf[0] == 27 && buf[1] == 91) {
        switch (buf[2]) {
            case 'A':
                write(1, "Up arrow pressed\n", 17);
                break;
            case 'B':
                write(1, "Down arrow pressed\n", 19);
                break;
            case 'C':
                write(1, "Right arrow pressed\n", 20);
                break;
            case 'D':
                write(1, "Left arrow pressed\n", 19);
                break;
            default:
                write(1, "Other key pressed\n", 18);
                break;
        }
    } else {
        write(1, "Non-arrow key pressed\n", 22);
    }

    // Use _exit to avoid process cleanup code (exit()) that may not be available and go straight to OS's shutdown routine
    _exit(0);
}