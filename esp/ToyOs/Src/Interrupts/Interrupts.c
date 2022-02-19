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

    /*其他类似*/
    /*Double Fault*/
    InterruptDescriptor* int_DoubleFault = (InterruptDescriptor*)(idtr.Offset + 0x8 * sizeof(InterruptDescriptor));
    SetOffest((uint64_t)DoubleFault_Handler,int_DoubleFault);
    int_DoubleFault->type_attributes = IDT_TA_InterruptGate;
    int_DoubleFault->selector = 0x08;

    /*General protection fault*/ 
    InterruptDescriptor* int_GeneralProtectionFault = (InterruptDescriptor*)(idtr.Offset + 0xD * sizeof(InterruptDescriptor));
    SetOffest((uint64_t)GeneralProtectionFault_Handler,int_GeneralProtectionFault);
    int_GeneralProtectionFault->type_attributes = IDT_TA_InterruptGate;
    int_GeneralProtectionFault->selector = 0x08;

    /*KeyBoard*/
    InterruptDescriptor* int_KeyBoard = (InterruptDescriptor*)(idtr.Offset + 0x21 * sizeof(InterruptDescriptor));//0x20是PIC1
    SetOffest((uint64_t)KeyBoard_Handler,int_KeyBoard);
    int_KeyBoard->type_attributes = IDT_TA_InterruptGate;
    int_KeyBoard->selector = 0x08;
    
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