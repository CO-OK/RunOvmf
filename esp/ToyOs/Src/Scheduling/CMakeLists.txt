add_library(PIT
    OBJECT
    PIT.c
)

target_include_directories(PIT
                            PUBLIC
                            ${CMAKE_SOURCE_DIR}/Include
                          )