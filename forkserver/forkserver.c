#include "forkserver.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

static int pipes[2], status_pipes[2];
static pid_t child_pid = -1;

void init_forkserver() {
    if (pipe(pipes) < 0 || pipe(status_pipes) < 0) {
        perror("pipe");
        exit(1);
    }

    printf("Forkserver initialized.\n");

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        // 자식 프로세스
        close(pipes[1]);
        close(status_pipes[0]);
        return;
    }

    // 부모 프로세스
    close(pipes[0]);
    close(status_pipes[1]);

    unsigned char buf[4] = {0};
    if (write(pipes[1], buf, 4) != 4) {
        perror("write to forkserver");
        exit(1);
    }
}

void run_target(char **argv) {
    child_pid = fork();
    if (child_pid < 0) {
        perror("fork");
        exit(1);
    }

    if (child_pid == 0) {
        // 자식 프로세스
        execv(argv[0], argv);
        perror("execv");
        exit(1);
    }

    wait(NULL); // 자식 프로세스 대기
}
