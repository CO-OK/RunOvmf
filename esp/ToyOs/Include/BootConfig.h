#pragma once
#include<Uefi.h>
#include<Video.h>
#include<Memory32.h>

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
  // PageTableManager pageTableManager;
  // PageFrameAllocator* pageFrameAllocator;
  void* rsdp;//root system description pointer
}BOOT_CONFIG;