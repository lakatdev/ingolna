#include <stdio.h>

int main(int argc, char** argv) {
    printf("Welcome to\n");

    int c;
    FILE* fptr;
    fptr = fopen("/etc/signature", "r");
    if (fptr) {
        while ((c = getc(fptr)) != EOF)
            putchar(c);
        fclose(fptr);
    }

    printf("Build 2020.06.11\n");

    return 0;
}