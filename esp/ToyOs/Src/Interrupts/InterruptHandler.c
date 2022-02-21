#include<Interrupts.h>
#include<Panic.h>
#include<IO.h>
__attribute__((interrupt)) void PageFault_Handler(interrupt_frame* frame){
    Panic("Page fault detected\n");
    while(true);
}

__attribute__((interrupt)) void DoubleFault_Handler(interrupt_frame* frame){
    Panic("Double fault detected\n");
    
    while(true);
}

__attribute__((interrupt)) void GeneralProtectionFault_Handler(interrupt_frame* frame){
    Panic("General proection fault detected\n");
    
    while(true);
}

__attribute__((interrupt)) void Mouse_Handler(interrupt_frame* frame){
    
    uint8_t scancode=inb(0x60);//ps2 controller data port
    PIC_EndSlave();
    printf("m");
    //while(true);
}


__attribute__((interrupt)) void KeyBoard_Handler(interrupt_frame* frame){
    
    /*
        因为键盘和ps2 有关 参见 https://wiki.osdev.org/%228042%22_PS/2_Controller 
    */
   /*读取扫描码*/
    uint8_t scancode=inb(0x60);//ps2 controller data port
    HandleKeyboard(scancode);
    PIC_EndMaster();
}