//
// Created by ShawnGao on 2022/7/31.
//

#ifndef JUDGE_CORE_KILL_H
#define JUDGE_CORE_KILL_H

#include "unistd.h"

struct timeout {
    int pid;
    int time_limit;
};

void *timeout_kill(struct timeout *_t);

#endif //JUDGE_CORE_KILL_H
