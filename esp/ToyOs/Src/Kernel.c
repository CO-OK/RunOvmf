#include <Kernel.h>



UINT64 KernelStart(BOOT_CONFIG *BootConfig)
{  

    UINT64 PassBack = 0;
    VideoInitial(BootConfig);
    ShellInitial();
    POINT p1,p2;
    p1.X=25;
    p1.Y=25;
    p2.X=100;
    p2.Y=30;
    BLOCK block;
    block.Start=p1;
    block.End=p2;
    block.Color=0xFF18679A;
    DrawBlock(block);
    
	
	
    
    
    return PassBack;
}
