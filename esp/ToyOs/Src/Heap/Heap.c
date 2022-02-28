#include<Heap.h>

/*全局堆的首尾地址*/
void* GlobalHeapStart;
void* GlobalHeapEnd;

HeapSegHeader* LastHdr;

void InitHeap(void* heapAddress, uint64_t pageCount){
    /*
        初始化为一个堆
        之后分配的时候从这个堆里切分
    */
    void* pos=heapAddress;
    for(uint64_t i=0;i<pageCount;i++){
        MapMemory(pos,Allocator.pageCtrlTable.RequestPage(),&GlobalPageTableManager);
        pos=(void*)((uint64_t)pos+0x1000);
        
    }
    /*以字节为单位*/
    uint64_t heapLength=pageCount*0x1000;

    GlobalHeapStart=heapAddress;
    GlobalHeapEnd=(void*)((uint64_t)heapAddress+heapLength);
    HeapSegHeader* StartSeg=(HeapSegHeader*)GlobalHeapStart;
    StartSeg->length=heapLength-sizeof(HeapSegHeader);
    StartSeg->next=NULL;
    StartSeg->last=NULL;
    StartSeg->free=true;
    LastHdr=StartSeg;
}

void* malloc(uint64_t size){
    if(size==0)return NULL;
    if((size%0x10)!=0){
        //确保每个分配的内存都是0x10的倍数
        size-=(size%0x10);
        size+=0x10;
    }
    HeapSegHeader*current=GlobalHeapStart;
    while(true){
        if(current->free){
            if(current->length>size){
                //split
                current=SplitHeap(current,size);
                current->free=false;
                return (void*)((uint64_t)current+sizeof(HeapSegHeader));
            }
            if(current->length==size){
                current->free=false;
                return (void*)((uint64_t)current+sizeof(HeapSegHeader));
            }
        }
        if(current->next==NULL)break;
        current=current->next;
    }
    //跳出循环则说明没有足够的堆
    //expand heap
    //printf("expand\n");
    ExpandHeap(size);
    return malloc(size);
}

HeapSegHeader* SplitHeap(HeapSegHeader*hdr,uint64_t size){
    if(size<0x10)return NULL;
    //剩余内存大小
    int remain=hdr->length-size-sizeof(HeapSegHeader);
    if(remain<0x10)return NULL;

    HeapSegHeader* new=(HeapSegHeader*)((uint64_t)hdr+remain+sizeof(HeapSegHeader));

    if(hdr->next!=NULL){
        hdr->next->last=new;
        new->next=hdr->next;
    }
    else{
        new->next=NULL;
    }
    hdr->next=new;
    new->last=hdr;
    new->free=true;
    new->length=size;
    hdr->length=remain;
    if (LastHdr == hdr) LastHdr = new;

    return new;
}

void ExpandHeap(uint64_t length){
    //在最后一个块上扩充
    /*先把length对齐为页*/
    if((length%0x1000)!=0){
        length-=(length%0x1000);
        length+=0x1000;
    }
    uint64_t pageCount=length/0x1000;
    HeapSegHeader*new=GlobalHeapEnd;
    for(uint64_t i=0;i<pageCount;i++){
        MapMemory(GlobalHeapEnd,Allocator.pageCtrlTable.RequestPage(),&GlobalPageTableManager);
        GlobalHeapEnd=(void*)((uint64_t)GlobalHeapEnd+0x1000);
    }
    new->free=true;
    new->last=LastHdr;
    LastHdr->next=new;
    LastHdr=new;
    new->next=NULL;
    new->length=length-sizeof(HeapSegHeader);
    /*检查一下是否可以合并*/

    CombineBackward(new);
}

void CombineForward(HeapSegHeader*hdr){
    if(hdr->next==NULL)return;
    if(!hdr->next->free)return;
    HeapSegHeader*temp=hdr->next;
    /*开始合并*/
    if(hdr->next==LastHdr)LastHdr=hdr;
    if(hdr->next->next!=NULL){
        temp->next->last=hdr;
        hdr->next=temp->next;
    }
    else{
        hdr->next=NULL;
    }
    hdr->length=hdr->length+temp->length+sizeof(HeapSegHeader);
}
void CombineBackward(HeapSegHeader*hdr){
    if(hdr->last!=NULL&&hdr->last->free==true)
        CombineForward(hdr->last);
}

void free(void* address){
    HeapSegHeader* hdr=((HeapSegHeader*)address)-1;
    hdr->free=true;
    CombineForward(address);
    CombineBackward(address);
}