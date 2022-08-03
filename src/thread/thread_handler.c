//
// Created by ShawnGao on 2022/7/31.
//
#include "stdio.h"
#include "sys/resource.h"
#include "string.h"
#include "unistd.h"

#include "../data/config.h"
#include "../exit/exit_handler.h"

void check_rlimit(int code) {
    if (code != 0) {
        exception_exit(SET_RLIMIT_FAILED);
    }
}

void set_rlimit(struct config *_c) {
    struct rlimit cpu_time, memory, stack, process, output;
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
    FILE *in = fopen(_c->input_path, "r");
    FILE *out = fopen(_c->output_path, "w");
    int err_flag = strcmp(_c->error_path, _c->output_path);
    FILE *err = err_flag ? fopen(_c->error_path, "w") : out;
    if (!(in != NULL && out != NULL && err != NULL)) {
        exception_exit(IO_EXCEPTION);
    }
    int dup2_flag = dup2(fileno(in), fileno(stdin)) != -1
            && dup2(fileno(out), fileno(stdout)) != -1
            && dup2(fileno(err), fileno(stderr));
    if (!dup2_flag) {
        exception_exit(HANDLE_COPY_FAILED);
    }
    execve(_c->execute_path, _c->args, _c->env);
    exception_exit(EXECVE_FAILED);
}
