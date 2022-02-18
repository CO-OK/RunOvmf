
[bits 64]
; https://wiki.osdev.org/GDT_Tutorial#How_to_Set_Up_The_GDT
; function

LoadGDT:
    ; rdi是默认的第一个参数寄存器
    lgdt [rdi]
    

    ; update code and data segments
    mov ax,0x10 ;0x10是kernelData Segment 在GDT的序号 相当于数组下标
    
    ; 要生效必须把段选择子重新载入段寄存器
    ; but changing the CS register requires code resembling a jump or call to elsewhere, as this is the only way its value is meant to be changed
    
    mov ds,ax
    
    mov es,ax
    mov fs,ax
    mov gs,ax
    mov ss,ax
    
    ; 调用函数之前压进栈的是返回地址
    pop rdi
    mov rax,0x08 ; Kernel code segment 序号

    push rax
    push rdi
    ; 处理器先从栈中弹出一个字到IP，再弹出一个字到CS
    retfq


 




GLOBAL LoadGDT
