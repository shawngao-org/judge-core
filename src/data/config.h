//
// Created by ShawnGao on 2022/7/30.
//

#ifndef JUDGE_CORE_CONFIG_H
#define JUDGE_CORE_CONFIG_H

struct config {
    int max_cpu_time;
    int max_real_time;
    long max_memory_size;
    long max_stack_size;
    int max_process_amount;
    long max_output_size;
    const char *execute_path;
    const char *input_path;
    const char *output_path;
    const char *error_path;
    const char *args[255];
    const char *env[255];
    const char *seccomp_rule_name;
};

void init_config(struct config *_config);

#endif //JUDGE_CORE_CONFIG_H
