cmake_minimum_required(VERSION 3.22)
project(judge_core C)
#set(CMAKE_C_FLAGS " ${CMAKE_C_FLAGS} --coverage")
set(CMAKE_C_STANDARD 99)
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
LINK_LIBRARIES(m)
LINK_LIBRARIES(seccomp)
add_executable(judge_core src/main.c)
file(GLOB SOURCE "src/*.c")
add_library(judge_include src/include/argtable3.h src/include/argtable3.c src/command/command_handler.c src/command/command_handler.h src/data/config.h src/command/command_args.h src/exit/exit_handler.c src/exit/exit_handler.h src/data/result.h src/data/result.c src/data/config.c src/execute/execute_handler.c src/thread/thread_handler.c src/thread/thread_handler.h src/kill/kill.h src/kill/kill.c src/execute/execute_handler.h src/execute/answer_code.h src/seccomp/seccomp_handler.c src/seccomp/seccomp_handler.h src/plugin/plugin_handler.c src/plugin/plugin_handler.h)
target_link_libraries(judge_core judge_include)
