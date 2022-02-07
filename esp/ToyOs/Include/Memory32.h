#pragma once
#include<Uefi.h>
#include<Char.h>
/*内存描述符结构体*/

typedef struct {
    UINT32 Type;
    EFI_PHYSICAL_ADDRESS PhysicalStart;
    EFI_VIRTUAL_ADDRESS VirtualStart;
    UINT64 NumberOfPages;
    UINT64 Attribute;
    /*占位 因为EFI_MEMORY_DESCRIPTOR在文档中是48字节*/
    UINT64 PadB;
} EFI_MEMORY_DESCRIPTOR;

UINT32 MemoryInit(MemoryMapStatus* MapStatus);
UINT32 EnablePaging();
void PrintMemoryMap(IN EFI_MEMORY_DESCRIPTOR* desc,IN int num);



