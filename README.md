

# Simple Judge Core

一个简单的判题核心, 核心的Seccomp规则可以通过插件的形式载入。(文档写的比较摆烂, 凑合看吧)
<br />
***注：这是一个未经过测试的预览版。***
<br />
[![State-of-the-art Shitcode](https://img.shields.io/static/v1?label=State-of-the-art&message=Shitcode&color=7B5804)](https://github.com/trekhleb/state-of-the-art-shitcode)
<br />
[![pipeline status](http://gitlab.shawngao.org/root/judge-core/badges/main/pipeline.svg)](http://gitlab.shawngao.org/root/judge-core/-/commits/main) [![Latest Release](http://gitlab.shawngao.org/root/judge-core/-/badges/release.svg)](http://gitlab.shawngao.org/root/judge-core/-/releases)
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
 --seccomp=<plugin file name (*.so)> 设置Seccomp规则插件位置, 例如: /home/ubuntu/plugin.so (已弃用, 可能会在未来版本中删除, 现阶段已没有任何作用)
```

### 返回值

- 结果码

| 值          | 说明        |
|------------|-----------|
| 1          | 内存超限      |
| 2          | 运行时错误     |
| 3          | 实际运行时间超限  |
| 4          | CPU运行时间超限 |
| -10 or 246 | 系统错误      |

- 退出码

| 值   | 说明                   |
|-----|----------------------|
| -1  | 错误的配置值               |
| -2  | 无效的操作                |
| -3  | 子进程创建失败              |
| -4  | 设置Rlimit失败           |
| -5  | IO异常                 |
| -6  | 句柄拷贝失败               |
| -7  | 程序执行失败 (--exec-path) |
| -8  | 不能创建线程               |
| -9  | 等待错误                 |
| -10 | 设置Seccomp失败          |
| -11 | 加载Seccomp失败          |
| -12 | 初始化Seccomp失败         |

### 使用到的库

- [libseccomp](https://github.com/seccomp/libseccomp)
- [argtable3](https://github.com/argtable/argtable3)

### 鸣谢

> CLion 是一个在各个方面都最大程度地提高开发人员的生产力的 IDE

在此特别鸣谢 JetBrains 为开源和教育学习提供的免费 CLion 授权

<img src="./images/jetbrains-variant-3.png" alt="Logo" width="200"/>
