

# Simple Judge Core

一个简单的判题核心, 核心的Seccomp规则可以通过插件的形式载入。(文档写的比较摆烂, 凑合看吧)
<br />
***注：这是一个未经过测试的预览版。***
<br />
[![State-of-the-art Shitcode](https://img.shields.io/static/v1?label=State-of-the-art&message=Shitcode&color=7B5804)](https://github.com/trekhleb/state-of-the-art-shitcode)
<br />
[![Build Status](https://jenkins.shawngao.org/buildStatus/icon?job=GiteaTeam%2Fjudge-core%2Fmain&build=3)](https://jenkins.shawngao.org/job/GiteaTeam/job/judge-core/job/main/3/)
<br />

<p align="center">
  <a href="#">
    <img src="./images/cover.png" alt="Logo" width="500"/>
  </a>
</p>
<h3 align="center">简单的判题核心</h3>
<p align="center">
  <br />
</p>

### 上手指南

###### 开发前的配置要求

1. 开发工具: CLion 2022.1.3 (需要配置远程开发, 除非开发机器是Linux环境, 不会配置可以移步至[Google](https://www.google.com/)或者[Baidu](https://www.baidu.com/))
2. 系统: Ubuntu / Debian / CentOS[不推荐，且尚未测试]
2. 环境: GCC / CMake (>= 3.22) / libseccomp2 / libseccomp-dev | (如果系统为Ubuntu或者Debian可以直接进行构建, 脚本会自动检查并安装环境)

##### **构建**

在项目根目录 (CMakeLists.txt所在目录) 中执行如下命令
```sh
bash build.sh
```

### 文件目录说明

```
judge-core/ #主目录
├── CMakeLists.txt
├── README.md
├── build.sh
├── images
│ ├── cover.png
│ └── logo.png
└── src #源代码目录
    ├── command #命令
    │ ├── command_args.h
    │ ├── command_handler.c
    │ └── command_handler.h
    ├── data #主要的数据类型
    │ ├── config.c
    │ ├── config.h
    │ ├── result.c
    │ └── result.h
    ├── execute #执行
    │ ├── answer_code.h
    │ ├── execute_handler.c
    │ └── execute_handler.h
    ├── exit #退出
    │ ├── exit_handler.c
    │ └── exit_handler.h
    ├── include #外部库
    │ ├── argtable3.c
    │ └── argtable3.h
    ├── kill #杀死进程
    │ ├── kill.c
    │ └── kill.h
    ├── main.c #入口文件
    ├── plugin #插件处理
    │ ├── plugin_handler.c
    │ └── plugin_handler.h
    ├── seccomp #规则处理
    │ ├── seccomp_handler.c
    │ └── seccomp_handler.h
    └── thread #线程处理
        ├── thread_handler.c
        └── thread_handler.h

```

### 参数说明

```
 -h, --help                          显示帮助
 -v, --version                       显示版本信息
 --unlimited-memory                  没有设置内存限制(只针对Java等代码的运行). 此选项比 --memory 具有更高的优先级
 --cpu=<int value>                   设置最大CPU时间, 单位: ms
 --real=<int value>                  设置最大真实运行时间, 单位: ms
 --memory=<int value>                设置最大使用内存, 单位: byte
 --stack=<int value>                 设置最大栈大小, 单位, byte (默认: 16777216 byte, 合计16MB)
 --proc=<int value>                  设置最大进程数
 --output-size=<int value>           设置最大输出大小, 单位: byte
 --exec-path=<string value>          设置要执行的二进制文件位置, 例如: /bin/ls
 --input-path=<string value>         设置输入内容位置, 默认: /dev/stdin
 --output-path=<string value>        设置输出位置, 默认: /dev/stdout
 --err-path=<string value>           设置错误内容位置, 默认: /dev/stderr
 --args=<string value>               设置运行的二进制文件的命令行参数, 数量, 0~255个, 例如: -a
 --env=<string value>                设置相应的环境变量, 数量: 0~255个, 例如: PATH=/bin
 --seccomp=<plugin file name (*.so)> 设置Seccomp规则插件位置, 例如: /home/ubuntu/plugin.so
```

### 关于插件

> 注: 核心中不含任何Seccomp规则, 所以在使用之前必须指定规则插件

> ***所有插件必须通过可信来源获取, 否则会出现安全问题。***

> ***如果插件的源代码不是自己写的, 建议检查源码中是否有不安全的内容。***

- 插件制作很简单，步骤如下：

  1. 新建一个C源文件, 例如: plugin_test.c
  2. 然后在源文件中写如下代码段: 
```c
// 插件主要用于Seccomp规则的自定义 (例子)
// 该代码是一个通用的规则。
#include "stdio.h"
#include "seccomp.h"
#include "fcntl.h"
#include "errno.h"
#include "syscall.h"

/**
 * 存放配置的结构体，可以直接使用这里面的数据。
 * 注：配置中所有配置均为已经配置完成，修改不会有任何作用
 */
struct config {
    int max_cpu_time;
    int max_real_time;
    long max_memory_size;
    long max_stack_size;
    int max_process_amount;
    long max_output_size;
    const char *execute_path;
    const char *input_path;
    const char *output_path;
    const char *error_path;
    const char *args[255];
    const char *env[255];
    const char *seccomp_rule_name;
};

// 插件起点函数
// *_c: 配置数据, 内容见struct config
// (规则设定好之后会自动被加载, 不需要写seccomp_load)
scmp_filter_ctx on_enable(struct config *_c) {
    // 提示, 可有可无
    printf("Rule was loaded.\n");
    scmp_filter_ctx ctx = NULL;
    ctx = seccomp_init(SCMP_ACT_ALLOW);
    if (!ctx) {
        return NULL;
    }
    // 黑名单 (本插件例子使用allow模式, deny模式请自行更改)
    int _blacklist[] = {SYS_clone, SYS_execveat, SYS_fork, SYS_kill, SYS_vfork};
    // 计算黑名单元素的个数
    int _blacklist_length = sizeof(_blacklist) / sizeof(int);
    // 循环添加规则
    for (int i = 0; i < _blacklist_length; i++) {
        if (seccomp_rule_add(ctx, SCMP_ACT_KILL, _blacklist[i], 0)) {
            return NULL;
        }
    }
    // 阻止操作socket
    if (seccomp_rule_add(ctx, SCMP_ACT_ERRNO(EACCES), SCMP_SYS(socket), 0) != 0) {
        return NULL;
    }
    // 这段比较重要, 因为这个涉及到了是否能正常执行“execute_path”指定的程序。
    // seccomp_rule_add(ctx, SCMP_ACT_KILL, SYS_execve, 1, SCMP_A0(SCMP_CMP_NE, (scmp_datum_t)(_c->execute_path)))
    // SCMP_ACT_KILL: 如果进行对应的系统调用，则会把进程杀死
    // SYS_execve: 对应execve等函数
    // 1: 处理的系统调用参数个数
    // SCMP_A0(SCMP_CMP_NE, (scmp_datum_t)(_c->execute_path)):
    //     SCMP_A0(SCMP_CMP_NE, (scmp_datum_t)(_c->execute_path)):
    //         SCMP_CMP_NE: 当参数值不等于(scmp_datum_t)(_c->execute_path)的时候
    // seccomp_rule_add返回0为成功
    // 简述: 当调用execve系统调用的时候, 如果参数不等于_c->execute_path, 则进行拦截, 并进行SCMP_ACT_KILL处理。
    // SCMP_CMP()和SCMP_A{0-5}()的有效操作数:
    //     不等于 (SCMP_CMP_NE)、小于 (SCMP_CMP_LT)、小于等于 (SCMP_CMP_LE)、等于 (SCMP_CMP_EQ)、大于等于 (SCMP_CMP_GE)、大于 (SCMP_CMP_GT)、掩码参数等于基准值掩码 (SCMP_CMP_MASKED_EQ)
    if (seccomp_rule_add(ctx, SCMP_ACT_KILL, SYS_execve, 1, SCMP_A0(SCMP_CMP_NE, (scmp_datum_t)(_c->execute_path)))) {
        return NULL;
    }
    if (seccomp_rule_add(ctx, SCMP_ACT_KILL, SYS_open, 1, SCMP_CMP(1, SCMP_CMP_MASKED_EQ, O_WRONLY, O_WRONLY))) {
        return NULL;
    }
    if (seccomp_rule_add(ctx, SCMP_ACT_KILL, SYS_open, 1, SCMP_CMP(1, SCMP_CMP_MASKED_EQ, O_RDWR, O_RDWR))) {
        return NULL;
    }
    if (seccomp_rule_add(ctx, SCMP_ACT_KILL, SYS_openat, 1, SCMP_CMP(2, SCMP_CMP_MASKED_EQ, O_WRONLY, O_WRONLY))) {
        return NULL;
    }
    if (seccomp_rule_add(ctx, SCMP_ACT_KILL, SYS_openat, 1, SCMP_CMP(2, SCMP_CMP_MASKED_EQ, O_RDWR, O_RDWR))) {
        return NULL;
    }
    // 返回规则上下文
    return ctx;
}
```
  3. 编译：

```shell
> gcc -c -fpic <源文件> -lseccomp
> gcc -shared -lc -lseccomp -o <文件名.so> <生成的.o文件>

# 也可以使用build_plugin.sh脚本进行构建

> bash build_plugin.sh
Enter your plugin resource code file name (eg. filename1): plugin_test
```



  4. 在运行核心的时候只需要指定“--seccomp=”参数的值就可以生效了 (值为你生成的.so文件的路径及文件名)

### 返回值

- 结果码

|值|说明|
|---|---|
|1|内存超限|
|2|运行时错误|
|3|实际运行时间超限|
|4|CPU运行时间超限|
|-10 or 246|系统错误|

- 退出码

|值| 说明                   |
|---|----------------------|
|-1| 错误的配置值               |
|-2| 无效的操作                |
|-3| 子进程创建失败              |
|-4| 设置Rlimit失败           |
|-5| IO异常                 |
|-6| 句柄拷贝失败               |
|-7| 程序执行失败 (--exec-path) |
|-8| 不能创建线程               |
|-9| 等待错误                 |
|-10| 设置Seccomp失败          |
|-11| 加载Seccomp失败          |
|-12| 初始化Seccomp失败         |

### 使用到的库

- [libseccomp](https://github.com/seccomp/libseccomp)
- [argtable3](https://github.com/argtable/argtable3)

### 鸣谢

> CLion 是一个在各个方面都最大程度地提高开发人员的生产力的 IDE

在此特别鸣谢 JetBrains 为开源和教育学习提供的免费 CLion 授权

<img src="./images/jetbrains-variant-3.png" alt="Logo" width="200"/>
