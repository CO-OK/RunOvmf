#include <Kernel.h>



UINT64 KernelStart(BOOT_CONFIG *BootConfig)
{  

    UINT64 PassBack = 0;
    VideoInitial(BootConfig);
    //ShellInitial();
    CharInit();
    PrintStr("CharInit done...\n");
    //MemoryInit();
    PrintDec((UINTN)BootConfig->MemoryMap.MemroyMapSize);
    PrintDec((UINTN)BootConfig->MemoryMap.DescriptorSize);
    PrintDec((UINTN)BootConfig->MemoryMap.MapKey);
    PrintDec((UINTN)BootConfig->MemoryMap.MemoryMapBuffer);
    while(1){}
    return PassBack;
}

