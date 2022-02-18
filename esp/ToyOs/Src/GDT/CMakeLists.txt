add_library(GDT
    OBJECT
    GDT.c
)

target_include_directories(GDT
                            PUBLIC
                            ${CMAKE_SOURCE_DIR}/Include
                          )

add_library(gdt
    OBJECT
    gdt.asm
)
set_property(SOURCE gdt.asm APPEND PROPERTY COMPILE_OPTIONS  "-f elf64")

