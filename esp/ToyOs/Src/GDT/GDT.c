#include<GDT.h>

__attribute__((aligned(0x1000)))

GDT DefaultGDT ={
    /*null*/
    {0,0,0,0x00,0x00,0},//
    /*内核代码*/
    {
        0,0,0,
        0x9a,//AccessByte
        0xa0,//limit and flag
        0
    },
    /*Kernel Data*/
    {
        0,0,0,
        0x92,//Access Byte
        0xa0,//limit and flag
        0
    },

    /*user 的 accessByte 应该还有问题*/

    /*user null*/
    {0,0,0,0x00,0x00,0},
    /*user code*/
    {
        0,0,0,
        0x9a,//AccessByte
        0xa0,//limit and flag
        0
    },
    /*user Data*/
    {
        0,0,0,
        0x92,//Access Byte
        0xa0,//limit and flag
        0
    },

};