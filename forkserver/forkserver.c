#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int pipes[2];  // 파이프 배열

void init_forkserver() {
    if (pipe(pipes) < 0) {
        perror("pipe");
        exit(1);
    }
    printf("Forkserver initialized.\n");
}

void run_forkserver() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        // 자식 프로세스
        close(pipes[0]); // 자식은 읽기 파이프 닫기
        printf("Child process ready.\n");
        // 자식 프로세스의 논리 수행
        while (1) {
            // 부모로부터 메시지 대기
            char buffer[1];
            if (read(pipes[1], buffer, 1) <= 0) {
                break; // 부모가 종료되면 루프 종료
            }
            printf("Child received message: %c\n", buffer[0]);
            // 작업 수행 후 결과 전송 (여기서는 예제 메시지 전송)
            write(pipes[1], "R", 1);
        }
        exit(0);
    } else {
        // 부모 프로세스
        close(pipes[1]); // 부모는 쓰기 파이프 닫기
        printf("Parent process running.\n");

        // 자식 프로세스와 통신
        char message = 'F';
        write(pipes[0], &message, 1); // 메시지 전송
        char response;
        read(pipes[0], &response, 1); // 응답 대기
        printf("Parent received response: %c\n", response);

        wait(NULL); // 자식 프로세스 종료 대기
    }
}

int main() {
    init_forkserver();
    run_forkserver();
    return 0;
}
