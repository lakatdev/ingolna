#include <stdio.h>
#include <stdlib.h>
#include <sys/mount.h>
#include <sys/stat.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void exec();
void mount_core_fss();

void main() {
    mount_core_fss();

    static char* argv_welcome[] = {"welcome", NULL};
    exec("/bin/welcome", argv_welcome);
    
    static char* argv_shell[] = {"shell", NULL};
    exec("/bin/shell", argv_shell);

    while(1);
}

void exec(char* cmd, char* argv[]) {
    pid_t pid = fork();
    if (pid == 0) {
        execv(cmd, argv);
    }
    else {
        waitpid(pid, 0, 0);
    }
}

void mount_core_fss() {
    printf("Mounting core filesystems...\n");

    if (!mount("none", "/dev", "devtmpfs", 0, "")) {
        printf("devtmpfs mounted\n");
    }
    else {
        printf("error mounting devtmpfs\n");
    }

    if (!mount("none", "/proc", "proc", 0, "")) {
        printf("proc mounted\n");
    }
    else {
        printf("error mounting proc\n");
    }

    if (!mount("none", "/tmp", "tmpfs", 0, "mode=1777")) {
        printf("tmpfs mounted\n");
    }
    else {
        printf("error mounting tmpfs\n");
    }

    if (!mount("none", "/sys", "sysfs", 0, "")) {
        printf("sysfs mounted\n");
    }
    else {
        printf("error mounting sysfs\n");
    }

    mkdir("/dev/pts", 0700);

    if (!mount("none", "/dev/pts", "devpts", 0, "")) {
        printf("devpts mounted\n");
    }
    else {
        printf("error mounting devpts\n");
    }
}