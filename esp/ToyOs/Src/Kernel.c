#include <Kernel.h>



UINT64 KernelStart(BOOT_CONFIG *BootConfig)
{  
    
    UINT64 PassBack = 0;
    VideoInitial(BootConfig);
    //PrintStr("VideoInit done...\n");
    //ShellInitial();
    CharInit();
    //PrintStr("CharInit done...\n");
    MemoryInit(&BootConfig->MemoryMap);
    
    while(1){}
    return PassBack;
}

