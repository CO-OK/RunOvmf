#include<Interrupts.h>
#include<IO.h>
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

/*设置中断门*/
void SetIDTGate(void* handlerFuncPointer,uint8_t entryOffset,uint8_t type_attr ,uint8_t slector){
    /*
        handlerFuncPointer 中断处理函数入口
        entryOffset        中断门描述符在IDT中的偏移
        type_attr          中断属性
        slector            中断所要用到的代码段选择子

        例子：
        
        InterruptDescriptor* int_PageFault = (InterruptDescriptor*)(idtr.Offset + 0xE * sizeof(InterruptDescriptor));//0xE是page fault的编号
    
        SetOffest((uint64_t)PageFault_Handler,int_PageFault);
        
        int_PageFault->type_attributes = IDT_TA_InterruptGate;
        
        int_PageFault->selector = 0x08;
    */

    /*得到们描述符基址*/
    InterruptDescriptor* descriptorEntry = (InterruptDescriptor*)(idtr.Offset + entryOffset * sizeof(InterruptDescriptor));
    /* 设置中断门描述符的函数入口*/
    SetOffest((uint64_t)handlerFuncPointer,descriptorEntry);
    /*设置 excepion 属性*/
    descriptorEntry->type_attributes = type_attr;
    /*中断要用到的代码段*/
    descriptorEntry->selector = slector;

}


void InitInterrupts(){
    idtr.Limit = 0x0FFF;
    idtr.Offset = (uint64_t)Allocator.pageCtrlTable.RequestPage();
    /*
        https://wiki.osdev.org/Exceptions
    */
    

    /*其他类似*/
    /*Page Fault*/
    SetIDTGate(PageFault_Handler,0xE,IDT_TA_InterruptGate,0x08);
    /*Double Fault*/
    SetIDTGate(DoubleFault_Handler,0x8,IDT_TA_InterruptGate,0x08);
    // InterruptDescriptor* int_DoubleFault = (InterruptDescriptor*)(idtr.Offset + 0x8 * sizeof(InterruptDescriptor));
    // SetOffest((uint64_t)DoubleFault_Handler,int_DoubleFault);
    // int_DoubleFault->type_attributes = IDT_TA_InterruptGate;
    // int_DoubleFault->selector = 0x08;

    /*General protection fault*/ 
    SetIDTGate(GeneralProtectionFault_Handler,0xD,IDT_TA_InterruptGate,0x08);
    // InterruptDescriptor* int_GeneralProtectionFault = (InterruptDescriptor*)(idtr.Offset + 0xD * sizeof(InterruptDescriptor));
    // SetOffest((uint64_t)GeneralProtectionFault_Handler,int_GeneralProtectionFault);
    // int_GeneralProtectionFault->type_attributes = IDT_TA_InterruptGate;
    // int_GeneralProtectionFault->selector = 0x08;

    /*KeyBoard*/
    SetIDTGate(KeyBoard_Handler,0x21,IDT_TA_InterruptGate,0x08);
    // InterruptDescriptor* int_KeyBoard = (InterruptDescriptor*)(idtr.Offset + 0x21 * sizeof(InterruptDescriptor));//0x20是PIC1
    // SetOffest((uint64_t)KeyBoard_Handler,int_KeyBoard);
    // int_KeyBoard->type_attributes = IDT_TA_InterruptGate;
    // int_KeyBoard->selector = 0x08;
    
    /*Mouse*/
    SetIDTGate(Mouse_Handler,0x2c,IDT_TA_InterruptGate,0x08);
    
    /*加载IDT*/
    asm ("lidt %0" : : "m" (idtr));

    ReMapPIC(0x20,0x28);

    /*umask 键盘*/
    /*
        The PIC has an internal register called the IMR, or the Interrupt Mask Register. 
        It is 8 bits wide. This register is a bitmap of the request lines going into the PIC.
        When a bit is set, the PIC ignores the request and continues normal operation
    */
    outb(PIC1_DATA,0b11111101);//https://wiki.osdev.org/Interrupts 为什么要umask 第二个
    outb(PIC2_DATA,0b11111111);
    /*enable maskble interrupts*/
    asm("sti");//IF置1
    InitPS2Mouse();
    outb(PIC1_DATA,0b11111001);//bit 2 置0容许2级PIC向Master发送中断
    outb(PIC2_DATA,0b11101111);//bit 4 让鼠标发送的中断不被屏蔽
}

/*重新初始化PIC （Programmable Interrupt Controller） 以免 外界中断与CPU内的中断的映射结果相同*/
void ReMapPIC(uint8_t offset1 , uint8_t offset2){
    /*
        arguments:
        offset1 - IDT中的偏移 master
        offset2 - IDT中的偏移 slave
    */
    unsigned char a1, a2;
 
	a1 = inb(PIC1_DATA);                        // save masks
    IO_Wait();
	a2 = inb(PIC2_DATA);
    IO_Wait();
	outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
	IO_Wait();
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	IO_Wait();
	outb(PIC1_DATA, offset1);                 // ICW2: Master PIC vector offset
	IO_Wait();
	outb(PIC2_DATA, offset2);                 // ICW2: Slave PIC vector offset
	IO_Wait();
	outb(PIC1_DATA, 4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	IO_Wait();
	outb(PIC2_DATA, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
	IO_Wait();
 
	outb(PIC1_DATA, ICW4_8086);
	IO_Wait();
	outb(PIC2_DATA, ICW4_8086);
	IO_Wait();
 
	outb(PIC1_DATA, a1);   // restore saved masks.
    IO_Wait();
	outb(PIC2_DATA, a2);
}

void PIC_EndMaster(){
    outb(PIC1_COMMAND,PIC_EOI);
}
void PIC_EndSlave(){
    outb(PIC2_COMMAND,PIC_EOI);
    outb(PIC1_COMMAND,PIC_EOI);//为什么加这个还不知道
}