// forkserver.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

// 크래시 상태 정의
#define FAULT_NONE  0
#define FAULT_CRASH 2

// 타겟 프로그램 경로
#define TARGET_PATH "./target"

// 최대 테스트케이스 크기
#define MAX_TESTCASE_SIZE 1024

// 함수 선언
void run_target(char *input, int *status);

// 메인 함수
int main() {
    char testcase[MAX_TESTCASE_SIZE];
    int status;

    printf("Forkserver started. Waiting for testcases...\n");

    while (1) {
        // 테스트케이스 입력 대기 (표준 입력으로부터)
        printf("Enter testcase (or 'exit' to quit): ");
        fflush(stdout);

        if (fgets(testcase, sizeof(testcase), stdin) == NULL) {
            printf("No more input. Exiting.\n");
            break;
        }

        // 개행 문자 제거
        testcase[strcspn(testcase, "\n")] = 0;

        // 종료 조건
        if (strcmp(testcase, "exit") == 0) {
            printf("Exiting forkserver.\n");
            break;
        }

        // 타겟 프로그램 실행
        run_target(testcase, &status);

        // 결과 보고
        if (status == FAULT_NONE) {
            printf("Testcase executed successfully.\n");
        } else if (status == FAULT_CRASH) {
            printf("Testcase caused a crash!\n");
        } else {
            printf("Testcase execution resulted in unknown status: %d\n", status);
        }
    }

    return 0;
}

// 타겟 프로그램 실행 함수
void run_target(char *input, int *status) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        *status = FAULT_NONE;
        return;
    }

    if (pid == 0) {
        // 자식 프로세스: 타겟 프로그램 실행
        execl(TARGET_PATH, TARGET_PATH, input, (char *)NULL);
        // execl이 실패하면 종료
        perror("execl failed");
        exit(1);
    } else {
        // 부모 프로세스: 자식 프로세스의 상태 대기
        int wstatus;
        waitpid(pid, &wstatus, 0);

        if (WIFSIGNALED(wstatus)) {
            int sig = WTERMSIG(wstatus);
            if (sig == SIGSEGV || sig == SIGABRT) {
                *status = FAULT_CRASH;
            } else {
                *status = FAULT_CRASH; // 다른 신호도 크래시로 간주
            }
        } else {
            *status = FAULT_NONE;
        }
    }
}

