//
// Created by ShawnGao on 2022/7/30.
//
#include "stdio.h"
#include "stdlib.h"
#include "exit_handler.h"

void normal_exit() {
    exit(0);
}

void exception_exit(int code) {
    printf("Process finished with exit code %d\n", code);
    exit(code);
}