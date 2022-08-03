//
// Created by ShawnGao on 2022/7/30.
//

#ifndef JUDGE_CORE_EXIT_HANDLER_H
#define JUDGE_CORE_EXIT_HANDLER_H

enum {
    WRONG_CONFIG_VALUE = -1,
    INVALID_OPTION = -2,
    FORK_FAILED = -3,
    SET_RLIMIT_FAILED = -4,
    IO_EXCEPTION = -5,
    HANDLE_COPY_FAILED = -6,
    EXECVE_FAILED = -7,
    CAN_NOT_CREATE_THREAD = -8,
    WAIT_FAILED = -9
};

void normal_exit();
void exception_exit(int code);

#endif //JUDGE_CORE_EXIT_HANDLER_H
