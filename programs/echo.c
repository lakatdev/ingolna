#include <stdio.h>

int main(int argv, char** args) {
    if (args[1] != NULL) {
        for (int i = 1; i < argv; i++) {
            printf("%s ", args[i]);
        }
        printf("\n");
    }
    return 0;
}
