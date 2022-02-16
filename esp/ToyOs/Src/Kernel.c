#include <Kernel.h>
#include<PageFrameAllocator.h>
#include<Assert.h>
#include<Paging.h>
#include<PageTableManager.h>
FILENUM(1);
extern PageFrameAllocator Allocator;
/*内核起始*/
extern uint64_t _KernelStart;
/*内核终止*/
extern uint64_t _KernelEnd;



UINT64 KernelStart(BOOT_CONFIG *BootConfig)
{  
    
    UINT64 PassBack = 0;
    VideoInitial(BootConfig);
    //PrintStr("VideoInit done...\n");
    //ShellInitial();
    CharInit();
    //PrintStr("CharInit done...\n");
    //ASSERT(7==8);
    MemoryInit(&BootConfig->MemoryMap);
    
    // MemCtrlTable.SetMap(0,false);
    // MemCtrlTable.SetMap(1,true);
    // MemCtrlTable.SetMap(2,false);
    // for(int i=0;i<24;i++){
        
    //     printf("%d\n",MemCtrlTable.GetMapValue(i));
        
       
    // }
    //printf("KernelStart=%x\nKernelEnd=%x\n",&_KernelStart,&_KernelEnd);
    //printf("KernelSize=%x\n",&_KernelEnd-&_KernelStart);

    PageMapLevel4Entry * PML4 = (PageMapLevel4Entry*)RequestPage();
    memset(PML4,0,4096);
    PageTableManager pageTableManager;
    pageTableManager.PML4=PML4;
    for(uint64_t i=0;i<GetTotallMemory();i+=4096){
        MapMemory((void*)i,(void*)i,&pageTableManager);
    }
    uint64_t FrameBase = BootConfig->VideoConfig.FrameBufferBase;
    uint64_t FrameSize = BootConfig->VideoConfig.FrameBufferSize+4096;
    for(uint64_t i=FrameBase;i<FrameBase+FrameSize;i+=4096 ){
        MapMemory((void*)i,(void*)i,&pageTableManager);
    }
    asm ("mov %0,%%cr3"::"r"(PML4));
    printf("Im in new memmap\n");
    MapMemory((void*)0x600000000,(void*)0x80000,&pageTableManager);
    uint64_t*test=(uint64_t*)0x600000000;
    *test=26;
    printf("num=%d\n",*test);
    while(1){}
    return PassBack;
}

