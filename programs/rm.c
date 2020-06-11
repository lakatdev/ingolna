#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

void rmtree(const char* path);

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Missing single operand: path\n");
        return -1;
    }

    if (unlink(argv[1]) == 0) {
        printf("Removed file: %s\n", argv[1]);
    }
    else {
        rmtree(argv[1]);   
    }

    return 0;
}

void rmtree(const char* path) {
    size_t path_len;
    char* full_path;
    DIR* dir;
    struct stat stat_path, stat_entry;
    struct dirent* entry;

    stat(path, &stat_path);

    if (S_ISDIR(stat_path.st_mode) == 0) {
        fprintf(stderr, "Path is not a directory\n");
        exit(-1);
    }

    if ((dir = opendir(path)) == NULL) {
        fprintf(stderr, "Cannot open directory\n");
        exit(-1);
    }

    path_len = strlen(path);

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        full_path = calloc(path_len + strlen(entry->d_name) + 1, sizeof(char));
        strcpy(full_path, path);
        strcat(full_path, "/");
        strcat(full_path, entry->d_name);

        stat(full_path, &stat_entry);

        if (S_ISDIR(stat_entry.st_mode) != 0) {
            rmtree(full_path);
            continue;
        }

        if (unlink(full_path) == 0) {
            printf("Removed file: %s\n", full_path);
        }
        else {
            printf("Failed to remove file: %s\n", full_path);
        }

        free(full_path);
    }

    if (rmdir(path) == 0) {
        printf("Removed directory: %s\n", path);
    }
    else {
        printf("Failed to remove directory: %s\n", path);
    }

    closedir(dir);
}