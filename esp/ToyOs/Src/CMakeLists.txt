

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

add_library(Char
    OBJECT
    Char.c
)
target_include_directories(Video
                            PUBLIC
                            ${CMAKE_SOURCE_DIR}/Include
                          )
target_include_directories(Kernel
                            PUBLIC
                            ${CMAKE_SOURCE_DIR}/Include
                          )
target_include_directories(Shell
                            PUBLIC
                            ${CMAKE_SOURCE_DIR}/Include
                          )
target_include_directories(Char
                            PUBLIC
                            ${CMAKE_SOURCE_DIR}/Include
                          )