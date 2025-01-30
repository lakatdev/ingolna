#include <stdio.h>
#include <stdlib.h>
#include <sys/mount.h>
#include <sys/stat.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void exec();
void mount_core_fss();
void setup_jvm();

void main() {
    mount_core_fss();
    setup_jvm();

    static char* args_welcome[] = {"welcome", NULL};
    exec("/bin/welcome", args_welcome);

    static char* args_shell[] = {"shell", NULL};
    exec("/bin/shell", args_shell);

    while(1);
}

void exec(char* cmd, char* args[]) {
    pid_t pid = fork();
    if (pid == 0) {
        execv(cmd, args);
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

void setup_jvm() {
    printf("Setting up JVM...\n");
    char *java_home = "/";
    setenv("JAVA_HOME", java_home, 1);

    char *current_path = getenv("PATH");
    if (current_path == NULL) {
        current_path = "";
    }
    char new_path[1024];
    snprintf(new_path, sizeof(new_path), "%s:%s/bin:/usr/bin", current_path, java_home);
    setenv("PATH", new_path, 1);

}
