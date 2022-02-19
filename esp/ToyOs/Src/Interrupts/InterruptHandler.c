#include<Interrupts.h>
#include<Panic.h>

__attribute__((interrupt)) void PageFault_Handler(interrupt_frame* frame){
    Panic("Page fault detected\n");
    
    while(true);
}