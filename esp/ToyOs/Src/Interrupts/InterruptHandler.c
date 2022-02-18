#include<Interrupts.h>


__attribute__((interrupt)) void PageFault_Handler(interrupt_frame* frame){
    printf("Page Fault detected\n");
    while(true);
}