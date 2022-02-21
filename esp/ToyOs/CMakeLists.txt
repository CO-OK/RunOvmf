cmake_minimum_required(VERSION 3.12)
project(Kernel.elf LANGUAGES C ASM_NASM)

# 设置编译器
SET(CMAKE_C_COMPILER "/usr/bin/gcc")
# 编译器选项 不能用-m32

SET(CFLAGS "-fno-stack-protector -nostdlib  -ffreestanding -mno-red-zone")
SET(CMAKE_C_FLAGS "${CFLAGS}")

#


# 链接选项
set(LDFLAGS "  -e KernelStart -T ../Kernel.ld  font.o")
set(CMAKE_EXE_LINKER_FLAGS    "${LDFLAGS}")



add_subdirectory(Src)

add_subdirectory(Src/Paging)
add_subdirectory(Src/GDT)
add_subdirectory(Src/Interrupts)
add_subdirectory(Src/UserInputs)



add_executable(Kernel.elf )


target_link_libraries(Kernel.elf PUBLIC Video)
target_link_libraries(Kernel.elf PUBLIC Shell)
target_link_libraries(Kernel.elf PUBLIC Kernel)
#target_link_libraries(Kernel.elf PUBLIC Font)
target_link_libraries(Kernel.elf PUBLIC Memory32)
target_link_libraries(Kernel.elf PUBLIC Char)
target_link_libraries(Kernel.elf PUBLIC PageFrameAllocator)
target_link_libraries(Kernel.elf PUBLIC Assert)
target_link_libraries(Kernel.elf PUBLIC PageMapIndexer)
target_link_libraries(Kernel.elf PUBLIC PageTableManager)
target_link_libraries(Kernel.elf PUBLIC Paging)
target_link_libraries(Kernel.elf PUBLIC GDT)
target_link_libraries(Kernel.elf PUBLIC gdt)
target_link_libraries(Kernel.elf PUBLIC Interrupts)
target_link_libraries(Kernel.elf PUBLIC InterruptHandler)
target_link_libraries(Kernel.elf PUBLIC Panic)
target_link_libraries(Kernel.elf PUBLIC IO)

target_link_libraries(Kernel.elf PUBLIC KbScanCodeTranslation)
target_link_libraries(Kernel.elf PUBLIC KeyBoard)


