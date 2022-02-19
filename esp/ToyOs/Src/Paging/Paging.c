#include<PagInit.h>
#include<PageFrameAllocator.h>
extern PageFrameAllocator Allocator;

void PagingInit(BOOT_CONFIG *BootConfig){

    PageMapLevel4Entry * PML4 = (PageMapLevel4Entry*)RequestPage();
    memset(PML4,0,4096);
    PageTableManager pageTableManager;
    pageTableManager.PML4=PML4;
    
    
    
    for(uint64_t i=0;i<GetTotallMemory();i+=4096){
        
        MapMemory((void*)i,(void*)i,&pageTableManager);
    }
    
    
    //LockPages((void*)FrameBase,FrameSize/4096 + 1);

    /*映射显存*/
    //printf("%x\n",BootConfig->VideoConfig.FrameBufferBase);
    uint64_t FrameBase = BootConfig->VideoConfig.FrameBufferBase;
    uint64_t FrameSize = BootConfig->VideoConfig.FrameBufferSize+4096;
    //Allocator.pageCtrlTable.LockPages((void*)FrameBase, FrameSize/ 0x1000 + 1);
    for(uint64_t i=FrameBase;i<FrameBase+FrameSize;i+=4096 ){
        MapMemory((void*)i,(void*)i,&pageTableManager);
    }
    asm ("mov %0,%%cr3"::"r"(PML4));
    
    /*解决一个彩虹bug 屏幕重新刷新*/
    memset((void*)FrameBase,0,FrameSize);
    ChangeCol(0);ChangeRow(0);

    BootConfig->pageTableManager=pageTableManager;
    BootConfig->pageFrameAllocator=&Allocator;
}