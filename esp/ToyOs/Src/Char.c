#include<Char.h>
/* the linear framebuffer */
//extern char *fb;
/* number of bytes in each line, it's possible it's not screen width * bytesperpixel! */
extern int scanline;
/* import our font that's in the object file we've created above */
extern char _binary_font_psf_start;
/* 屏幕在字符模式下的行数和列数*/
extern int Row;
extern int Col;
extern int CurrentRow;
extern int CurrentCol;


extern UINT32 *VideoStart;


unsigned char VideoBuffer[28][84]={};
void _putchar(uint32_t fb,
    /* note that this is int, not char as it's a unicode character */
    unsigned short int c,
    /* cursor position on screen, in characters not in pixels */
    int cx, int cy,
    /* foreground and background colors, say 0xFFFFFF and 0x000000 */
    uint32_t fg, uint32_t bg)
{
    /* cast the address to PSF header struct */
    PSF_font *font = (PSF_font*)&_binary_font_psf_start;
    /* we need to know how many bytes encode one row */
    int bytesperline=(font->width+7)/8;
    /* get the glyph for the character. If there's no
       glyph for a given character, we'll display the first glyph. */
    unsigned char *glyph =
     (unsigned char*)&_binary_font_psf_start +
     font->headersize +
     (c>0&&c<font->numglyph?c:0)*font->bytesperglyph;
    /* calculate the upper left corner on screen where we want to display.
       we only do this once, and adjust the offset later. This is faster. */
    int offs =
        (cy * font->height * scanline) +
        (cx * (font->width + 1) * sizeof(ONEPIXEL));
    /* finally display pixels according to the bitmap */
    int x,y, line,mask;
    for(y=0;y<font->height;y++){
        /* save the starting position of the line */
        line=offs;
        mask=1<<(font->width-1);
        /* display a row */
        for(x=0;x<font->width;x++){
            *((ONEPIXEL*)(fb + line)) = *((unsigned int*)glyph) & mask ? fg : bg;
            /* adjust to the next pixel */
            mask >>= 1;
            line += sizeof(ONEPIXEL);
        }
        /* adjust to the next line */
        glyph += bytesperline;
        offs  += scanline;
    }
}

void putchar(unsigned short c){
    /* 如果需要回滚 */
    if(CurrentRow==Row){
        RollBack();
        CurrentRow--;
        CurrentCol=0;
    }
    switch (c)
    {
    case '\n':
        CurrentCol=0;
        CurrentRow++;
        break;
    
    default:
        _putchar(VideoStart,c,CurrentCol,CurrentRow,White,Black);
        VideoBuffer[CurrentRow][CurrentCol]=c;
        CurrentCol++;
        if(CurrentCol==Col){
            /*换行*/
            CurrentCol=0;
            CurrentRow++;
        }
        break;
    }
    
        
}

void RollBack(){
   /*一行字符所占像素总数*/
  for(int i=1;i<Row;i++){
      for(int j=0;j<Col;j++){
          VideoBuffer[i-1][j]=VideoBuffer[i][j];
      }
  }
  for(int i=0;i<Row-1;i++){
      for(int j=0;j<Col;j++){
          _putchar(VideoStart,VideoBuffer[i][j],j,i,White,Black);
      }
  }
  for(int i=0;i<Col;i++){
      _putchar(VideoStart,' ',i,Row-1,Black,Black);
  }
  
}


void PrintStr(char* str){
    while(*str!='\0'){
        putchar(*str);
        str++;
    }
    
}

void PrintDec(UINTN num){
    char Dec[20];
    UINT8 Len = 0;
    UINT64 Trans = num;
    for(int i = 0; i < 20; i++)
    {
        Dec[i] = (Trans % 10) + 48;
        Trans = Trans / 10;
        Len++;
        if(Trans == 0)
        {
            break;
        }  
    }

    for(int j = Len-1; j >= 0; j--)
    {
        putchar(Dec[j]);
    }

    putchar('\n');
}






void CharInit(){
    /*
        初始化字体改成正常形式
    */
   /*
    字体文件中存放的bitmap是反对称的如A:
    0000000000000000
    0000000000000000
    0000000000000000
    0000000000000000
    0000000000000000
    0000000000000000
    1111000000001111
    1111100000011111
    0001110000111000
    0000110000110000
    0000110000110000
    0000110000110000
    0000110000110000
    0000110000110000
    0000110000110000
    0000110000110000
    1111110000111111
    1111110000111111
    0000110000110000
    0000110000110000
    0000110000110000
    0000110000110000
    0000110000110000
    0000110000110000
    0000110000110000
    0000110000110000
    0000000000000000
    0000000000000000
    0000000000000000
    0000000000000000
    0000000000000000
    0000000000000000
    需要将左右部分互换！
    我们可以只在初始阶段互换一次即可
    */
    PSF_font *font = (PSF_font*)&_binary_font_psf_start;
    /* we need to know how many bytes encode one row */
    int bytesperline=(font->width+7)/8;
    /* unicode translation */

    /* get the glyph for the character. If there's no
       glyph for a given character, we'll display the first glyph. */
    unsigned char *glyph =
     (unsigned char*)&_binary_font_psf_start +
     font->headersize;

    for(int i=0;i<font->numglyph;i++){
        //互换左右两边
        unsigned char* temp = glyph;
        unsigned char c;
        for(int j=0;j<font->height;j++){
            c=*temp;
            *temp=*(temp+1);
            *(temp+1)=c;
            temp+=bytesperline;
        }
        glyph+=font->bytesperglyph;
    }
    for(int i=0;i<Row;i++){
      for(int j=0;j<Col;j++){
          VideoBuffer[i][j]=' ';
      }
  }
  Row=28;
  Col=84;
}