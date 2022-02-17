#pragma once
#include<Color.h>
#include<Video.h>
#define ONEPIXEL uint32_t   /* pixel pointer */
/*printf相关这些好像都是编译器内置所以直接声明了*/
typedef __builtin_va_list va_list;
#define va_start(ap, param) __builtin_va_start(ap, param)
#define va_end(ap)          __builtin_va_end(ap)
#define va_arg(ap, type)    __builtin_va_arg(ap, type)


/*字体文件头结构体*/
typedef struct {
    uint32_t magic;         /* magic bytes to identify PSF */
    uint32_t version;       /* zero */
    uint32_t headersize;    /* offset of bitmaps in file, 32 */
    uint32_t flags;         /* 0 if there's no unicode table */
    uint32_t numglyph;      /* number of glyphs */
    uint32_t bytesperglyph; /* size of each glyph */
    uint32_t height;        /* height in pixels */
    uint32_t width;         /* width in pixels */
} PSF_font;

void _putchar(uint32_t fb,
    /* note that this is int, not char as it's a unicode character */
    unsigned short int c,
    /* cursor position on screen, in characters not in pixels */
    int cx, int cy,
    /* foreground and background colors, say 0xFFFFFF and 0x000000 */
    uint32_t fg, uint32_t bg);

void putchar(unsigned short c);
void CharInit();
void RollBack();
/*打印字符串*/
void PrintStr(char* str);
/*打印10进制*/
void PrintDec(UINTN num);
/*打印16进制*/
void PrintHex(UINT64 Number);

/*printf*/
void printf(const char *format, ...);

/*改变光标所在位置*/
void ChangeRow(int row);
void ChangeCol(int col);
