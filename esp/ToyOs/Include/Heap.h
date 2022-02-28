#pragma once
#include<Uefi.h>
#include<Bool.h>
#include<PageFrameAllocator.h>
#include<PageTableManager.h>
extern PageFrameAllocator Allocator;
extern PageTableManager GlobalPageTableManager;
/*这个结构体用在每个内存块之前*/
typedef struct  HeapSegHeader{
    uint64_t length;
    struct HeapSegHeader* next;
    struct HeapSegHeader* last;
    bool free;
    // void CombineForward();
    // void CombineBackward();
    // HeapSegHdr* Split(uint64_t splitLength);
}HeapSegHeader;

/*初始化堆*/
void InitHeap(void* heapAddress, uint64_t pageCount);

void* malloc(uint64_t size);
void free(void* address);

void ExpandHeap(uint64_t length);

HeapSegHeader* SplitHeap(HeapSegHeader*hdr,uint64_t size);

void CombineForward(HeapSegHeader*hdr);
void CombineBackward(HeapSegHeader*hdr); 