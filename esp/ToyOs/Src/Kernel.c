#include <Kernel.h>
#include<PageFrameAllocator.h>
extern PageFrameAllocator Allocator;

UINT64 KernelStart(BOOT_CONFIG *BootConfig)
{  
    
    UINT64 PassBack = 0;
    VideoInitial(BootConfig);
    //PrintStr("VideoInit done...\n");
    //ShellInitial();
    CharInit();
    //PrintStr("CharInit done...\n");
    
    MemoryInit(&BootConfig->MemoryMap);
    
    // MemCtrlTable.SetMap(0,false);
    // MemCtrlTable.SetMap(1,true);
    // MemCtrlTable.SetMap(2,false);
    // for(int i=0;i<24;i++){
        
    //     printf("%d\n",MemCtrlTable.GetMapValue(i));
        
       
    // }
    for(int i=0;i<10;i++){
        printf("%x\n",(UINT64)Allocator.pageCtrlTable.RequestPage());
    }
    while(1){}
    return PassBack;
}

