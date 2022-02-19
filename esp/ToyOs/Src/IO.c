#include<IO.h>

/*端口输出一个字节*/
void outb(uint16_t port,uint8_t value){
    asm volatile (
        "outb %0,%1"
        :/*输出*/
        :"a"(value),"Nd"(port)/*输入*/
        :
    );
}
/*端口读入一个字节*/
uint8_t inb(uint16_t port){
    uint8_t value;
    asm volatile (
        "inb %1,%0"
        :"=a"(value)
        :"Nd"(port)
        :
    );
    return value;
}

void IO_Wait(){
    /*向一个没用的端口发送数据*/
    asm volatile(
        "outb %%al,$0x80"
        :
        :"a"(0)
        :
    );
}