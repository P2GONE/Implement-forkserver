#include "shared_memory.h"
#include "timeout.h"
#include "forkserver.h"
#include <stdio.h>
#include <stdlib.h>

void timeout_handler(int sig) {
    printf("Timeout occurred!\n");
    exit(1);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <program_to_fuzz>\n", argv[0]);
        return 1;
    }

    init_shared_memory();
    init_timeout_handler(timeout_handler);
    init_forkserver(&argv[1]);

    while (1) {
        //set_timeout(2); // 타임아웃 설정
        run_target();
        sleep(2);
    }
    
    shutdown_fork_server();
    cleanup_shared_memory();
    return 0;
}
