#include <Kernel.h>



UINT64 KernelStart(BOOT_CONFIG *BootConfig)
{  
    
    UINT64 PassBack = 0;
    VideoInitial(BootConfig);
    //PrintStr("VideoInit done...\n");
    //ShellInitial();
    CharInit();
    //PrintStr("CharInit done...\n");
    printf("num=%x\n",999);
    printf("address=%u\n",BootConfig->MemoryMap.MemoryMapBuffer);
    
    while(1){}
    return PassBack;
}

