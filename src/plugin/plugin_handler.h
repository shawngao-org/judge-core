//
// Created by ShawnGao on 2022/8/3.
//

#ifndef JUDGE_CORE_PLUGIN_HANDLER_H
#define JUDGE_CORE_PLUGIN_HANDLER_H
#include "seccomp.h"
#include "../data/config.h"

int plugin_handler(struct config *_c, scmp_filter_ctx *context);

#endif //JUDGE_CORE_PLUGIN_HANDLER_H
