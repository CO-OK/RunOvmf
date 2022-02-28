add_library(Heap
    OBJECT
    Heap.c
)

target_include_directories(Heap
                            PUBLIC
                            ${CMAKE_SOURCE_DIR}/Include
                          )