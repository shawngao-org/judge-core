//
// Created by ShawnGao on 2022/7/31.
//
#include "sys/syscall.h"
#include "seccomp.h"
#include "string.h"
#include "fcntl.h"
#include "../data/config.h"
#include "../exit/exit_handler.h"

int seccomp_handler(struct config *_c) {
    scmp_filter_ctx ctx = NULL;
    if (strcmp(_c->seccomp_rule_name, "c_or_cpp_white_rule") == 0) {
        ctx = seccomp_init(SCMP_ACT_KILL);
        int c_or_cpp_white_rule[] = {SYS_read, SYS_readv, SYS_write, SYS_writev,
                                     SYS_fstat, SYS_mmap, SYS_mprotect, SYS_munmap,
                                     SYS_uname, SYS_arch_prctl, SYS_brk, SYS_access,
                                     SYS_exit_group, SYS_close, SYS_readlink, SYS_readlinkat,
                                     SYS_sysinfo, SYS_lseek, SYS_clock_gettime, SYS_pread64,
                                     SYS_open, SYS_dup, SYS_dup2, SYS_dup3};
        int c_or_cpp_white_rule_len = sizeof(c_or_cpp_white_rule) / sizeof(int);
        for (int i = 0; i <= c_or_cpp_white_rule_len - 1; ++i) {
            int add_seccomp_rule_result = seccomp_rule_add(ctx, SCMP_ACT_ALLOW, c_or_cpp_white_rule[i], 0);
            if (add_seccomp_rule_result != 0) {
                return SET_SECCOMP_FAILED;
            }
        }
        seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SYS_openat, 1, SCMP_CMP(1, SCMP_CMP_EQ, O_WRONLY | O_RDWR, 0));
        seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SYS_openat2, 1, SCMP_CMP(1, SCMP_CMP_EQ, O_WRONLY | O_RDWR, 0));
        seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SYS_execve, 1, SCMP_CMP(SCMP_CMP_EQ, (scmp_datum_t) _c->execute_path));
        return seccomp_load(ctx);
    }
    if (strcmp(_c->seccomp_rule_name, "sys") == 0) {
        ctx = seccomp_init(SCMP_ACT_ALLOW);
        int black_list[] = {SYS_kill, SYS_fork, SYS_vfork, SYS_clone, SYS_clone3, SYS_execveat};
        int black_list_len = sizeof(black_list) / sizeof(int);
        for (int i = 0; i <= black_list_len - 1; ++i) {
            int add_seccomp_rule_result = seccomp_rule_add(ctx, SCMP_ACT_KILL, black_list[i], 0);
            if (add_seccomp_rule_result != 0) {
                return SET_SECCOMP_FAILED;
            }
        }
        return seccomp_load(ctx);
    }
    return SET_SECCOMP_FAILED;
}
