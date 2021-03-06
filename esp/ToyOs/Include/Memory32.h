#pragma once
#include<Uefi.h>
#include<Char.h>
#include<Bool.h>

//#include<PageFrameAllocator.h>
/*内存描述符结构体*/
typedef struct {
    UINT32 Type;
    EFI_PHYSICAL_ADDRESS PhysicalStart;
    EFI_VIRTUAL_ADDRESS VirtualStart;
    UINT64 NumberOfPages;
    UINT64 Attribute;
    /*占位 因为EFI_MEMORY_DESCRIPTOR在文档中是48字节 在整理内存时有别的作用*/
    UINT64 Pad;
} EFI_MEMORY_DESCRIPTOR;

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

/*内存初始化*/
UINT32 MemoryInit(MemoryMapStatus* MapStatus);
/*重整内存*/
UINT32 ResetMemory(IN EFI_MEMORY_DESCRIPTOR* desc,IN int num);
/*内存是否相接*/
UINT32 IsTogether(IN EFI_MEMORY_DESCRIPTOR*desc);
/*获得总内存大小*/
UINTN _GetTotallMemory(IN EFI_MEMORY_DESCRIPTOR* desc,IN int num);
typedef UINTN (*GetTotallMemoryEntry)();
UINTN GetTotallMemory();
void PrintMemoryMap(IN EFI_MEMORY_DESCRIPTOR* desc,IN int num);
/*memset*/
void memset(void* start, uint8_t value, uint64_t num);








