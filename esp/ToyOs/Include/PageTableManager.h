#pragma once

#include<Paging.h>

typedef struct {
    /*每个进程都应该有一个PageTableManager*/
    PageMapLevel4Entry*PML4;
}PageTableManager;

extern PageTableManager GlobalPageTableManager;
/*实现虚实映射*/
void MapMemory(void* virtualMemory, void* physicalMemory,PageTableManager*pageTableManager);


