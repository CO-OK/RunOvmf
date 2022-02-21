#include<KbScanCodeTranslation.h>

/*https://wiki.osdev.org/PS/2_Keyboard#Scan_Code_Set_1*/
const char ASCIITable[] = {
    0 ,  0 , '1', '2',
    '3', '4', '5', '6',
    '7', '8', '9', '0',
    '-', '=',  0 ,  0 ,
    'q', 'w', 'e', 'r',
    't', 'y', 'u', 'i',
    'o', 'p', '[', ']',
     0 ,  0 , 'a', 's',
    'd', 'f', 'g', 'h',
    'j', 'k', 'l', ';',
    '\'','`',  0 , '\\',
    'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',',
    '.', '/',  0 , '*',
     0 , ' '
};

char Translate(uint8_t scancode, bool uppercase){
    if(scancode>0x39)return 0;//超出上面数组的长度
    if(uppercase){
        return ASCIITable[scancode]-32;
    }
    return ASCIITable[scancode];
}