/**
 * Custom unistd implementation using inline assembly for syscalls.
 * Only for x86_64 Linux
 */

// #include <stddef.h>
#include "custom_unistd.h"

/**
 * perform read or write syscall
 */
ssize_t my_syscall(int trap_number, int fd, const void *buf, ssize_t count) {
    long ret;
    asm volatile (
        "syscall"
        : "=a"(ret)
        : "a"((long)trap_number),// rax = syscall number
          "D"((long)fd),         // rdi = fd
          "S"(buf),              // rsi = buf
          "d"((long)count)       // rdx = count
        : "rcx", "r11", "memory"
    );
    // Negative return = error
    return ret;
}

__attribute__((noreturn))
void my_exit(int status) {
    asm volatile (
        "syscall"
        :
        : "a"((long)SYS_EXIT),   // rax = syscall number
          "D"((long)status)      // rdi = exit status
        : "rcx", "r11", "memory"
    );
    // tell compiler this never returns
    __builtin_unreachable();
}
