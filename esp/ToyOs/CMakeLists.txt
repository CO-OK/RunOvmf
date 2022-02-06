cmake_minimum_required(VERSION 3.12)
project(Kernel.elf LANGUAGES C)

# 设置编译器
SET(CMAKE_C_COMPILER "/usr/bin/clang")

# 编译器选项 不能用-m32

SET(CFLAGS "-fno-stack-protector -nostdlib -ffreestanding ")
SET(CMAKE_C_FLAGS "${CFLAGS}")

# 链接选项
set(LDFLAGS "-e KernelStart font.o")
set(CMAKE_EXE_LINKER_FLAGS    "${LDFLAGS}")



add_subdirectory(Src)




add_executable(Kernel.elf )

target_link_libraries(Kernel.elf PUBLIC Video)
target_link_libraries(Kernel.elf PUBLIC Shell)
target_link_libraries(Kernel.elf PUBLIC Kernel)
#target_link_libraries(Kernel.elf PUBLIC Font)
target_link_libraries(Kernel.elf PUBLIC Memory32)
target_link_libraries(Kernel.elf PUBLIC Char)


