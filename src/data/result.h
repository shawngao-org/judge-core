//
// Created by ShawnGao on 2022/7/31.
//

#ifndef JUDGE_CORE_RESULT_H
#define JUDGE_CORE_RESULT_H

struct result {
    int cpu_time;
    int real_time;
    long memory;
    int signal_code;
    int exit_code;
    int result;
};

enum {
    SUCCESS = 0
};

void init_result(struct result *_result);

#endif //JUDGE_CORE_RESULT_H
