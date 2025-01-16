#include "forkserver.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

static int pipes[2], status_pipes[2];
static pid_t child_pid = -1;

void init_forkserver(char **argv) {
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
        // 자식 프로세스: Fork Server 역할
        close(pipes[1]);        // 부모와의 쓰기 파이프 닫기
        close(status_pipes[0]); // 부모와의 읽기 파이프 닫기

        while (1) {
            unsigned char cmd;
            ssize_t res = read(pipes[0], &cmd, 1);
            if (res < 1) {
                // 파이프가 닫히거나 에러 발생 시 종료
                exit(0);
            }

            if (cmd == 'R') {
                // 'R' 명령을 받으면 자식 프로세스 생성
                pid_t target_pid = fork();
                if (target_pid < 0) {
                    perror("fork");
                    unsigned char status = 'F'; // Fork 실패 상태
                    write(status_pipes[1], &status, 1);
                    continue;
                }

                if (target_pid == 0) {
                    // 자식 프로세스: 타겟 프로그램 실행
                    // 환경 변수에 공유 메모리 ID 설정은 이미 init_shared_memory에서 처리됨

                    // 타임아웃 설정
                    // set_timeout이 호출되기 전에 타겟 프로그램이 SIGALRM을 처리할 수 있도록 시그널 핸들러 설정
                    // 여기서는 기본 시그널 핸들러 사용 (SIG_DFL)
                    signal(SIGALRM, SIG_DFL);

                    // 타임아웃 설정 (예: 2초)
                    set_timeout(2);

                    // 타겟 프로그램 실행
                    execv(argv[0], argv);
                    perror("execv failed");
                    exit(1);
                } else {
                    // Fork Server는 타겟 프로세스의 상태를 기다림
                    int status;
                    if (waitpid(target_pid, &status, 0) < 0) {
                        perror("waitpid failed");
                        unsigned char status_byte = 'W'; // Wait 실패 상태
                        write(status_pipes[1], &status_byte, 1);
                        continue;
                    }

                    unsigned char status_byte;
                    if (WIFEXITED(status)) {
                        status_byte = WEXITSTATUS(status);
                    } else if (WIFSIGNALED(status)) {
                        status_byte = 128 + WTERMSIG(status); // AFL과 유사한 방식
                    } else {
                        status_byte = 255; // 알 수 없는 상태
                    }

                    // 상태를 퍼저에게 전송
                    write(status_pipes[1], &status_byte, 1);
                }
            }
            else if (cmd == 'Q') {
                // 'Q' 명령을 받으면 Fork Server 종료
                exit(0);
            }
            // 추가 명령어를 처리하려면 여기서 확장 가능
        }
    }
    else {
        // 부모 프로세스: Fork Server와 통신 준비
        close(pipes[0]);        // 자식과의 읽기 파이프 닫기
        close(status_pipes[1]); // 자식과의 쓰기 파이프 닫기

        // 초기화 완료 신호 전송 (AFL에서는 초기화 완료를 알리는 신호를 보냄)
        unsigned char buf[4] = {0};
        if (write(pipes[1], buf, 4) != 4) {
            perror("write to forkserver");
            exit(1);
        }

        // Fork Server가 초기화되었는지 확인 (여기서는 단순히 메시지를 보냄)
    }
}

void run_target() {
    // Fork Server에게 포크 요청 ('R' 명령)
    unsigned char cmd = 'R';
    if (write(pipes[1], &cmd, 1) != 1) {
        perror("write to forkserver failed");
        exit(1);
    }

    // Fork Server로부터 상태를 읽음
    unsigned char status;
    ssize_t res = read(status_pipes[0], &status, 1);
    if (res != 1) {
        perror("read from status_pipe failed");
        exit(1);
    }

    // 상태 처리
    if (status < 128) {
        printf("[Parent] Child exited with status: %d\n", status);
    }
    else if (status >= 128 && status < 255) {
        printf("[Parent] Child was killed by signal: %d\n", status - 128);
    }
    else {
        printf("[Parent] Unknown status: %d\n", status);
    }
}

// Fork Server 종료 함수
void shutdown_fork_server() {
    // Fork Server에게 종료 요청 ('Q' 명령)
    unsigned char cmd = 'Q';
    if (write(pipes[1], &cmd, 1) != 1) {
        perror("write to forkserver failed");
        exit(1);
    }

    // Fork Server 자식 프로세스 종료 대기
    pid_t pid = wait(NULL);
    if (pid < 0) {
        perror("wait failed");
        exit(1);
    }

    // 파이프 닫기
    close(pipes[1]);
    close(status_pipes[0]);
}