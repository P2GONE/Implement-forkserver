#ifndef TIMEOUT_H
#define TIMEOUT_H

void set_timeout(int seconds);
void init_timeout_handler(void (*handler)(int));

#endif // TIMEOUT_H
