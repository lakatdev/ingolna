#include <stdio.h>

int main(int argv, char** args) {
    if (args[1] != NULL) {
        printf(args[1]);
        printf("\n");
    }
    return 0;
}
