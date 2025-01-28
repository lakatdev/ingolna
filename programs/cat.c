#include <stdio.h>

int main(int argc, char** argv) {
    if (argv[1] != NULL) {
        int c;
        FILE* fptr;
        fptr = fopen(argv[1], "r");
        if (fptr) {
            while ((c = getc(fptr)) != EOF) {
                putchar(c);
            }
            fclose(fptr);
        }
    }
    return 0;
}
