//
// Created by ShawnGao on 2022/8/3.
//
#include "stdio.h"
#include "dlfcn.h"
#include "string.h"
#include "plugin_handler.h"

void *out_lib;
int (*on_enable)(char *exec);
char *error_message;

int plugin_handler(char *argv, char *exec) {
    printf("Plugin is loading...\n");
    out_lib = dlopen(argv, RTLD_LAZY);
    error_message = dlerror();

    if (error_message) {
        printf("Plugin load failed.\n");
        printf("%s\n", error_message);
        return -101;
    }
    *(void **)(&on_enable) = dlsym(out_lib, "on_enable");
    error_message = dlerror();

    if (error_message) {
        printf("Plugin enable failed.\n");
        printf("%s\n", error_message);
        return -102;
    }
    return on_enable(exec);
}
