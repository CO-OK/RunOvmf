#include<Memory32.h>
static UINT64 TotallMemorySize=0;
/*内存操作表*/
MemoryControlTable MemCtrlTable;
/*BitMap*/

BitMap MemBitMap;
bool map[20]={1,1,1};

/*内存初始化*/
UINT32 MemoryInit(MemoryMapStatus* MapStatus){
    /*首先打印内存信息*/
    //PrintMemoryMap(MapStatus->MemoryMapBuffer,MapStatus->MemroyMapSize/MapStatus->DescriptorSize);
    /*重整内存*/
    ResetMemory(MapStatus->MemoryMapBuffer,MapStatus->MemroyMapSize/MapStatus->DescriptorSize);
    //PrintMemoryMap(MapStatus->MemoryMapBuffer,MapStatus->MemroyMapSize/MapStatus->DescriptorSize);
    /*计算内存总量*/
    _GetTotallMemory(MapStatus->MemoryMapBuffer,MapStatus->MemroyMapSize/MapStatus->DescriptorSize);
    MemBitMap.MapBase=map;
   
    /*最后初始化一个MemoryControlTable*/
    MemCtrlTable.GetMapValue=GetBitMapValue;
    MemCtrlTable.SetMap=SetBitMap;
    MemCtrlTable.GetTotalMemory=GetTotallMemory;
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
   for(int i=0;i<num;){
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
           continue;
       }
       else{
           desc++;
           i++;
       }
   }

}
/*计算内存总量*/
UINTN _GetTotallMemory(IN EFI_MEMORY_DESCRIPTOR* desc,IN int num){
    
    if(TotallMemorySize>0)return TotallMemorySize;
    for(int i=0;i<num;i++){
        TotallMemorySize+=desc->NumberOfPages*0x1000;
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
       if(desc->Pad==0){
            //printf("Type:%x   ",desc->Type);
            //printf("Start:%x   ",desc->PhysicalStart);
            //printf("Num pages:%x\n",desc->NumberOfPages);
            TotalPages+=desc->NumberOfPages;
       }
       
       desc++;
   }
   printf("TotalPages=%d\n",TotalPages);

}


bool _GetBitMapValue(IN UINTN index,IN bool* MapBase){
    /*所在字节与所在字节中的所在位*/
    UINTN ByteIndex=index/8;
    UINT8 BitIndex=index%8;
    /**/
    UINT8 mask=0b10000000>>BitIndex;
    if((MapBase[ByteIndex]&mask)>0){
        return true;
    }
    return false;
}
bool GetBitMapValue(IN UINTN index){
    return _GetBitMapValue(index,MemBitMap.MapBase);
}

void _SetBitMap(IN bool value,IN UINTN index,IN bool* MapBase){
    /*所在字节与所在字节中的所在位*/
    UINTN ByteIndex=index/8;
    UINT8 BitIndex=index%8;
    UINT8 mask=0b10000000>>BitIndex;
    if(value>0){
        MapBase[ByteIndex] |= mask;
        return;
    }
    MapBase[ByteIndex] &= ~mask;
}
void SetBitMap(IN bool index,IN UINTN value){
    _SetBitMap(value,index,MemBitMap.MapBase);
}