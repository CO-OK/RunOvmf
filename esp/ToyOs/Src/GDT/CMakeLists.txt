add_library(GDT
    OBJECT
    GDT.c
)

target_include_directories(GDT
                            PUBLIC
                            ${CMAKE_SOURCE_DIR}/Include
                          )