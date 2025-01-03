#include "timeout.h"
#include <signal.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

void set_timeout(int seconds) {
    struct itimerval timer;
    timer.it_value.tv_sec = seconds;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;

    if (setitimer(ITIMER_REAL, &timer, NULL) < 0) {
        perror("setitimer");
        exit(1);
    }
}

void init_timeout_handler(void (*handler)(int)) {
    struct sigaction sa;
    sa.sa_handler = handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGALRM, &sa, NULL) < 0) {
        perror("sigaction");
        exit(1);
    }
}
