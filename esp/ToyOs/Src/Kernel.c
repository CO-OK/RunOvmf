#include <Kernel.h>



UINT64 KernelStart(BOOT_CONFIG *BootConfig)
{  

    UINT64 PassBack = 0;
    VideoInitial(BootConfig);
    //ShellInitial();
    CharInit();
    
    for(int i=0;i<100;i++){
        putchar(BootConfig->VideoConfig.FrameBufferBase,i,i,0,White,Black);
    }
    
	//putchar(BootConfig->VideoConfig.FrameBufferBase,35,1,0,White,Black);
    //putchar(BootConfig->VideoConfig.FrameBufferBase,36,2,0,White,Black);
	
    
    
    return PassBack;
}
