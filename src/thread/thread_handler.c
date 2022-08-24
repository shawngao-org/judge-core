//
// Created by ShawnGao on 2022/7/31.
//
#include "stdio.h"
#include "sys/resource.h"
#include "string.h"
#include "unistd.h"
#include "fcntl.h"

#include "../data/config.h"
#include "../exit/exit_handler.h"
#include "../seccomp/seccomp_handler.h"

void check_rlimit(int code) {
    if (code != 0) {
        exception_exit(SET_RLIMIT_FAILED);
    }
}

void set_rlimit(const struct config *_c) {
    struct rlimit cpu_time;
    struct rlimit memory;
    struct rlimit stack;
    struct rlimit process;
    struct rlimit output;
    cpu_time.rlim_cur = cpu_time.rlim_max = _c->max_cpu_time;
    memory.rlim_cur = memory.rlim_max = _c->max_memory_size;
    stack.rlim_cur = stack.rlim_max = _c->max_stack_size;
    process.rlim_cur = process.rlim_max = _c->max_process_amount;
    output.rlim_cur = output.rlim_max = _c->max_output_size;
    check_rlimit(setrlimit(RLIMIT_CPU, &cpu_time));
    check_rlimit(setrlimit(RLIMIT_AS, &memory));
    check_rlimit(setrlimit(RLIMIT_STACK, &stack));
    check_rlimit(setrlimit(RLIMIT_NPROC, &process));
    check_rlimit(setrlimit(RLIMIT_FSIZE, &output));
}

void thread_handler(struct config *_c) {
    set_rlimit(_c);
    int fno_in = open(_c->input_path, O_RDONLY);
    if (fno_in == -1) {
        exception_exit(IO_EXCEPTION);
    }
    int d_in = dup2(fno_in, fileno(stdin)) != -1;

    int fno_out = open(_c->output_path, O_WRONLY);
    if (fno_out == -1) {
        exception_exit(IO_EXCEPTION);
    }
    int d_out = dup2(fno_out, fileno(stdout)) != -1;

    int err_flag = strcmp(_c->error_path, _c->output_path);
    int fno_err;
    if (err_flag != 0) {
        fno_err = open(_c->error_path, O_WRONLY);
    } else {
        fno_err = fno_out;
    }
    if (fno_err == -1) {
        exception_exit(IO_EXCEPTION);
    }
    int d_err = dup2(fno_err, fileno(stderr)) != -1;
    int dup2_flag = d_in && d_out && d_err;
    if (!dup2_flag) {
        exception_exit(HANDLE_COPY_FAILED);
    }
    if (seccomp_handler(_c)) {
        exception_exit(SET_SECCOMP_FAILED);
    }
    execve(_c->execute_path, _c->args, _c->env);
    exception_exit(EXECVE_FAILED);
}
