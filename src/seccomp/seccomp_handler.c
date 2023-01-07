//
// Created by ShawnGao on 2022/7/31.
//
#include "seccomp.h"
#include "string.h"
#include "../data/config.h"
#include "../exit/exit_handler.h"
#include "../plugin/plugin_handler.h"

int seccomp_handler(struct config *_c) {
    scmp_filter_ctx ctx = plugin_handler(_c);
    if (ctx == NULL) {
        exception_exit(INIT_SECCOMP_FAILED);
    }
    if (seccomp_load(ctx)) {
        exception_exit(LOAD_SECCOMP_FAILED);
    }
    return 0;
}
