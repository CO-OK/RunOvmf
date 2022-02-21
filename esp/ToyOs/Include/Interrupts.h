#pragma once
#include<Uefi.h>
#include<Char.h>
#include<Bool.h>
#include<PageFrameAllocator.h>
#include<KeyBoard.h>
#include<Mouse.h>
// https://wiki.osdev.org/Interrupt 

/*InterruptDescriptor(Gate)的类型以及属性*/
#define IDT_TA_InterruptGate    0b10001110
#define IDT_TA_TrapGate         0b10001111
#define IDT_TA_CallGate         0b10001100

#define PIC1		0x20		/* IO base address for master PIC */
#define PIC2		0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)
#define PIC_EOI     0x20        /*End of Interrupt*/

/* reinitialize the PIC controllers, giving them specified vector offsets
   rather than 8h and 70h, as configured by default */
 
#define ICW1_ICW4	0x01		/* ICW4 (not) needed */
#define ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	0x10		/* Initialization - required! */
 
#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	0x10		/* Special fully nested (not) */

/*重新初始化PIC （Programmable Interrupt Controller） 以免 外界中断与CPU内的中断的映射结果相同*/
void ReMapPIC(uint8_t offset1 , uint8_t offset2);
/*结束PIC发起的中断*/
void PIC_EndMaster();
void PIC_EndSlave();

typedef struct{
    /*
        https://wiki.osdev.org/Interrupt_Descriptor_Table#Structure_on_x86-64
    */
    uint16_t offset_0;        // offset bits 0..15
    uint16_t selector;        // a code segment selector in GDT or LDT
    uint8_t  ist;             // bits 0..2 holds Interrupt Stack Table offset, rest of bits zero.
    uint8_t  type_attributes; // gate type, dpl, and p fields
    uint16_t offset_1;        // offset bits 16..31
    uint32_t offset_2;        // offset bits 32..63
    uint32_t zero;            // reserved
    
}InterruptDescriptor;

typedef struct __attribute__((packed)){
    uint16_t Limit;
    uint64_t Offset;
}IDTR;

/*InterruptDescriptor offset 操作*/
void SetOffest(uint64_t offset,InterruptDescriptor* desc);
uint64_t GetOffset(InterruptDescriptor* desc);

/*初始化IDT*/

void InitInterrupts();

/*Interrupts Handler*/
/*https://wiki.osdev.org/Interrupt_Service_Routines*/
/*这里没有使用汇编来写 稍微简单些*/
typedef struct __attribute__((packed)){
    uint64_t ip;
    uint64_t cs;
    uint64_t flags;
    uint64_t sp;
    uint16_t ss;
}interrupt_frame;

/*设置中断门*/
void SetIDTGate(void* handlerFuncPointer,uint8_t entryOffset,uint8_t type_attr ,uint8_t slector);
/*https://wiki.osdev.org/Exceptions*/
__attribute__((interrupt)) void PageFault_Handler(interrupt_frame* frame);
__attribute__((interrupt)) void DoubleFault_Handler(interrupt_frame* frame);
__attribute__((interrupt)) void GeneralProtectionFault_Handler(interrupt_frame* frame);
__attribute__((interrupt)) void KeyBoard_Handler(interrupt_frame* frame);
__attribute__((interrupt)) void Mouse_Handler(interrupt_frame* frame);