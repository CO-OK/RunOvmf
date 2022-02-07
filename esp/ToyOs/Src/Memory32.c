#include<Memory32.h>
/*内存初始化*/
UINT32 MemoryInit(MemoryMapStatus* MapStatus){
    /*首先打印内存信息*/
    PrintMemoryMap(MapStatus->MemoryMapBuffer,MapStatus->MemroyMapSize/MapStatus->DescriptorSize);
    return 0;
}
/*打印内存信息*/
void PrintMemoryMap(EFI_MEMORY_DESCRIPTOR* desc,int num){
    /*
        desc：EFI_MEMORY_DESCRIPTOR*数组始址
        num:数组长度
    */
   for(int i=0;i<num;i++){
       printf("Type:%x   ",desc->Type);
       printf("Start:%x   ",desc->PhysicalStart);
       printf("Num pages:%x\n",desc->NumberOfPages);
       desc++;
   }

}
/*启动32位分页*/
UINT32 EnablePaging(){
    return 0;
}