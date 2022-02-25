#include <Kernel.h>



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
    KernelInit(BootConfig);
    
    
    
    
    
    
    
    while(1){}
    return PassBack;
}


void KernelInit(BOOT_CONFIG* BootConfig){
    CharInit();
    VideoInitial(BootConfig);
   
    
    
    InitGDT();
    MemoryInit(&BootConfig->MemoryMap);
    //printf("%x\n",BootConfig->VideoConfig.FrameBufferBase);
    PagingInit(BootConfig);
    
    InitInterrupts();
    InitACPI(BootConfig);
    printf("Kernel\n");
}

void InitACPI(BOOT_CONFIG *BootConfig){
    //"XSDT" Extended System Description Table
    //https://wiki.osdev.org/XSDT
    SDTHeader* xsdt = (SDTHeader*)(((RSDP*)BootConfig->rsdp)->XsdtAddress);
    
    int entries = (xsdt->Length-sizeof(SDTHeader))/8;
   
    // for(int i=0;i<entries;i++){
    //     SDTHeader* newSDTHeader = (SDTHeader*)(*((uint64_t*)((uint64_t)xsdt+sizeof(SDTHeader)+i*8)));
    //     for(int j=0;j<4;j++){
    //         // printf("7");
    //         printf("%c",*(uint8_t*)((uint64_t)newSDTHeader+j));
           
    //     }
    //     printf("%x\n",newSDTHeader->Signature);
    //     //printf("\n");
    // }
    MCFGHeader*mcfg=(MCFGHeader*)GetAcpiTable(xsdt,MCFG_SIGNATURE);
    
    EnumeratePCI(mcfg);
    
}