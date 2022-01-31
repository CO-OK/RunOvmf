cmake_minimum_required(VERSION 3.12)
project(Kernel.elf LANGUAGES C)

# 设置编译器
SET(CMAKE_C_COMPILER "/usr/bin/clang")

# 编译器选项

SET(CFLAGS "-fno-stack-protector -nostdlib")
SET(CMAKE_C_FLAGS "${CFLAGS}")

# 链接选项
set(LDFLAGS "-e KernelStart")
set(CMAKE_EXE_LINKER_FLAGS    "${LDFLAGS}")

add_library(Video
    OBJECT
    Video.c
)
add_library(Shell
    OBJECT
    Shell.c
)
add_library(Kernel
    OBJECT
    Kernel.c
)

add_executable(Kernel.elf )
target_link_libraries(Kernel.elf PUBLIC Video)
target_link_libraries(Kernel.elf PUBLIC Shell)
target_link_libraries(Kernel.elf PUBLIC Kernel)


