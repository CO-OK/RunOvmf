#include<Interrupts.h>
extern PageFrameAllocator Allocator;
void SetOffest(uint64_t offset,InterruptDescriptor* desc){
    desc->offset_0 = (uint16_t)(offset & 0x000000000000ffff);
    desc->offset_1 = (uint16_t)((offset & 0x00000000ffff0000)>>16);
    desc->offset_2 = (uint32_t)((offset & 0xffffffff00000000)>>32);
}

uint64_t GetOffset(InterruptDescriptor* desc){
    uint64_t offset=0;
    offset |= (uint64_t)desc->offset_0;
    offset |= (uint64_t)desc->offset_1<<16;
    offset |= (uint64_t)desc->offset_2<<32;
    return offset;
}

IDTR idtr;
void InitInterrupts(){
    idtr.Limit = 0x0FFF;
    idtr.Offset = (uint64_t)Allocator.pageCtrlTable.RequestPage();
    /*
        https://wiki.osdev.org/Exceptions
    */
    /*page fault exception 入口*/
    InterruptDescriptor* int_PageFault = (InterruptDescriptor*)(idtr.Offset + 0xE * sizeof(InterruptDescriptor));//0xE是page fault的编号
    /*page fault 函数入口*/
    SetOffest((uint64_t)PageFault_Handler,int_PageFault);
    /*设置 excepion 属性*/
    int_PageFault->type_attributes = IDT_TA_InterruptGate;
    /*Kernel Code segment*/
    int_PageFault->selector = 0x08;

    /*加载IDT*/
    asm ("lidt %0" : : "m" (idtr));
}