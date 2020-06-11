#ifndef BINARY_H
#define BINARY_H

#include <stdlib.h>

char* instructions;
unsigned int instructions_length = 0;

char* arguments;
unsigned int arguments_length = 0;

/*
    Appends instruction to instructions array
*/
void add_instruction(unsigned short type, unsigned short argsize, unsigned int args) {
    instructions = realloc(instructions, instructions_length + sizeof(short) * 2 + sizeof(int));
    memcpy(&instructions[instructions_length], &type, sizeof(short));
    memcpy(&instructions[instructions_length + sizeof(short)], &argsize, sizeof(short));
    memcpy(&instructions[instructions_length + sizeof(short) * 2], &args, sizeof(int));

    instructions_length += sizeof(short) * 2 + sizeof(int);
}

/*
    Adds list of ints as arguments to arguments array returns location in said array
*/
unsigned int add_int_arguments(int* args, int length) {
    unsigned int start_location = arguments_length;

    arguments = realloc(arguments, arguments_length + sizeof(int) * length);

    memcpy(&arguments[arguments_length], args, length * sizeof(int));
    arguments_length += length * sizeof(int);

    return start_location;
}

/*
    Adds list of bytes as arguments to arguments array returns location in said array
*/
unsigned int add_bytes_argument(char* args, int length) {
    unsigned int start_location = arguments_length;

    arguments = realloc(arguments, arguments_length + length);

    memcpy(&arguments[arguments_length], args, length);
    arguments_length += length;

    return start_location;
}

/*
    Merges instructions and arguments into one byte array
*/
void merge() {
    instructions = realloc(instructions, instructions_length + arguments_length);
    memcpy(&instructions[instructions_length], arguments, arguments_length);
    free(arguments);
    instructions_length += arguments_length;
    arguments_length = 0;
}

#endif