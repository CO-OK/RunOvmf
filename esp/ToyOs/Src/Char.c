#include<Char.h>
#include<Assert.h>
FILENUM(0);
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

/*前景色和背景色*/
uint32_t BackColor;
uint32_t ForeColor;

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
        if(CurrentCol==Col){
            /*换行*/
            CurrentCol=0;
            CurrentRow++;
        }
        VideoBuffer[CurrentRow][CurrentCol]=c;
        _putchar(VideoStart,c,CurrentCol,CurrentRow,ForeColor,BackColor);
        
        CurrentCol++;
        
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
          _putchar(VideoStart,VideoBuffer[i][j],j,i,ForeColor,BackColor);
      }
  }
  for(int i=0;i<Col;i++){
      VideoBuffer[Row-1][i]=' ';
      _putchar(VideoStart,' ',i,Row-1,ForeColor,BackColor);
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

    //putchar('\n');
}

void PrintHex(UINT64 Number)
{
    UINT64 Trans = Number;
    
    putchar('0');
    putchar('x');
    char Hex[16];
    int  Len = 0;
    for(int i = 0; i < 16; i++)
    {
        Hex[i] = Trans & 0xF;
        if(Hex[i] >= 0 && Hex[i] <= 9)
        {
            Hex[i] += 48;
        }
        if(Hex[i] > 9 && Hex[i] < 16)
        {
            Hex[i] += 55;
        }
        Trans = Trans >> 4;
        Len++;
        if(Trans == 0)
        {
            break;
        }
    }
    /*目前先用32位*/
    /*for(int i=0;i<8-Len;i++)
        putchar('0');*/
    for(int j = 0; j < Len; j++)
    {
        putchar(Hex[Len-j-1]);
    }  
    
    
}
void printf(const char *format, ...){
    va_list arguments;
    /* Initializing arguments to store all values after num */
    va_start (arguments, format);
    /*开始打印*/
    while(*format!='\0'){
        switch(*format){
            case '%':{
                format++;
                if(*format=='d')
                    PrintDec(va_arg(arguments,int));
                else if(*format=='u')
                    PrintDec(va_arg(arguments,UINT64));
                else if(*format=='c')
                    putchar(va_arg(arguments,char));
                else if(*format=='x')
                    PrintHex(va_arg(arguments,UINT64));
                else if(*format=='s')
                    PrintStr(va_arg(arguments,char*));
                else{
                    format--;
                    putchar('%');
                }
                break;
            }
            default:{
                putchar(*format);
                break;
            }
        }
        format++;
    }
    va_end(arguments);
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
  BackColor=Black;
  ForeColor=White;
}

void ChangeRow(int row){
    if(row==Row){
        RollBack();
        CurrentRow=row-1;
        CurrentCol=0;
        return ;
    }
    CurrentRow=row;
}
void ChangeCol(int col){
    CurrentCol=col;
}

void SetBackColor(uint32_t color){
    BackColor=color;
}
void SetForeColor(uint32_t color){
    ForeColor=color;
}

void NextLine(){
    ChangeRow(CurrentRow+1);
    ChangeCol(0);
}

void Backspace(){
    if(CurrentCol!=0)
        ChangeCol(CurrentCol-1);
    putchar(' ');
    if(CurrentCol!=0)
        ChangeCol(CurrentCol-1);
}