#ifndef FLAGS_H
#define FLAGS_H

#include <stdlib.h>

typedef struct {
    char* name;
    int name_length;
    int position;
} flag;

flag* flags;
int flags_size = 0;

/*
    Add new flag to the global array of flags
*/
void add_flag(char* name, int name_length, int position) {
    flags = realloc(flags, sizeof(flag) * (flags_size + 1));
    flags[flags_size].name_length = name_length;
    flags[flags_size].name = malloc(name_length);
    memcpy(flags[flags_size].name, name, name_length);
    flags[flags_size].position = position;
    flags_size++;
}

/*
    Change value of given flag
*/
void set_flag(char* name, int name_length, int position) {
    for (int i = 0; i < flags_size; i++) {
        if (memcmp(name, flags[i].name, name_length) == 0) {
            flags[i].position = position;
            break;
        }
    }
}

/*
    Returns position of flag
*/
int get_flag_position(char* name, int name_length) {
    for (int i = 0; i < flags_size; i++) {
        if (memcmp(name, flags[i].name, name_length) == 0) {
            return flags[i].position;
        }
    }
    return 0;
}

#endif