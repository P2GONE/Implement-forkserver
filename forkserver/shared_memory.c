#include "shared_memory.h"
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <stdio.h>

static unsigned char *trace_bits = NULL;
static int shm_id = -1;

void init_shared_memory() {
    shm_id = shmget(IPC_PRIVATE, MAP_SIZE, IPC_CREAT | IPC_EXCL | 0600);
    if (shm_id < 0) {
        perror("shmget");
        exit(1);
    }

    trace_bits = (unsigned char *)shmat(shm_id, NULL, 0);
    if (trace_bits == (void *)-1) {
        perror("shmat");
        exit(1);
    }

    memset(trace_bits, 0, MAP_SIZE);

    char shm_str[32];
    snprintf(shm_str, sizeof(shm_str), "%d", shm_id);
    setenv(SHM_ENV_VAR, shm_str, 1);
}

unsigned char *get_trace_bits() {
    return trace_bits;
}

void cleanup_shared_memory() {
    if (trace_bits) {
        shmdt(trace_bits);
        unsetenv(SHM_ENV_VAR);
        trace_bits = NULL;
    }

    if (shm_id >= 0) {
        shmctl(shm_id, IPC_RMID, NULL);
        shm_id = -1;
    }
}
