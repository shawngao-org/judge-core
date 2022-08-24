//
// Created by ShawnGao on 2022/7/31.
//
#include "unistd.h"
#include "signal.h"
#include "pthread.h"
#include "kill.h"

void *timeout_kill(struct timeout *_t) {
    pid_t pid = _t->pid;
    int timeout = _t->time_limit;
    unsigned int t = (timeout + 1000) / 1000;
    _t->pid = pid;
    _t->time_limit = timeout;
    if (pthread_detach(pthread_self()) != 0 || sleep(t) != 0 || kill(pid, SIGKILL) != 0) {
        kill(pid, SIGKILL);
    }
    return NULL;
}
