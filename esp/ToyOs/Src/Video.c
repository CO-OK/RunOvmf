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
int Row=28;
int Col=84;
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
    UINT32 *Position = VideoStart
                       + (Pixel.Y - 1) * VideoConfig->PixelsPerScanLine
                       + Pixel.X;   
    *Position = Pixel.Color;
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
