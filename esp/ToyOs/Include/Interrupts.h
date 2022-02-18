#pragma once
#include<Uefi.h>
#include<Char.h>
#include<Bool.h>
#include<PageFrameAllocator.h>
// https://wiki.osdev.org/Interrupt 

/*InterruptDescriptor(Gate)的类型以及属性*/
#define IDT_TA_InterruptGate    0b10001110
#define IDT_TA_TrapGate         0b10001111
#define IDT_TA_CallGate         0b10001100
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
__attribute__((interrupt)) void PageFault_Handler(interrupt_frame* frame);