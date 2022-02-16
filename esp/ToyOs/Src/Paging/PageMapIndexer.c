#include<PageMapIndexer.h>
void PageMapIndexerInit(PageMapIndexer*pageMapIndexer,uint64_t virtualAddress){
    virtualAddress >>= 12;
    pageMapIndexer->P_i = virtualAddress & 0x1ff;
    virtualAddress >>= 9;
    pageMapIndexer->PT_i = virtualAddress & 0x1ff;
    virtualAddress >>= 9;
    pageMapIndexer->PD_i = virtualAddress & 0x1ff;
    virtualAddress >>= 9;
    pageMapIndexer->PDP_i = virtualAddress & 0x1ff;
}