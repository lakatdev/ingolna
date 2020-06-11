#ifndef UTILS_H
#define UTILS_H

#include <string.h>
#include <stdlib.h>

#define bool _Bool
#define true 1
#define false 0

typedef struct {
    char* data;
    unsigned int length;
} token_t;

typedef struct {
    token_t* data;
    unsigned int length;
} line_t;

typedef  struct {
    line_t* data;
    unsigned int length;
} lines_list_t;

/*
    Replaces og character in string with to character
*/
void replace_char(char* str, unsigned int length, char og, char to) {
    for (int i = 0; i < length; i++) {
        if (str[i] == og) {
            str[i] == to;
        }
    }
}

/*
    Checks for occurances in string when del0 and del1 are next to each other and removes del1
    Returns length of modified string
*/
int remove_double_chars(char* str, unsigned int length, char del0, char del1, char rep) {
    for (int i = 0; i < length - 1; i++) {
        if (str[i] == del0 && str[i + 1] == del1) {
            memmove(&str[i + 1], &str[i + 2], length - i + 1);
            str[i] = rep;
            length = remove_double_chars(str, length - 1, del0, del1, rep);
        }
    }
    return length;
}

/*
    Counts how many times a character is present in a string
*/
int count_chars(char* str, unsigned int length, char del) {
    int count = 0;
    for (int i = 0; i < length; i++) {
        if (str[i] == del) {
            count++;
        }
    }
    return count;
}

/*
    Returns position of requested character in string
*/
int get_char_pos(char* str, unsigned int length, char del, int which) {
    int pos = 0;
    while (which + 1 > 0) {
        if (pos >= length) {
            return length;
        }
        if (str[pos] == del) {
            which--;
        }
        pos++;
    }
    return pos - 1;
}

/*
    Generates random string of given length
*/
void random_string(char* str, unsigned int size) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    if (size) {
        --size;
        for (int i = 0; i < size; i++) {
            int key = rand() % (sizeof(charset) - 1);
            str[i] = charset[key];
        }
    }
}

/*
    Inserts new line to lines_list_t
*/
void insert_line(lines_list_t* lines, int index, line_t line) {
    lines->data = realloc(lines->data, sizeof(line_t) * (lines->length + 1));
    memmove(&lines->data[index + 1], &lines->data[index], (lines->length - index) * sizeof(line_t));
    lines->length++;
    lines->data[index] = line;
}

/*
    Removes line from lines_list_t at index
*/
void remove_line(lines_list_t* lines, int index) {
    memmove(&lines->data[index], &lines->data[index + 1], (lines->length - index - 1) * sizeof(line_t));
    lines->data = realloc(lines->data, sizeof(line_t) * (lines->length - 1));
    lines->length--;
}

/*
    Checks if given string is numerical
*/
bool is_number(char* str, int length) {
    for (int i = 0; i < length; i++) {
        if (str[i] < 48 || str[i] > 57) {
            return false;
        }
    }
    return true;
}

/*
    Inserts null terminator at the end of the string
*/
char* insert_null(char* str, int length) {
    str = realloc(str, length + 1);
    str[length] = 0;

    return str;
}

/*
    Converts C string to int
*/
int to_int(char* str) {
    int result;
    int puiss;

    result = 0;
    puiss = 1;
    while (('-' == (*str)) || ((*str) == '+')){
        if (*str == '-')
            puiss = puiss * -1;
        str++;
    }
    while ((*str >= '0') && (*str <= '9')){
        result = (result * 10) + ((*str) - '0');
        str++;
    }
    return (result * puiss);
}

/*
    Converts escaped characters to their real counterpart
*/
int convert_chars(char* str, int length) {
    int new_length = remove_double_chars(str, length, '\\', 'n', '\n');
    new_length = remove_double_chars(str, new_length, '\\', 'h', '#');
    return remove_double_chars(str, new_length, '\\', '\\', '\\');
}

#endif