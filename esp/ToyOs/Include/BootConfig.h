#pragma once
#include<Uefi.h>
#include<Video.h>
#include<Memory32.h>
#include<PageTableManager.h>
#include<PageFrameAllocator.h>
typedef struct __attribute__((packed)){
    EFI_PHYSICAL_ADDRESS       FrameBufferBase;
    UINT64                      FrameBufferSize;
    UINT32                     HorizontalResolution;
    UINT32                     VerticalResolution;
    UINT32                     PixelsPerScanLine;
} VIDEO_CONFIG;



typedef struct
{
  VIDEO_CONFIG VideoConfig;
  MemoryMapStatus MemoryMap;
  PageTableManager pageTableManager;
  PageFrameAllocator* pageFrameAllocator;
}BOOT_CONFIG;