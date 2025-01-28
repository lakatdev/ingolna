#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <dirent.h>
#include <linux/limits.h>

#define BUFFSIZE 1024

#define TOKENSIZE 64
#define TOKENDELIM " \t\r\n\a"

void main_loop();
char* read_line();
char** split_line(char* line);
int execute(char** args);

int main(int argc, char** argv) {
    main_loop();
    return 0;
}

void main_loop() {
    char* line;
    char** args;
    int status;

    do {
        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf(cwd);
        }

        printf("> ");
        line = read_line();
        args = split_line(line);
        status = execute(args);

        free(line);
        free(args);
    } while (status);
}

char* read_line() {
    int buffsize = BUFFSIZE;
    int position = 0;
    char* buffer = malloc(sizeof(char) * buffsize);
    int c;

    if (!buffer) {
        fprintf(stderr, "shell: Allocation error\n");
        exit(EXIT_FAILURE);
    }

    while (1) {
        
        c = getchar();

        if (c == EOF || c == '\n') {
            buffer[position] = 0;
            return buffer;
        }
        else {
            buffer[position] = c;
        }
        position++;

        if (position >= buffsize) {
            buffsize += BUFFSIZE;
            buffer = realloc(buffer, buffsize);

            if (!buffer) {
                fprintf(stderr, "shell: Allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

char** split_line(char* line) {
    int buffsize = TOKENSIZE, position = 0;
    char** tokens = malloc(buffsize * sizeof(char*));
    char* token;

    if (!tokens) {
        fprintf(stderr, "shell: Allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, TOKENDELIM);
    while (token != NULL) {
        tokens[position] = token;
        position++;

        if (position >= buffsize) {
            buffsize += TOKENSIZE;
            tokens = realloc(tokens, buffsize * sizeof(char*));

            if (!tokens) {
                fprintf(stderr, "shell: Allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL, TOKENDELIM);
    }
    tokens[position] = NULL;
    return tokens;
}

int launch(char** args) {
    pid_t pid, wpid;
    int status;

    pid = fork();

    if (pid == 0) {
        if (execvp(args[0], args) == -1) {
            perror("shell");
        }
        exit(EXIT_FAILURE);
    }
    else if (pid < 0) {
        perror("shell");
    }
    else {
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

//BUILTIN PROGRAMS START

/*
    Sort builtin programs alphabetically
*/
int shell_cd(char** args);
int shell_clear(char** args);
int shell_exit(char** args);
int shell_help(char** args);
int shell_ls(char** args);

char* builtin_str[] = {
    "cd",
    "clear",
    "exit",
    "help",
    "ls"
};

int (*builtin_func[]) (char**) = {
    &shell_cd,
    &shell_clear,
    &shell_exit,
    &shell_help,
    &shell_ls
};

int num_builtins() {
    return sizeof(builtin_str) / sizeof(char*);
}

int shell_cd(char** args) {
    if (args[1] == NULL) {
        if (chdir("/") != 0) {
            perror("shell");
        }
    }
    else if (chdir(args[1]) != 0) {
        perror("shell");
    }
    return 1;
}

int shell_clear(char** args) {
    write(1, "\33[H\33[2J", 7);
    return 1;
}

int shell_exit(char** args) {
    return 0;
}

int shell_help(char** args) {
    printf("Ingolna Shell\n");
    printf("Shell programs:\n");

    for (int i = 0; i < num_builtins(); i++) {
        printf("%s  ", builtin_str[i]);
    }

    printf("\nInstalled programs:\n");

    struct dirent* de;
    DIR* dr = opendir("/bin");

    if (dr == NULL) {
        fprintf(stderr, "Could not open /bin directory\n");
        return 1;
    }

    while ((de = readdir(dr)) != NULL) {
        if (strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0) {
            printf("%s  ", de->d_name);
        }
    }

    printf("\n");

    return 1;
}

int shell_ls(char** args) {
    struct dirent* de;
    DIR* dr = args[1] == NULL ? opendir(".") : opendir(args[1]);

    if (dr == NULL) {
        fprintf(stderr, "Could not open directory\n");
        return 1;
    }

    while ((de = readdir(dr)) != NULL) {
        printf("%s  ", de->d_name);
    }

    printf("\n");
    closedir(dr);

    return 1;
}

//BUILTIN PROGRAMS END

int execute(char** args) {
    if (args[0] == NULL) {
        return 1;
    }

    for (int i = 0; i < num_builtins(); i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            return (*builtin_func[i])(args);
        }
    }

    return launch(args);
}
