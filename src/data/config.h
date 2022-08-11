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
    char *execute_path;
    char *input_path;
    char *output_path;
    char *error_path;
    char *args[255];
    char *env[255];
    char *seccomp_rule_name;
    char *seccomp_rule_mode;
};

void init_config(struct config *_config);

#endif //JUDGE_CORE_CONFIG_H
