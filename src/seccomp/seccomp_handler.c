//
// Created by ShawnGao on 2022/7/31.
//
#include "sys/syscall.h"
#include "seccomp.h"
#include "string.h"
#include "fcntl.h"
#include "../data/config.h"
#include "../exit/exit_handler.h"
#include "../plugin/plugin_handler.h"

int seccomp_handler(struct config *_c) {
    scmp_filter_ctx ctx = NULL;
    ctx = seccomp_init(SCMP_ACT_ALLOW);
    ctx = NULL;
    return plugin_handler(_c->seccomp_rule_name, _c->execute_path);
}
