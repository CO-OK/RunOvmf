#pragma once
#include<Uefi.h>

//https://wiki.osdev.org/Global_Descriptor_Table

/*GDTDescriptor 即 GDTR*/
typedef struct __attribute__((packed)){
    /*The size of the table in bytes subtracted by 1*/
    uint16_t Size;
    /* The linear address of the GDT (not the physical address, paging applies). */
    uint64_t Offset;

}GDTDescriptor ;
typedef GDTDescriptor GDTR;

/*Segment Descriptor 即 GDTEntry*/
typedef struct __attribute__((packed)){
    /*
        Base: A 32-bit value containing the linear address where the segment begins. 
        Limit: A 20-bit value, tells the maximum addressable unit, either in 1 byte units, or in 4KiB pages. 
        In 64-bit mode, the Base and Limit values are ignored, each descriptor covers the entire linear address space regardless of what they are set to. 
        更多信息 上面链接
    */
    uint16_t Limit0;
    uint16_t Base0;
    uint8_t Base1;
    uint8_t AccessByte;
    uint8_t limit1AndFlags;
    uint8_t Base2;
}SegmentDescriptor ;
typedef SegmentDescriptor GDTEntry;

typedef struct __attribute__((packed)) __attribute__((aligned(0x1000))){
    /*第一个永远是null*/
    GDTEntry Null;
    /*内核代码*/
    GDTEntry KernelCode;
    /*内核数据*/
    GDTEntry KernelData;
    /*下面的一些暂时不会用到*/
    GDTEntry UesrNull;
    GDTEntry UserCode;
    GDTEntry UserData;

}GDT ;

/*在GDT.c中声明*/
extern GDT DefaultGDT;
/*in gdt.asm*/
extern void LoadGDT(GDTR* GDTR);