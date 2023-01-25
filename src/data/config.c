//
// Created by ShawnGao on 2022/7/31.
//
#include "stdio.h"
#include "config.h"

void init_config(struct config *_config) {
    _config->max_cpu_time = 1000;
    _config->max_real_time = 1000;
    _config->unlimited_memory = 0;
    _config->max_memory_size = 16777216;
    _config->max_stack_size = 16777216;
    _config->max_process_amount = 1;
    _config->max_output_size = 16777216;
    _config->input_path = "/dev/stdin";
    _config->output_path = "/dev/stdout";
    _config->error_path = "/dev/stderr";
    _config->seccomp_rule_name = NULL;
}