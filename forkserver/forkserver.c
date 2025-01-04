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
    pid_t child_pid = fork();
    if (child_pid < 0) {
        perror("fork failed");
        exit(1);
    } 
    else if (child_pid == 0){
        printf("[Child] Process created successfully! PID: %d\n", getpid());
        if (execv(argv[0], argv) < 0) {
            perror("execv failed");
            exit(1); 
        }
    }
    else {
        printf("[Parent] Created child process. Parent PID: %d, Child PID: %d\n",
                    getpid(), child_pid);

        int status;
        if (waitpid(child_pid, &status, 0) < 0) {
            perror("waitpid failed");
            exit(1);
        }
        
        // 자식 종료 상태 확인
        if (WIFEXITED(status)) {
            printf("[Parent] Child exited with status: %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("[Parent] Child was killed by signal: %d\n", WTERMSIG(status));
        }
    }

}
