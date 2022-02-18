#include <Kernel.h>
#include<BootConfig.h>
#include<PagInit.h>
#include<Assert.h>
#include<VideoInit.h>
#include<GDT.h>
#include<Interrupts.h>
FILENUM(1);
//extern PageFrameAllocator Allocator;
/*内核起始*/
extern uint64_t _KernelStart;
/*内核终止*/
extern uint64_t _KernelEnd;

//extern GDT DefaultGDT;


UINT64 KernelStart(BOOT_CONFIG *BootConfig)
{  
    
    UINT64 PassBack = 0;
    VideoInitial(BootConfig);
   
    CharInit();
    
    InitGDT();
    MemoryInit(&BootConfig->MemoryMap);
    //printf("%x\n",BootConfig->VideoConfig.FrameBufferBase);
    PagingInit(BootConfig);
    
    InitInterrupts();

     int* test = (int*)0x80000000000;
    *test = 2;
    
    printf("done\n");
    while(1){}
    return PassBack;
}

