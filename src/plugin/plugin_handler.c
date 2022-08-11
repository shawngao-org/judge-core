//
// Created by ShawnGao on 2022/8/3.
//
#include "stdio.h"
#include "dlfcn.h"
#include "seccomp.h"
#include "../data/config.h"
#include "plugin_handler.h"

void *out_lib;
int (*on_enable)(struct config *c, scmp_filter_ctx *context);
char *error_message;

int plugin_handler(struct config *_c, scmp_filter_ctx *context) {
    printf("Rule is loading...\n");
    out_lib = dlopen(_c->seccomp_rule_name, RTLD_LAZY);
    error_message = dlerror();

    if (error_message) {
        printf("Rule load failed.\n");
        printf("%s\n", error_message);
        return 0;
    }
    *(void **)(&on_enable) = dlsym(out_lib, "on_enable");
    error_message = dlerror();

    if (error_message) {
        printf("Rule enable failed.\n");
        printf("%s\n", error_message);
        return 0;
    }
    return on_enable(_c, context);
}
