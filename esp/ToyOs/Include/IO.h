#pragma once
#include<Uefi.h>
/*端口输出一个字节*/
void outb(uint16_t port,uint8_t value);
/*端口读入一个字节*/
uint8_t inb(uint16_t port);

/*等待*/
void IO_Wait();