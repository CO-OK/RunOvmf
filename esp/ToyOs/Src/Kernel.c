#include <Kernel.h>
#include<BootConfig.h>
#include<PagInit.h>
#include<Assert.h>
#include<VideoInit.h>

FILENUM(1);
//extern PageFrameAllocator Allocator;
/*内核起始*/
extern uint64_t _KernelStart;
/*内核终止*/
extern uint64_t _KernelEnd;



UINT64 KernelStart(BOOT_CONFIG *BootConfig)
{  
    
    UINT64 PassBack = 0;
    VideoInitial(BootConfig);
   
    CharInit();
  
    MemoryInit(&BootConfig->MemoryMap);
    //printf("%x\n",BootConfig->VideoConfig.FrameBufferBase);
    PagingInit(BootConfig);
    
    printf("done\n");
    while(1){}
    return PassBack;
}

