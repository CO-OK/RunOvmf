#include "Kernel.h"
#include "Video.h"

UINT64 KernelStart(BOOT_CONFIG *BootConfig)
{  
    UINT32 PassBack = 0;
    UINT32 DarkBlue = 0xFF18679A;
    UINT32* VideoHandle = (UINT32*)(BootConfig->FrameBufferBase);
    for(int i=0;i<900;i++){
        for(int j=0;j<1440;j++){
            DrawPixel(VideoHandle,DarkBlue,j,i);
        }
    }
    PassBack = BootConfig->FrameBufferBase;
    return PassBack;
}
