#include <Kernel.h>



UINT64 KernelStart(BOOT_CONFIG *BootConfig)
{  

    UINT64 PassBack = 0;
    VideoInitial(BootConfig);
    //ShellInitial();
    CharInit();
    for(int j=0;j<1000;j++){
        
            putchar(65);
        
        putchar('\n');
        
            putchar(48);
            //
        
        putchar('\n');
    }
    
    return PassBack;
}
