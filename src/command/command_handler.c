//
// Created by ShawnGao on 2022/7/30.
//
#include "stdio.h"
#include "string.h"
#include "../include/argtable3.h"
#include "../data/config.h"
#include "command_args.h"
#include "../exit/exit_handler.h"

/**
 * 命令参数处理器
 * @param arg_table 参数表
 * @param program_name 程序名称
 * @param _config 配置信息
 * @param _args 命令参数结构体
 */
void command_args_handler(void *arg_table, char program_name[], struct config *_config, const struct command_args *_args) {
    if (_args->help->count > 0) {
        printf("Usage: %s\n", program_name);
        arg_print_syntax(stdout, arg_table, "\n\n");
        arg_print_glossary(stdout, arg_table, " %-40s %s\n");
        normal_exit();
    }
    if (_args->version->count > 0) {
        printf("Judge core version: 0.1.8\n");
        normal_exit();
    }
    if (_args->max_cpu_time->count > 0) {
        _config->max_cpu_time = _args->max_cpu_time->ival[0];
    }
    if (_args->max_real_time->count > 0) {
        _config->max_real_time = _args->max_real_time->ival[0];
    }
    if (_args->skip_seccomp->count > 0) {
        _config->skip_seccomp = 1;
    } else {
        _config->skip_seccomp = 0;
    }
    if (_args->unlimited_memory->count <= 0) {
        if (_args->max_memory->count > 0) {
            _config->unlimited_memory = 0;
            _config->max_memory_size = (long) _args->max_memory->ival[0];
        }
    } else {
        _config->unlimited_memory = 1;
    }
    if (_args->max_stack->count > 0) {
        _config->max_stack_size = (long) _args->max_stack->ival[0];
    }
    if (_args->max_process->count > 0) {
        _config->max_process_amount = _args->max_process->ival[0];
    }
    if (_args->max_output_size->count > 0) {
        _config->max_output_size = (long) _args->max_output_size->ival[0];
    }
    if (_args->exec_path->count <= 0) {
        exception_exit(WRONG_CONFIG_VALUE);
    }
    _config->execute_path = _args->exec_path->sval[0];
    if (_args->input_path->count > 0) {
        _config->input_path = _args->input_path->sval[0];
    }
    if (_args->output_path->count > 0) {
        _config->output_path = _args->output_path->sval[0];
    }
    if (_args->err_path->count > 0) {
        _config->error_path = _args->err_path->sval[0];
    }
    if (_args->seccomp_rule->count > 0) {
        _config->seccomp_rule_name = _args->seccomp_rule->sval[0];
    }
    _config->args[0] = _config->execute_path;
    int i = 1;
    if (_args->args->count > 0) {
        for (; i < _args->args->count + 1; ++i) {
            _config->args[i] = _args->args->sval[i - 1];
        }
    }
    _config->args[i] = NULL;
    i = 0;
    if (_args->env->count > 0) {
        for (; i < _args->env->count; ++i) {
            _config->env[i] = _args->env->sval[i];
        }
    }
    _config->env[i] = NULL;
}
