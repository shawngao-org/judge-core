//
// Created by ShawnGao on 2022/7/31.
//
#include "stdio.h"
#include "unistd.h"
#include "sys/time.h"
#include "sys/wait.h"
#include "pthread.h"
#include "signal.h"
#include "sys/resource.h"

#include "../data/config.h"
#include "../data/result.h"
#include "../exit/exit_handler.h"
#include "../thread/thread_handler.h"
#include "../kill/kill.h"
#include "answer_code.h"
#include "execute_handler.h"

void execute_handler(struct config *_c, struct result *_r) {
    struct timeval start;
    struct timeval end;
    gettimeofday(&start, NULL);
    pid_t pid = fork();
    if (pid < 0) {
        exception_exit(FORK_FAILED);
    }
    if (pid == 0) {
        thread_handler(_c);
        return;
    }
    pthread_t tid = 0;
    struct timeout _timeout;
    _timeout.pid = pid;
    _timeout.time_limit = _c->max_real_time;
    if (pthread_create(&tid, NULL, (void *(*)(void *)) timeout_kill, (void *) (&_timeout))) {
        kill(pid, SIGKILL);
        exception_exit(CAN_NOT_CREATE_THREAD);
    }
    int status;
    struct rusage _rusage;
    if (wait4(pid, &status, WSTOPPED, &_rusage) == -1) {
        kill(pid, SIGKILL);
        exception_exit(WAIT_FAILED);
    }
    gettimeofday(&end, NULL);
    long e = end.tv_sec * 1000 + end.tv_usec / 1000;
    long s = start.tv_sec * 1000 + start.tv_usec / 1000;
    _r->real_time = (int) (e - s);
    if (pthread_cancel(tid) != 0) {
        printf("\033[33m[Warning] Thread ID %ld return failure.\033[0m\n", tid);
    }
    if (WIFSIGNALED(status)) {
        _r->signal_code = WTERMSIG(status);
    }
    if (_r->signal_code == SIGUSR1) {
        _r->result = SYS_ERROR;
        return;
    }
    _r->exit_code = WEXITSTATUS(status);
    _r->cpu_time = (int) (_rusage.ru_utime.tv_sec * 1000 + _rusage.ru_utime.tv_usec / 1000);
    _r->memory = _rusage.ru_maxrss * 1024;
    if (_r->signal_code == SIGSEGV) {
        if (_r->memory > _c->max_memory_size) {
            _r->result = OOM;
        } else {
            _r->result = RTE;
        }
    } else {
        if (_r->signal_code) {
            _r->result = RTE;
        }
        if (_r->memory > _c->max_memory_size) {
            _r->result = OOM;
        }
        if (_r->real_time > _c->max_real_time) {
            _r->result = RTO;
        }
        if (_r->cpu_time > _c->max_cpu_time) {
            _r->result = CTO;
        }
    }
}
