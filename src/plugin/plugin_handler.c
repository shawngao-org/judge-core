//
// Created by ShawnGao on 2022/8/3.
//
#include "stdio.h"
#include "seccomp.h"
#include "fcntl.h"
#include "errno.h"
#include "syscall.h"
#include "../data/config.h"
#include "plugin_handler.h"

scmp_filter_ctx on_enable(struct config *_c) {
    scmp_filter_ctx ctx = NULL;
    ctx = seccomp_init(SCMP_ACT_ALLOW);
    if (!ctx) {
        return NULL;
    }
    // 黑名单 (本插件例子使用allow模式, deny模式请自行更改)
    int _blacklist[] = {SYS_clone, SYS_execveat, SYS_fork, SYS_kill, SYS_vfork};
    // 计算黑名单元素的个数
    int _blacklist_length = sizeof(_blacklist) / sizeof(int);
    // 循环添加规则
    for (int i = 0; i < _blacklist_length; i++) {
        if (seccomp_rule_add(ctx, SCMP_ACT_KILL, _blacklist[i], 0)) {
            return NULL;
        }
    }
    // 阻止操作socket
    if (seccomp_rule_add(ctx, SCMP_ACT_ERRNO(EACCES), SCMP_SYS(socket), 0) != 0) {
        return NULL;
    }
    // seccomp_rule_add(ctx, SCMP_ACT_KILL, SYS_execve, 1, SCMP_A0(SCMP_CMP_NE, (scmp_datum_t)(_c->execute_path)))
    // SCMP_ACT_KILL: 如果进行对应的系统调用，则会把进程杀死
    // SYS_execve: 对应execve等函数
    // 1: 处理的系统调用参数个数
    // SCMP_A0(SCMP_CMP_NE, (scmp_datum_t)(_c->execute_path)):
    //     SCMP_A0(SCMP_CMP_NE, (scmp_datum_t)(_c->execute_path)):
    //         SCMP_CMP_NE: 当参数值不等于(scmp_datum_t)(_c->execute_path)的时候
    // seccomp_rule_add返回0为成功
    // 简述: 当调用execve系统调用的时候, 如果参数不等于_c->execute_path, 则进行拦截, 并进行SCMP_ACT_KILL处理。
    // SCMP_CMP()和SCMP_A{0-5}()的有效操作数:
    //     不等于 (SCMP_CMP_NE)、小于 (SCMP_CMP_LT)、小于等于 (SCMP_CMP_LE)、等于 (SCMP_CMP_EQ)、大于等于 (SCMP_CMP_GE)、大于 (SCMP_CMP_GT)、掩码参数等于基准值掩码 (SCMP_CMP_MASKED_EQ)
    if (seccomp_rule_add(ctx, SCMP_ACT_KILL, SYS_execve, 1, SCMP_A0(SCMP_CMP_NE, (scmp_datum_t)(_c->execute_path)))) {
        return NULL;
    }
    if (seccomp_rule_add(ctx, SCMP_ACT_KILL, SYS_open, 1, SCMP_CMP(1, SCMP_CMP_MASKED_EQ, O_WRONLY, O_WRONLY))) {
        return NULL;
    }
    if (seccomp_rule_add(ctx, SCMP_ACT_KILL, SYS_open, 1, SCMP_CMP(1, SCMP_CMP_MASKED_EQ, O_RDWR, O_RDWR))) {
        return NULL;
    }
    if (seccomp_rule_add(ctx, SCMP_ACT_KILL, SYS_openat, 1, SCMP_CMP(2, SCMP_CMP_MASKED_EQ, O_WRONLY, O_WRONLY))) {
        return NULL;
    }
    if (seccomp_rule_add(ctx, SCMP_ACT_KILL, SYS_openat, 1, SCMP_CMP(2, SCMP_CMP_MASKED_EQ, O_RDWR, O_RDWR))) {
        return NULL;
    }
    // 返回规则上下文
    return ctx;
}

scmp_filter_ctx plugin_handler(struct config *_c) {
    scmp_filter_ctx context = on_enable(_c);
    return context;
}
