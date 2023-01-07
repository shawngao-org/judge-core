//
// Created by ShawnGao on 2022/8/3.
//
#include "stdio.h"
#include "dlfcn.h"
#include "seccomp.h"
#include "../data/config.h"
#include "plugin_handler.h"

void *out_lib;
scmp_filter_ctx (*on_enable)(struct config *c);
char *error_message;

scmp_filter_ctx plugin_handler(struct config *_c) {
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
    scmp_filter_ctx context = on_enable(_c);
    dlclose(out_lib);
    return context;
}
