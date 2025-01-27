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

    printf("Build 2025.01.27\n");

    return 0;
}
