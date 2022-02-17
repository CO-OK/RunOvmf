#include<PageFrameAllocator.h>
#include<Bool.h>
#include<Uefi.h>
#include<Assert.h>

FILENUM(3);

/*总可用内存*/
static uint64_t FreeMemory;
/*系统保留内存*/
static uint64_t ReservedMemory;
/*已使用内存*/
static uint64_t UsedMemory;
/*PageFrameAllocator是否已經被初始化*/
bool IsInitialized = false;
/*page frame allocator*/
PageFrameAllocator Allocator;

/**/
uint64_t pageBitMapIndex=0;

void ReadEFIMemoryMap(IN EFI_MEMORY_DESCRIPTOR* desc,IN int num,IN UINTN DescriptorSize){
    if(IsInitialized) return;
    /*找到最大的一块用来存放bitmap*/
    /*最大的内存起始*/
    void* LargestFreeMemoryBase=NULL;
    /*最大内存大小*/
    UINT64 LargestFreeMemorySize=0;
    EFI_MEMORY_DESCRIPTOR*temp=desc;
    for(int i=0;i<num;i++){
        //printf("pages=%d\n",temp->NumberOfPages);
        if(temp->Type==EfiConventionalMemory&&temp->Pad!=1){
            FreeMemory+=temp->NumberOfPages*4096;
            if(temp->NumberOfPages*0x1000>LargestFreeMemorySize){
                LargestFreeMemoryBase=(void*)temp->PhysicalStart;
                LargestFreeMemorySize=temp->NumberOfPages*0x1000;
            }
            
        }
        temp++;
    }
    /*得到总内存大小*/
    UINT64 TotalMemory=GetTotallMemory();

    /*计算BitMap需要的大小，以字节为单位*/
    UINT64 BitMapSize=(TotalMemory/4096)/8 +1;
    /*初始化BitMap*/
    InitBitMap(BitMapSize,LargestFreeMemoryBase);
    //printf("bit map size=%x\n",BitMapSize);
    //printf("Init bitmap done\n");
    /*初始化PageControlTable*/
    Allocator.pageCtrlTable.FreePage=FreePage;
    Allocator.pageCtrlTable.FreePages=FreePages;
    Allocator.pageCtrlTable.LockPage=LockPage;
    Allocator.pageCtrlTable.LockPages=LockPages;
    Allocator.pageCtrlTable.ReservePage=ReservePage;
    Allocator.pageCtrlTable.ReservePages=ReservePages;
    Allocator.pageCtrlTable.UnReservePage=UnReservePage;
    Allocator.pageCtrlTable.UnReservePages=UnReservePages;
    Allocator.pageCtrlTable.GetFreeMemory=GetFreeMemory;
    Allocator.pageCtrlTable.GetReservedMemory=GetReservedMemory;
    Allocator.pageCtrlTable.GetUsedMemory=GetUsedMemory;
    Allocator.pageCtrlTable.RequestPage=RequestPage;
    /*锁定BitMap所在页*/
    Allocator.pageCtrlTable.LockPages(Allocator.PageBitMap.MapBase,Allocator.PageBitMap.size/4096+1);
    //printf("lock pages done\n");
    /*reserve不是EfiConventionalMemory的neicun*/
    temp=desc;
    for(int i=0;i<num;i++){
        if(temp->Type!=EfiConventionalMemory&&temp->Pad!=1){
            /*reserve pages*/
            Allocator.pageCtrlTable.ReservePages((void*)temp->PhysicalStart,temp->NumberOfPages);
        }
        temp++;
    }
    //printf("Reserve pages done\n");
}


/*初始化BitMap*/
static void InitBitMap(UINT64 bitMapSize,void*bitMapBase){
    Allocator.PageBitMap.size=bitMapSize;
    Allocator.PageBitMap.MapBase=(UINT8*)bitMapBase;
    /*设置BitMap为0*/
    for(int i=0;i<bitMapSize;i++){
        *(UINT8*)(Allocator.PageBitMap.MapBase+i)=0;
    }
    Allocator.bitMapCtrlTable.GetMapValue=GetBitMapValue;
    Allocator.bitMapCtrlTable.SetMap=SetBitMap;
}

