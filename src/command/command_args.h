//
// Created by ShawnGao on 2022/7/30.
//

#ifndef JUDGE_CORE_COMMAND_ARGS_H
#define JUDGE_CORE_COMMAND_ARGS_H

struct command_args {
    struct arg_lit *version, *help, *unlimited_memory;
    struct arg_int *max_cpu_time, *max_real_time, *max_memory, *max_stack, *max_process, *max_output_size;
    struct arg_str *exec_path, *input_path, *output_path, *err_path, *args, *env, *seccomp_rule;
    struct arg_end *end;
};

#endif //JUDGE_CORE_COMMAND_ARGS_H
