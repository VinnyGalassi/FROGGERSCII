#ifndef CUSTOM_UNISTD_H
#define CUSTOM_UNISTD_H
/**
 * Custom unistd implementation using inline assembly for syscalls.
 * Only for x86_64 Linux
 */
// #include <stddef.h>


typedef long ssize_t;

#define SYS_READ   0
#define SYS_WRITE  1
#define SYS_EXIT   60

#define	STDIN_FILENO	0	/* Standard input.  */
#define	STDOUT_FILENO	1	/* Standard output.  */
#define	STDERR_FILENO	2	/* Standard error output.  */

ssize_t my_read(int fd, void *buf, ssize_t count);
ssize_t my_write(int fd, const void *buf, ssize_t count);
void my_exit(int status);

#endif // CUSTOM_UNISTD_H