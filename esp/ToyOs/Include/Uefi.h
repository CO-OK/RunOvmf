#pragma once

typedef void VOID;
typedef unsigned char UINT8;
typedef UINT8 CHAR8;
typedef unsigned int  UINT32;
typedef unsigned long long  UINT64;
typedef unsigned short UINT16;
typedef UINT64 UINTN;
typedef UINT64 EFI_PHYSICAL_ADDRESS;
typedef UINT64 EFI_VIRTUAL_ADDRESS;
typedef UINT16  uint16_t;
typedef UINT32  uint32_t;
typedef UINT64  uint64_t;
#define NULL   ((void* )0)

typedef struct{
    /*如果给的内存空间足够则返回实际占用的内存空间*/
    UINTN MemroyMapSize;
    /*memory descriptor 数组首地址*/
    VOID* MemoryMapBuffer;
    /*ExitBootServices时用到*/
    UINTN MapKey;
    /*单个memory descriptor的大小*/
    UINTN DescriptorSize;
    /**/
    UINT32 DescriptorVersion;

}MemoryMapStatus;

#pragma pack(1)
typedef struct
{
  UINT8 Blue;
  UINT8 Green;
  UINT8 Red;
  UINT8 Reserved;
} BGR_PIXEL;

typedef struct
{
    EFI_PHYSICAL_ADDRESS       FrameBufferBase;
    UINT64                      FrameBufferSize;
    UINT32                     HorizontalResolution;
    UINT32                     VerticalResolution;
    UINT32                     PixelsPerScanLine;
} VIDEO_CONFIG;

#pragma pack()

typedef struct
{
  VIDEO_CONFIG VideoConfig;
  MemoryMapStatus MemoryMap;
} BOOT_CONFIG;
