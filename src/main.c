#include <stdio.h>
#include "include/argtable3.h"
#include "command/command_args.h"
#include "command/command_handler.h"
#include "exit/exit_handler.h"
#include "execute/execute_handler.h"
#include "data/result.h"

#define INT_PLACE "<int value>"
#define STR_PLACE "<string value>"

struct command_args command_args_list;

int main(int argc, char *argv[]) {
    void *arg_table[] = {
            command_args_list.help = arg_litn("h", "help", 0, 1, "Display help pages"),
            command_args_list.version = arg_litn("v", "version", 0, 1, "Display version information."),
            command_args_list.unlimited_memory = arg_litn(NULL, "unlimited-memory", 0, 1, "No memory limit set "
                                                                                          "(Only for the operation of codes such as Java). "
                                                                                          "This option has higher priority than --memory."),
            command_args_list.skip_seccomp = arg_litn(NULL, "skip-seccomp", 0, 1, "Skip Seccomp filter."),

            command_args_list.max_cpu_time = arg_intn(NULL, "cpu", INT_PLACE, 0, 1, "Set max CPU time (ms)"),
            command_args_list.max_real_time = arg_intn(NULL, "real", INT_PLACE, 0, 1, "Set max real time (ms)"),
            command_args_list.max_memory = arg_intn(NULL, "memory", INT_PLACE, 0, 1, "Set max memory (byte)"),
            command_args_list.max_stack = arg_intn(NULL, "stack", INT_PLACE, 0, 1,
                                 "Set max stack size (byte, default: 16777216 byte)"),
            command_args_list.max_process = arg_intn(NULL, "proc", INT_PLACE, 0, 1, "Set max process."),
            command_args_list.max_output_size = arg_intn(NULL, "output-size", INT_PLACE, 0, 1, "Set max output size (byte)."),

            command_args_list.exec_path = arg_strn(NULL, "exec-path", STR_PLACE, 0, 1, "Set execute file path."),
            command_args_list.input_path = arg_strn(NULL, "input-path", STR_PLACE, 0, 1, "Set input path."),
            command_args_list.output_path = arg_strn(NULL, "output-path", STR_PLACE, 0, 1, "Set output path."),
            command_args_list.err_path = arg_strn(NULL, "err-path", STR_PLACE, 0, 1, "Set error output path."),
            command_args_list.args = arg_strn(NULL, "args", STR_PLACE, 0, 255, "Set parameters. (Amount: 0 ~ 255)"),
            command_args_list.env = arg_strn(NULL, "env", STR_PLACE, 0, 255, "Set env. (Amount: 0 ~ 255)"),
            command_args_list.seccomp_rule = arg_strn(NULL, "seccomp", "<Rule file name (*.so)>", 0, 1,
                                                      "Set seccomp rule name."
                                                      "(Deprecated, may be removed in a future release, "
                                                      "no longer available at this stage)"),

            command_args_list.end = arg_end(20)
    };
    char program_name[] = "judge_core";
    int err = arg_parse(argc, argv, arg_table);
    struct config _config;
    if (err > 0) {
        arg_print_errors(stdout, command_args_list.end, program_name);
        printf("Try '%s --help' or '%s -h' for more information.\n", program_name, program_name);
        arg_free(arg_table);
        exception_exit(INVALID_OPTION);
    }
    init_config(&_config);
    command_args_handler(arg_table, program_name, &_config, &command_args_list);
    struct result _result;
    init_result(&_result);
    execute_handler(&_config, &_result);
    printf("{\n");
    printf("\t\"cpu\": %d,\n", _result.cpu_time);
    printf("\t\"real\": %d,\n", _result.real_time);
    printf("\t\"memory\": %ld,\n", _result.memory);
    printf("\t\"signal\": %d,\n", _result.signal_code);
    printf("\t\"exit\": %d,\n", _result.exit_code);
    printf("\t\"result\": %d\n", _result.result);
    printf("}\n");
    return 0;
}
