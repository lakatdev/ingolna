#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

int main(int  argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Missing single operand: path\n");
        return -1;
    }

    struct stat st = {0};

    if (stat(argv[1], &st) == -1) {
        mkdir(argv[1], 0700);
    }
    else {
        fprintf(stderr, "Directory already exists\n");
        return -1;
    }

    return 0;
}