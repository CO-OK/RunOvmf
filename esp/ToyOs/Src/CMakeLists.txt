

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

add_library(Memory32
    OBJECT
    Memory32.c
)



add_library(Assert
    OBJECT
    Assert.c
)

add_library(Panic
    OBJECT
    Panic.c
)

add_library(IO
    OBJECT
    IO.c
)

add_library(Acpi
    OBJECT
    Acpi.c
)

add_library(PCI
    OBJECT
    PCI.c
)

add_library(PciDescriptor
    OBJECT
    PciDescriptor.c
)

add_library(PowerManage
    OBJECT
    PowerManage.c
)

add_library(AHCI
    OBJECT
    AHCI.c
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
target_include_directories(Memory32
                            PUBLIC
                            ${CMAKE_SOURCE_DIR}/Include
                          )
                        
target_include_directories(Assert
                            PUBLIC
                            ${CMAKE_SOURCE_DIR}/Include
                          )

target_include_directories(Panic
                            PUBLIC
                            ${CMAKE_SOURCE_DIR}/Include
                          )

target_include_directories(IO
                            PUBLIC
                            ${CMAKE_SOURCE_DIR}/Include
                          )

target_include_directories(Acpi
                            PUBLIC
                            ${CMAKE_SOURCE_DIR}/Include
                          )

target_include_directories(PCI
                            PUBLIC
                            ${CMAKE_SOURCE_DIR}/Include
                          )

target_include_directories(PciDescriptor
                            PUBLIC
                            ${CMAKE_SOURCE_DIR}/Include
                          )

target_include_directories(PowerManage
                            PUBLIC
                            ${CMAKE_SOURCE_DIR}/Include
                          )

target_include_directories(AHCI
                            PUBLIC
                            ${CMAKE_SOURCE_DIR}/Include
                          )