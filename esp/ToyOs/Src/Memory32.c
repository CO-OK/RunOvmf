#include<Memory32.h>
/*内存初始化*/
UINT32 MemoryInit(MemoryMapStatus* MapStatus){
    /*首先打印内存信息*/
    //PrintMemoryMap(MapStatus->MemoryMapBuffer,MapStatus->MemroyMapSize/MapStatus->DescriptorSize);
    /*重整内存*/
    ResetMemory(MapStatus->MemoryMapBuffer,MapStatus->MemroyMapSize/MapStatus->DescriptorSize);
    //PrintMemoryMap(MapStatus->MemoryMapBuffer,MapStatus->MemroyMapSize/MapStatus->DescriptorSize);
    return 0;
}
UINT32 IsTogether(IN EFI_MEMORY_DESCRIPTOR*desc){
    EFI_MEMORY_DESCRIPTOR*temp=desc+1;
    if(desc->PhysicalStart+0xFFF*desc->NumberOfPages+1==temp->PhysicalStart)
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
           //printf("666\n");
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
           //printf("444\n");
           desc++;
           i++;
       }
   }

}

/*打印内存信息*/
void PrintMemoryMap(EFI_MEMORY_DESCRIPTOR* desc,int num){
    /*
        desc：EFI_MEMORY_DESCRIPTOR*数组始址
        num:数组长度
    */
   int a=0;
   for(int i=0;i<30;i++){
       if(desc->Pad==0){
            printf("Type:%x   ",desc->Type);
            printf("Start:%x   ",desc->PhysicalStart);
            printf("Num pages:%x\n",desc->NumberOfPages);
            a++;
       }
       
       desc++;
   }
   //printf("Number of desc=%u\n",a);

}
/*启动32位分页*/
UINT32 EnablePaging(){
    return 0;
}