//#include"../Include/Uefi.h"
#include<stdlib.h>
#include<stdio.h>
#include <bitset>
#include<iostream>
#include<limits.h>
using namespace std;
extern char _binary_font_psf_start;
extern char _binary_font_psf_end;

#define PSF_FONT_MAGIC 0x864ab572
 
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

uint16_t *unicode;
void read_psf(){
    PSF_font *font = (PSF_font*)&_binary_font_psf_start;
    printf("num glyph=%d\n",font->numglyph);
    printf("bytes per glyph=%d\n",font->bytesperglyph);
    printf("height=%d\n",font->height);
    printf("width=%d\n",font->width);
}
void psf_init()
{
    
    /* cast the address to PSF header struct */
    PSF_font *font = (PSF_font*)&_binary_font_psf_start;
    int bytesperline=(font->width+7)/8;
    printf("bytesperline=%d\n",bytesperline);
    unsigned short *glyph =(unsigned short*)((unsigned char*)&_binary_font_psf_start +font->headersize+font->bytesperglyph*'A') ;
    for(int i=0;i<32;i++){
        cout<<bitset<16>(*glyph)<<endl;
        glyph++;
    }
     
}
int main(){
    read_psf();
    psf_init();
}