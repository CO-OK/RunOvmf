add_library(KbScanCodeTranslation
    OBJECT
    KbScanCodeTranslation.c
)

target_include_directories(KbScanCodeTranslation
                            PUBLIC
                            ${CMAKE_SOURCE_DIR}/Include
                          )


add_library(KeyBoard
    OBJECT
    KeyBoard.c
)

target_include_directories(KeyBoard
                            PUBLIC
                            ${CMAKE_SOURCE_DIR}/Include
                          )