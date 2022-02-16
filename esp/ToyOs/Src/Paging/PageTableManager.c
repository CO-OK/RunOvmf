#include<PageTableManager.h>
#include<PageMapIndexer.h>
#include<Memory32.h>
#include<PageFrameAllocator.h>

//extern PageFrameAllocator Allocator;

void MapMemory(void* virtualMemory, void* physicalMemory,PageTableManager*pageTableManager){
    /*PageMapIndexer*/
    PageMapIndexer pageMapIndexer;
    PageMapIndexerInit(&pageMapIndexer,(uint64_t)virtualMemory);

    /*先找到1级入口*/
    PageMapLevel4Entry PML4E;
    /*第一级页表中的页表项*/
    PML4E = pageTableManager->PML4[pageMapIndexer.PDP_i];

    PageDirPointerTablePageDirEntry* PDP;
    /*如果不存在*/
    if (!PML4E.present){
        PDP = (PageDirPointerTablePageDirEntry*)RequestPage();
        /*对这个Entry清0*/
        memset(PDP,0,4096);
        /*修改PML4E*/
        PML4E.present = true;
        //PML4E.accessed = true;
        PML4E.user_access =true;
        PML4E.writeable=true;
        PML4E.page_ppn = (uint64_t)PDP >> 12;
        pageTableManager->PML4[pageMapIndexer.PDP_i] = PML4E;
        // PDP = (PageTable*)GlobalAllocator.RequestPage();
        // memset(PDP, 0, 0x1000);
        // PML4E.Address = (uint64_t)PDP >> 12;
        // PML4E.Present = true;
        // PML4E.ReadWrite = true;
        // PML4->entries[indexer.PDP_i] = PML4E;
    }
    else{
        PDP = (PageDirPointerTablePageDirEntry*)((uint64_t)PML4E.page_ppn<<12);
    }


    PageDirPointerTablePageDirEntry PDPE;
    /*二级页表中的页表项*/
    PDPE = PDP[pageMapIndexer.PD_i];

    PageDirPageTableEntry* PDT;
    /*如果不存在*/
    if(!PDPE.present){
        PDT = (PageDirPageTableEntry*)RequestPage();
        memset(PDT,0,4096);
        //PDPE.accessed=true;
        PDPE.present = true;
        PDPE.user_access =true;
        PDPE.writeable=true;
        PDPE.page_ppn = (uint64_t)PDT>>12;
        PDP[pageMapIndexer.PD_i] = PDPE;
    }
    else{
        PDT = (PageDirPageTableEntry*)((uint64_t)PDPE.page_ppn<<12);
    }

    /*三级页表中的页表项*/
    PageDirPageTableEntry PDTE = PDT[pageMapIndexer.PT_i];

    PageTableEntry* PT;

    if(!PDTE.present){
        PT = (PageTableEntry*)RequestPage();
        memset(PT,0,4096);
        //PDTE.accessed=true;
        PDTE.present=true;
        PDTE.user_access=true;
        PDTE.writeable=true;
        PDTE.page_ppn=(uint64_t)PT>>12;
        PDT[pageMapIndexer.PT_i]=PDTE;
    }
    else{
        PT = (PageTableEntry*)((uint64_t)PDTE.page_ppn<<12);
    }

    /*四级页表中的页表项*/
    PageTableEntry PTE = PT[pageMapIndexer.P_i];
    PTE.present=true;
    PTE.user_access=true;
    PTE.writeable=true;
    //PTE.accessed=true;
    PTE.page_ppn=(uint64_t)physicalMemory>>12;
    PT[pageMapIndexer.P_i]=PTE;
}