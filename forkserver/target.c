#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 128

int main() {
    char buffer[BUFFER_SIZE];

    printf("Enter input: ");
    if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
        fprintf(stderr, "Error reading input.\n");
        return 1;
    }

    printf("You entered: %s", buffer);

    return 0;
}
