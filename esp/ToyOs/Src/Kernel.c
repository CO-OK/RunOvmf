#include <Kernel.h>
extern MemoryControlTable MemCtrlTable;


UINT64 KernelStart(BOOT_CONFIG *BootConfig)
{  
    
    UINT64 PassBack = 0;
    VideoInitial(BootConfig);
    //PrintStr("VideoInit done...\n");
    //ShellInitial();
    CharInit();
    //PrintStr("CharInit done...\n");
    
    MemoryInit(&BootConfig->MemoryMap);
    
    MemCtrlTable.SetMap(0,false);
    MemCtrlTable.SetMap(1,true);
    MemCtrlTable.SetMap(2,false);
    for(int i=0;i<24;i++){
        
        printf("%d\n",MemCtrlTable.GetMapValue(i));
        
       
    }
    printf("done\n");
    while(1){}
    return PassBack;
}

