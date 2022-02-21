#pragma once
#include<Uefi.h>
#include<Bool.h>
/*scan code set 1*/
extern const char ASCIITable[];

#define LeftShift 0x2A
#define RightShift 0x36
#define Enter 0x1C
#define BackSpace 0x0E
#define Spacebar 0x39

char Translate(uint8_t scancode, bool uppercase);