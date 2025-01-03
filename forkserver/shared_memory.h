#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <stddef.h>

#define SHM_ENV_VAR "__AFL_SHM_ID"
#define MAP_SIZE 65536

void init_shared_memory(void);
unsigned char *get_trace_bits(void);
void cleanup_shared_memory(void);

#endif // SHARED_MEMORY_H
