//
// Created by ShawnGao on 2022/7/31.
//
#include "result.h"

void init_result(struct result *_result) {
    _result->cpu_time = 0;
    _result->real_time = 0;
    _result->memory = 0;
    _result->signal_code = 0;
    _result->exit_code = 0;
    _result->result = SUCCESS;
}