void FreePage(void*pageAddress){
    /*计算index*/
    UINT64 index = ((UINTN)pageAddress)/4096;
    if(Allocator.bitMapCtrlTable.GetMapValue(index)==false)return;
    if(Allocator.bitMapCtrlTable.SetMap(index,false)){
        FreeMemory+=4096;
        UsedMemory-=4096;
        if(pageBitMapIndex>index)pageBitMapIndex=index;
    }
    else{
        
        ASSERT(7==8);
    }
    
}

void FreePages(void*pageAddress,UINT64 pageCount){
    for(int i=0;i<pageCount;i++){
        FreePage((void*)((UINT64)(pageAddress)+(i*4096)));
    }
}

void LockPage(void*pageAddress){
    /*计算index*/
    UINT64 index = ((UINTN)pageAddress)/4096;
    if(Allocator.bitMapCtrlTable.GetMapValue(index)==true)return;
    if(Allocator.bitMapCtrlTable.SetMap(index,true)){
        FreeMemory-=4096;
        UsedMemory+=4096;
    }
    else{
        printf("index=%x\n",index);
        ASSERT(7==8);
        
    }
    
}

void LockPages(void*pageAddress,UINT64 pageCount){
    for(int i=0;i<pageCount;i++){
        LockPage((void*)((UINT64)(pageAddress)+(i*4096)));
    }
}
void UnReservePage(void*pageAddress){
    /*计算index*/
    UINT64 index = ((UINTN)pageAddress)/4096;
    if(Allocator.bitMapCtrlTable.GetMapValue(index)==false)return;
    if(Allocator.bitMapCtrlTable.SetMap(index,false)){
        FreeMemory+=4096;
        ReservedMemory-=4096;
        if(pageBitMapIndex>index)pageBitMapIndex=index;
    }
    else{
        ASSERT(7==8);
    }
    
}

void UnReservePages(void*pageAddress,UINT64 pageCount){
    for(int i=0;i<pageCount;i++){
        UnReservePage((void*)((UINT64)(pageAddress)+(i*4096)));
    }
}

void ReservePage(void*pageAddress){
    /*计算index*/
    UINT64 index = ((UINTN)pageAddress)/4096;
    if(Allocator.bitMapCtrlTable.GetMapValue(index)==true)return;
    if(Allocator.bitMapCtrlTable.SetMap(index,true)){
        FreeMemory-=4096;
        ReservedMemory+=4096;
    }
    else{
        //ASSERT(7==8);
    }
    
}

void ReservePages(void*pageAddress,UINT64 pageCount){
    //printf("pagecount=%d\n",pageCount);
    for(int i=0;i<pageCount;i++){

        ReservePage((void*)((UINT64)(pageAddress)+(i*4096)));
    }
}

uint64_t GetFreeMemory(){
    return FreeMemory;
}
uint64_t GetUsedMemory(){
    return UsedMemory;
}
uint64_t GetReservedMemory(){
    return ReservedMemory;
}

void* RequestPage(){
    for(;pageBitMapIndex<Allocator.PageBitMap.size*8;pageBitMapIndex++){
        /*没找到空闲页就继续*/
        if(Allocator.bitMapCtrlTable.GetMapValue(pageBitMapIndex)==true)continue;
        /*找到则分配*/
        Allocator.bitMapCtrlTable.SetMap(pageBitMapIndex,false);
        Allocator.pageCtrlTable.LockPage((void*)(pageBitMapIndex*4096));
        return (void*)(pageBitMapIndex*4096);
    }
    /*如果都没找到则需要换页之类的，现在先不实现*/
    return NULL;
}


bool _GetBitMapValue(IN UINTN index,IN bool* MapBase){
    if(index > Allocator.PageBitMap.size*8) return false;
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
    return _GetBitMapValue(index,Allocator.PageBitMap.MapBase);
}

bool _SetBitMap(IN bool value,IN UINTN index,IN bool* MapBase){
    if(index > Allocator.PageBitMap.size*8) return false;
    /*所在字节与所在字节中的所在位*/
    UINTN ByteIndex=index/8;
    UINT8 BitIndex=index%8;
    UINT8 mask=0b10000000>>BitIndex;
    if(value>0){
        MapBase[ByteIndex] |= mask;
        return true;
    }
    MapBase[ByteIndex] &= ~mask;
    return true;
}
bool SetBitMap(IN UINTN index,IN bool value){
    //printf("index=%x\n",index);
    return _SetBitMap(value,index,Allocator.PageBitMap.MapBase);
}