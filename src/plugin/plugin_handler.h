//
// Created by ShawnGao on 2022/8/3.
//

#ifndef JUDGE_CORE_PLUGIN_HANDLER_H
#define JUDGE_CORE_PLUGIN_HANDLER_H
#include "seccomp.h"
#include "../data/config.h"

scmp_filter_ctx plugin_handler(struct config *_c);

#endif //JUDGE_CORE_PLUGIN_HANDLER_H
