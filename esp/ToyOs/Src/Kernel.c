#include <Kernel.h>
#include<BootConfig.h>
#include<PagInit.h>
#include<Assert.h>
#include<VideoInit.h>
#include<GDT.h>
#include<Interrupts.h>
#include<Mouse.h>
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
    CharInit();
    VideoInitial(BootConfig);
   
    
    
    InitGDT();
    MemoryInit(&BootConfig->MemoryMap);
    //printf("%x\n",BootConfig->VideoConfig.FrameBufferBase);
    PagingInit(BootConfig);
    
    InitInterrupts();
    
    outb(PIC1_DATA,0b11111001);//bit 2 置0容许2级PIC向Master发送中断
    outb(PIC2_DATA,0b11101111);//bit 4 让鼠标发送的中断不被屏蔽
    InitPS2Mouse();
    
    
    printf("done\n");
    while(1){}
    return PassBack;
}

