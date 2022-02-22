#pragma once
#include "Uefi.h"





#pragma pack(1)

typedef struct
{
    UINT32 X;
    UINT32 Y;
} POINT;

typedef struct
{
    UINT32 X;
    UINT32 Y;
    UINT32 Color;
} PIXEL;

typedef struct
{
    POINT Start;
    POINT End;
    UINT32 Color;
} BLOCK;

#pragma pack()


int DrawPixel(PIXEL Pixel);
int DrawBlock(BLOCK Block);

/*得到屏幕上的像素信息*/
uint32_t GetPixel(uint32_t X,uint32_t Y);

/*绘制鼠标*/
void DrawOverlayMouseCursor(uint8_t* mouseCursor, POINT position, uint32_t colour);

/*清除上一个鼠标留下的痕迹*/
void ClearMouseCursor(uint8_t* mouseCursor,POINT position);