// target.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _CRT_SECURE_NO_WARNINGS

int main(int argc, char *argv[]) {
    char buffer[20];
    
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input>\n", argv[0]);
        return 1;
    }

    strcpy(buffer, argv[1]);

    printf("Received input: %s\n", buffer);
    return 0;
}

