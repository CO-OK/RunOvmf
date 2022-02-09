#pragma once
#include<Uefi.h>
#include<Char.h>
#include<Bool.h>
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


/*page frame allocator*/

/*采用bitmap*/
typedef struct{
    /*bit map长度*/
    UINTN size;
    /*bit map起始地址*/
    bool* MapBase;
}BitMap;

/*获得BitMap具体值*/
typedef bool (*GetBitMapValueEntry)(IN UINTN);
bool _GetBitMapValue(IN UINTN index,IN bool* MapBase);
bool GetBitMapValue(IN UINTN index);

/*设置BitMap具体值*/
typedef void (*SetBitMapEntry)(IN bool ,IN UINTN );
void _SetBitMap(IN bool value,IN UINTN index,IN bool* MapBase);
void SetBitMap(IN bool value,IN UINTN index);

/*内存操作的结构体*/
typedef struct{
    GetBitMapValueEntry GetMapValue;
    SetBitMapEntry SetMap;
    GetTotallMemoryEntry GetTotalMemory;
}MemoryControlTable;





