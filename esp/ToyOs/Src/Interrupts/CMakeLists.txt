add_library(Interrupts
    OBJECT
    Interrupts.c
)

target_include_directories(Interrupts
                            PUBLIC
                            ${CMAKE_SOURCE_DIR}/Include
                          )

add_library(InterruptHandler
    OBJECT
    InterruptHandler.c
)

target_include_directories(InterruptHandler
                            PUBLIC
                            ${CMAKE_SOURCE_DIR}/Include
                          )

#中断只能使用通用寄存器
set_property(SOURCE InterruptHandler.c APPEND PROPERTY COMPILE_OPTIONS  "-mno-red-zone" "-mgeneral-regs-only")