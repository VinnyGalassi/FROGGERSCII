/**
 * Custom unistd implementation using inline assembly for syscalls.
 * Only for x86_64 Linux
 */

// #include <stddef.h>
#include "custom_unistd.h"

/**
 * perform read syscall
 */
ssize_t my_read(int fd, void *buf, ssize_t count) {
    long ret;
    asm volatile (
        "syscall"
        : "=a"(ret)
        : "a"((long)SYS_READ),   // rax = syscall number
          "D"((long)fd),         // rdi = fd
          "S"(buf),              // rsi = buf
          "d"((long)count)       // rdx = count
        : "rcx", "r11", "memory"
    );
    // Negative return = error
    return ret;
}

ssize_t my_write(int fd, const void *buf, ssize_t count) {
    long ret;
    asm volatile (
        "syscall"
        : "=a"(ret)
        : "a"((long)SYS_WRITE),
          "D"((long)fd),
          "S"(buf),
          "d"((long)count)
        : "rcx", "r11", "memory"
    );
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
