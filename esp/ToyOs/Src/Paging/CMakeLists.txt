add_library(PageFrameAllocator
    OBJECT
    PageFrameAllocator.c
)


target_include_directories(PageFrameAllocator
                            PUBLIC
                            ${CMAKE_SOURCE_DIR}/Include
                          )


add_library(PageMapIndexer
    OBJECT
    PageMapIndexer.c
)

target_include_directories(PageMapIndexer
                            PUBLIC
                            ${CMAKE_SOURCE_DIR}/Include
                          )


add_library(PageTableManager
    OBJECT
    PageTableManager.c
)

target_include_directories(PageTableManager
                            PUBLIC
                            ${CMAKE_SOURCE_DIR}/Include
                          )