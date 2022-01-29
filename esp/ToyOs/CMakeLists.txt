cmake_minimum_required(VERSION 3.12)
project(Kernel LANGUAGES C)

# 设置编译器
SET(CMAKE_C_COMPILER "/usr/bin/clang")

# 编译器选项

SET(CFLAGS "-nostdlib")
SET(CMAKE_C_FLAGS "${CFLAGS}")

add_executable(Kernel.elf Kernel.c Video.c)