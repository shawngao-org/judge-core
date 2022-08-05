

# Simple Judge Core

一个简单的判题核心，核心的Seccomp规则可以通过插件的形式载入。（文档写的比较摆烂，凑合看吧）
<br />
[![State-of-the-art Shitcode](https://img.shields.io/static/v1?label=State-of-the-art&message=Shitcode&color=7B5804)](https://github.com/trekhleb/state-of-the-art-shitcode)
<br />

<p align="center">
  <a href="https://github.com/ftpmsgx/judge-core">
    <img src="./images/cover.png" alt="Logo" width="500">
  </a>

  <h3 align="center">简单的判题核心</h3>
  <p align="center">
    <br />
    <a href="https://github.com/ftpmsgx/judge-core/issues">报告Bug</a>
    ·
    <a href="https://github.com/ftpmsgx/judge-core/issues">提出新特性</a>
  </p>

</p>

### 上手指南

###### 开发前的配置要求

1. 开发工具：CLion 2022.1.3（需要配置远程开发，除非开发机器是Linux环境,不会配置可以移步至[Google](https://www.google.com/)或者[Baidu](https://www.baidu.com/)）
2. 系统以及环境：Ubuntu Server、GCC、GNU make、cmake、libseccomp-dev、libseccomp2

##### **构建**
在项目根目录（CMakeLists.txt所在目录）中执行如下命令
```sh
# 生成 Makefile
cmake .
# 构建可执行文件
make
```

### 文件目录说明

```
judge-core/ #主目录
├── CMakeLists.txt
├── README.md
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
 --cpu=<int value>                   设置最大CPU时间，单位：ms
 --real=<int value>                  设置最大真实运行时间，单位：ms
 --memory=<int value>                设置最大使用内存，单位：byte
 --stack=<int value>                 设置最大栈大小，单位：byte（默认：16777216 byte，合计16MB）
 --proc=<int value>                  设置最大进程数
 --output-size=<int value>           设置最大输出大小，单位：byte
 --exec-path=<string value>          设置要执行的二进制文件位置，例如：/bin/ls
 --input-path=<string value>         设置输入内容位置，默认：/dev/stdin
 --output-path=<string value>        设置输出位置，默认：/dev/stdout
 --err-path=<string value>           设置错误内容位置，默认：/dev/stderr
 --args=<string value>               设置运行的二进制文件的命令行参数，数量：0~255个，例如：-a
 --env=<string value>                设置相应的环境变量，数量：0~255个，例如：PATH=/bin
 --seccomp=<plugin file name (*.so)> 设置Seccomp规则插件位置，例如：/home/ubuntu/plugin.so
```

### 关于插件

> 注：核心中不含任何Seccomp规则，所以在使用之前必须指定规则插件

- 插件制作很简单，步骤如下：

  1. 新建一个C源文件，例如：plugin_test.c
  2. 然后在源文件中写如下代码段：
    ```c
    #include "stdio.h"
    #include "sys/syscall.h"
    #include "seccomp.h"
    #include "string.h"
    #include "fcntl.h"
    // char *exec => 就是在执行的时候的“--exec-path=”参数的值
    int on_enable(char *exec) {
        scmp_filter_ctx context = NULL;
        // 这里可以是SCMP_ACT_KILL，也可以是SCMP_ACT_ALLOW，根据具体情况填写
        context = seccomp_init(SCMP_ACT_KILL);
        // 从这里开始自定义你的规则
        return seccomp_load(context);
    }
    ```
  3. 编译：
    ```sh
    gcc -c -fpic <源文件> -lseccomp
    gcc -shared -lc -lseccomp -o <文件名.so> <生成的.o文件>
    ```
  4. 在运行核心的时候只需要指定“--seccomp=”参数的值就可以生效了（值为你生成的.so文件的路径及文件名）

### 使用到的库

- [libseccomp](https://github.com/seccomp/libseccomp)
- [argtable3](https://github.com/argtable/argtable3)

### 鸣谢

> CLion 是一个在各个方面都最大程度地提高开发人员的生产力的 IDE

在此特别鸣谢 JetBrains 为开源和教育学习提供的免费 CLion 授权

<img src="./images/jetbrains-variant-3.png" alt="Logo" width="200">
