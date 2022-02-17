#pragma once
#include<Memory32.h>

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
typedef bool (*SetBitMapEntry)(IN UINTN ,IN bool);
bool _SetBitMap(IN bool value,IN UINTN index,IN bool* MapBase);
bool SetBitMap(IN UINTN index ,IN bool value);

/*BitMap操作的结构体*/
typedef struct{
    GetBitMapValueEntry GetMapValue;
    SetBitMapEntry SetMap;
}BitMapControlTable;

typedef void (*FreePageEntry)(void*);
typedef void (*FreePagesEntry)(void*,UINT64);
typedef void (*LockPageEntry)(void*);
typedef void (*LockPagesEntry)(void*,UINT64);
typedef void (*ReservePageEntry)(void*);
typedef void (*UnReservePageEntry)(void*);
typedef void (*ReservePagesEntry)(void*,UINT64);
typedef void (*UnReservePagesEntry)(void*,UINT64);
typedef uint64_t (*GetMemoryEntry)();
typedef void* (*RequestPageEntry)();
/*PageFrame 操作结构体*/
typedef struct{
    FreePageEntry FreePage;
    FreePagesEntry FreePages;
    LockPageEntry LockPage;
    LockPagesEntry LockPages;
    ReservePageEntry ReservePage;
    ReservePagesEntry ReservePages;
    UnReservePageEntry UnReservePage;
    UnReservePagesEntry UnReservePages;
    GetMemoryEntry GetFreeMemory;
    GetMemoryEntry GetUsedMemory;
    GetMemoryEntry GetReservedMemory;
    RequestPageEntry RequestPage;
    
}PagesControlTable;

/*获得EFI内存信息*/
void ReadEFIMemoryMap(IN EFI_MEMORY_DESCRIPTOR* desc,IN int num,IN UINTN DescriptorSize);
/*初始化BitMap*/
static void InitBitMap(UINT64 bitMapSize,void*bitMapBase);
/*释放页*/
void FreePage(void*pageAddress);
/*释放页*/
void FreePages(void*pageAddress,UINT64 pageCount);
/*锁定页*/
void LockPage(void*pageAddress);
/*锁定页*/
void LockPages(void*pageAddress,UINT64 pageCount);
/*保留页*/
void ReservePage(void*pageAddress);
/*释放保留页*/
void UnReservePage(void*pageAddress);
/*保留页*/
void ReservePages(void*pageAddress,UINT64 pageCount);
/*释放保留页*/
void UnReservePages(void*pageAddress,UINT64 pageCount);
/*获取内存信息*/
uint64_t GetFreeMemory();
uint64_t GetUsedMemory();
uint64_t GetReservedMemory();

/*申请页*/
void* RequestPage();
/*PageFrameAllocator*/
typedef struct{
    BitMap PageBitMap;
    BitMapControlTable bitMapCtrlTable;
    PagesControlTable pageCtrlTable;
}PageFrameAllocator;