#ifndef INPUT_H
#define INPUT_H

typedef enum {
    KEY_NONE = 0,
    KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT,
    KEY_QUIT
} Key;

int  input_init_raw(void);
void input_restore(void);
Key  input_read_key(void);

#endif
