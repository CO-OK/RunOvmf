//#include"../Include/Uefi.h"
#include<stdlib.h>
#include<stdio.h>
#include <bitset>
#include<iostream>
#include<stdarg.h>
#include<limits.h>
#include<stdbool.h>
#include<assert.h>
using namespace std;
extern char _binary_font_psf_start;
extern char _binary_font_psf_end;
typedef struct
{
  uint64_t present                   :1;
  uint64_t writeable                 :1;
  uint64_t user_access               :1;
  uint64_t write_through             :1;
  uint64_t cache_disabled            :1;
  uint64_t accessed                  :1;
  uint64_t ignored_3                 :1;
  uint64_t size                      :1; // must be 0
  uint64_t ignored_2                 :4;
  uint64_t page_ppn                  :28;
  uint64_t reserved_1                :12; // must be 0
  uint64_t ignored_1                 :11;
  uint64_t execution_disabled        :1;
} PageMapLevel4Entry ;
#define PSF_FONT_MAGIC 0x864ab572
unsigned char temp[28][84]={};
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
    for(int i=0;i<20;i++){
        cout<<bitset<16>(*glyph)<<endl;
        glyph++;
    }
    
     
}
const char* file[]={
        "Char.c",
        "Kernel.c",
        "Memory32.c",
        "PageFrameAllocator.c",
        "Shell.c",
        "Video.c",
        "Assert.c"
    };
int main(){

    read_psf();
    psf_init();
   
    
}