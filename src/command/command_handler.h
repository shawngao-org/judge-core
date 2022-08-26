//
// Created by ShawnGao on 2022/7/30.
//

#ifndef JUDGE_CORE_COMMAND_HANDLER_H
#define JUDGE_CORE_COMMAND_HANDLER_H
#include "../data/config.h"

/**
 * 命令参数处理器
 * @param arg_table 参数表
 * @param program_name 程序名称
 * @param _config 配置信息
 * @param _args 命令参数结构体
 */
void command_args_handler(void *arg_table, char program_name[], struct config *_config, const struct command_args *_args);

#endif //JUDGE_CORE_COMMAND_HANDLER_H
