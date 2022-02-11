#include <Kernel.h>
#include<PageFrameAllocator.h>
#include<Assert.h>
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
    ASSERT(7==8);
    MemoryInit(&BootConfig->MemoryMap);
    
    // MemCtrlTable.SetMap(0,false);
    // MemCtrlTable.SetMap(1,true);
    // MemCtrlTable.SetMap(2,false);
    // for(int i=0;i<24;i++){
        
    //     printf("%d\n",MemCtrlTable.GetMapValue(i));
        
       
    // }
    printf("KernelStart=%x\nKernelEnd=%x\n",&_KernelStart,&_KernelEnd);
    printf("KernelSize=%x\n",&_KernelEnd-&_KernelStart);
    while(1){}
    return PassBack;
}

