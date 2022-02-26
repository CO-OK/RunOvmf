#include <Video.h>
#include <Color.h>
#include<Assert.h>
#include<VideoInit.h>
FILENUM(5);
VIDEO_CONFIG *VideoConfig;
UINT32 *VideoStart;
UINT64 FrameBufferSize;
/* number of bytes in each line, it's possible it's not screen width * bytesperpixel! */
int scanline=0;
/*字符模式下总行数总列数*/
int Row=33;
int Col=112;
/*字符模式下光标所在位置*/
int CurrentRow=0;
int CurrentCol=0;

int VideoInitial(BOOT_CONFIG *BootConfig)
{

    VideoConfig = &BootConfig->VideoConfig;
    VideoStart = (UINT32 *)BootConfig->VideoConfig.FrameBufferBase;
    FrameBufferSize = (UINT32 *)BootConfig->VideoConfig.FrameBufferSize;
    BLOCK BackGround;

    BackGround.Start.X = 0;
    BackGround.Start.Y = 0;
    BackGround.End.X = BootConfig->VideoConfig.HorizontalResolution - 1;  
    BackGround.End.Y = BootConfig->VideoConfig.VerticalResolution - 1;
    BackGround.Color = Black;
    scanline = BootConfig->VideoConfig.PixelsPerScanLine*4;
    DrawBlock(BackGround);
    return 0;
}


int DrawPixel(PIXEL Pixel)
{
    // UINT32 *Position = VideoStart
    //                    + (Pixel.Y - 1) * VideoConfig->PixelsPerScanLine
    //                    + Pixel.X;  
    // //printf("draw %x\n",(uint32_t)Position); 
    // *Position = Pixel.Color;
    *(uint32_t*)((uint64_t)VideoConfig->FrameBufferBase + (Pixel.X*4) + (Pixel.Y * VideoConfig->PixelsPerScanLine * 4)) = Pixel.Color;
    return 0;
}

/*
int DrawPixel(UINT32* VideoHandle, UINT32 Color, int X , int Y){
    UINT32* Pixel = VideoHandle;
    Pixel += Y*1440+X;
    *Pixel=Color;
    return 0;
}
*/

int DrawBlock(BLOCK Block)
{
    PIXEL Pixel;
    Pixel.Color = Block.Color;
    for(int i = Block.Start.Y; i < Block.End.Y; i++)
    {
        for(int j = Block.Start.X; j < Block.End.X; j++)
        {
            Pixel.X = j;
            Pixel.Y = i;
            DrawPixel(Pixel);
            
        }
    }
    return 0;
}


void ClearScreen(uint32_t color){
    
    uint64_t ptr = (uint64_t)VideoConfig->FrameBufferBase;
    for(uint64_t i=0;i<VideoConfig->FrameBufferSize>>2;i++){
        *((uint64_t*)(ptr+i*4))=color;
    }
   
}

uint32_t GetPixel(uint32_t X,uint32_t Y){
    return *(uint32_t*)((uint64_t)VideoConfig->FrameBufferBase + (X*4) + (Y * VideoConfig->PixelsPerScanLine * 4));
}

/*存储未绘制鼠标前的屏幕信息*/
MouseCursorBufferBack[16*16]={};
/*存储绘制了鼠标之后的屏幕信息*/
MouseCursorBufferNow[16*16]={};


void DrawOverlayMouseCursor(uint8_t* mouseCursor, POINT position, uint32_t colour){
    int xMax = 16;
    int yMax = 16;
    int differenceX = VideoConfig->HorizontalResolution - position.X;
    int differenceY = VideoConfig->VerticalResolution - position.Y;
    PIXEL pixel;
    pixel.Color=colour;
    
    if (differenceX < 16) xMax = differenceX;
    if (differenceY < 16) yMax = differenceY;

    for (int y = 0; y < yMax; y++){
        for (int x = 0; x < xMax; x++){
            int bit = y * 16 + x;
            int byte = bit / 8;
            if ((mouseCursor[byte] & (0b10000000 >> (x % 8)))!=0)
            {
                pixel.X=position.X+x;
                pixel.Y=position.Y+y;
                MouseCursorBufferBack[16*y+x] = GetPixel(pixel.X,pixel.Y);
                DrawPixel(pixel);
                MouseCursorBufferNow[16*y+x] = pixel.Color;
            }
        }
    }
}


void ClearMouseCursor(uint8_t* mouseCursor,POINT position){
    int xMax = 16;
    int yMax = 16;
    int differenceX = VideoConfig->HorizontalResolution - position.X;
    int differenceY = VideoConfig->VerticalResolution - position.Y;
    PIXEL pixel;
    
    if (differenceX < 16) xMax = differenceX;
    if (differenceY < 16) yMax = differenceY;

    for (int y = 0; y < yMax; y++){
        for (int x = 0; x < xMax; x++){
            int bit = y * 16 + x;
            int byte = bit / 8;
            if ((mouseCursor[byte] & (0b10000000 >> (x % 8)))!=0)
            {
                pixel.X=position.X+x;
                pixel.Y=position.Y+y;
                if(GetPixel(pixel.X,pixel.Y)==MouseCursorBufferNow[x+16*y]){//不排除有其他应用已经修改了部分像素
                    pixel.Color=MouseCursorBufferBack[x+16*y];
                    DrawPixel(pixel);
                }
            }
        }
    }
}