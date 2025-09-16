// #include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include "input.h"
#include "custom_unistd.h"

static struct termios orig;
static int raw_enabled = 0;

int input_init_raw(void) {
    struct termios t;
    if (tcgetattr(STDIN_FILENO, &orig) < 0) return -1;
    t = orig;
    t.c_lflag &= ~(ICANON | ECHO);
    t.c_cc[VMIN] = 0;
    t.c_cc[VTIME] = 0;
    if (tcsetattr(STDIN_FILENO, TCSANOW, &t) < 0) return -1;
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    if (flags >= 0) fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
    raw_enabled = 1;
    return 0;
}

void input_restore(void) {
    if (raw_enabled) tcsetattr(STDIN_FILENO, TCSANOW, &orig);
    raw_enabled = 0;
}

static Key map_char(unsigned char c) {
    if (c=='q' || c=='Q') return KEY_QUIT;
    if (c=='w' || c=='W') return KEY_UP;
    if (c=='s' || c=='S') return KEY_DOWN;
    if (c=='a' || c=='A') return KEY_LEFT;
    if (c=='d' || c=='D') return KEY_RIGHT;
    return KEY_NONE;
}

Key input_read_key(void) {
    unsigned char buf[8];
    int n = (int)my_syscall(SYS_READ, STDIN_FILENO, buf, sizeof(buf));
    if (n <= 0) return KEY_NONE;

    if (buf[0] == 0x1b && n >= 3 && buf[1] == '[') {
        switch (buf[2]) {
            case 'A': return KEY_UP;
            case 'B': return KEY_DOWN;
            case 'C': return KEY_RIGHT;
            case 'D': return KEY_LEFT;
        }
    }
    return map_char(buf[0]);
}
