// target.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    char buffer[100];
    
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input>\n", argv[0]);
        return 1;
    }

    strncpy(buffer, argv[1], 100);
    buffer[99] = '\0'; // 안전하게 null-terminate

    // 간단한 조건: 입력이 "crash"이면 강제 크래시
    if (strcmp(buffer, "crash") == 0) {
        printf("Crashing as per input...\n");
        int *ptr = NULL;
        *ptr = 42; // Segmentation Fault 유발
    }

    printf("Received input: %s\n", buffer);
    return 0;
}

