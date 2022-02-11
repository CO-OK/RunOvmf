#include<Memory32.h>
#include<PageFrameAllocator.h>
#include<Assert.h>
FILENUM(2);
static UINT64 TotallMemorySize=0;



/*内存初始化*/
UINT32 MemoryInit(MemoryMapStatus* MapStatus){
    /*计算内存总量*/
    _GetTotallMemory(MapStatus->MemoryMapBuffer,MapStatus->MemroyMapSize/MapStatus->DescriptorSize);
    /*重整内存*/
    ResetMemory(MapStatus->MemoryMapBuffer,MapStatus->MemroyMapSize/MapStatus->DescriptorSize);
    /*初始化PageFrameAllocator*/
    ReadEFIMemoryMap(MapStatus->MemoryMapBuffer,MapStatus->MemroyMapSize/MapStatus->DescriptorSize,MapStatus->MemroyMapSize);
    //printf("Read efi memory map done\n");
    
    return 0;
}
UINT32 IsTogether(IN EFI_MEMORY_DESCRIPTOR*desc){
    EFI_MEMORY_DESCRIPTOR*temp=desc+1;
    if(desc->PhysicalStart+0x1000*desc->NumberOfPages==temp->PhysicalStart)
        return 1;
    return 0;
}
/*重新整理内存将可以合并的可用内存合并*/
UINT32 ResetMemory(IN EFI_MEMORY_DESCRIPTOR* desc,IN int num){
    /*
        BS_CODE
        BS_DATA
        Available
        这三个可以直接合并成Available
        其他的暂时不变
    */
   EFI_MEMORY_DESCRIPTOR* temp=NULL;
   for(int i=0;i<num;i++){
       if(desc->Type==EfiConventionalMemory||desc->Type==EfiBootServicesData||desc->Type==EfiBootServicesCode){
           /*首先改变类型为EfiBootServicesData*/
           desc->Type=EfiConventionalMemory;
           /*看看接下来有没有可以合并的*/
           temp = desc+1;
           while((temp->Type==EfiConventionalMemory||temp->Type==EfiBootServicesData||temp->Type==EfiBootServicesCode)&&IsTogether(desc)==1){
               /*合并*/
               /*Pad正好设为1表示这个段已经被合并了*/
               temp->Pad=1;
               desc->NumberOfPages+=temp->NumberOfPages;
               temp++;
               i++;
           }
           desc=temp;
           
       }
       else{
           desc++;
           
       }
   }

}
/*计算内存总量*/
UINTN _GetTotallMemory(IN EFI_MEMORY_DESCRIPTOR* desc,IN int num){
    
    if(TotallMemorySize>0)return TotallMemorySize;
    for(int i=0;i<num;i++){
        
        TotallMemorySize+=desc->NumberOfPages*0x1000;
        desc++;
    }
    return TotallMemorySize;
}
UINTN GetTotallMemory(){
    return TotallMemorySize;
}
/*打印内存信息*/
void PrintMemoryMap(EFI_MEMORY_DESCRIPTOR* desc,int num){
    /*
        desc：EFI_MEMORY_DESCRIPTOR*数组始址
        num:数组长度
    */
   UINT64 TotalPages=0;
   for(int i=0;i<num;i++){
       EFI_MEMORY_DESCRIPTOR*temp=(EFI_MEMORY_DESCRIPTOR*)((UINT64)desc+i*48);
       if(desc->Pad==0){
            printf("Type:%x   ",temp->Type);
            printf("Start:%x   ",temp->PhysicalStart);
            printf("Num pages:%x\n",temp->NumberOfPages);
            TotalPages+=temp->NumberOfPages;
       }
       
       //desc++;
   }
   printf("TotalPages=%d\n",TotalPages);

}


