#include <stdio.h>

int main(int argc, char** argv) {
    if (argv[1] != NULL) {
        for (int i = 1; i < argc; i++) {
            printf("%s ", argv[i]);
        }
        printf("\n");
    }
    return 0;
}
